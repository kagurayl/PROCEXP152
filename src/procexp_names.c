#include "procexp_compat.h"

/* The exception scopes at original RVAs 0x292F..0x2AFF and 0x2B11..0x2B21
 * are absent from Hex-Rays pseudocode. Restore them from the PE scope table. */
static NTSTATUS ProcExpRenderObjectName(char Unicode, ULONG *Request,
    LONG_PTR Output, LONG64 *OutputSize, PVOID Object, ULONG capacity, NTSTATUS status)
{
    ULONG returnLength;
    ANSI_STRING ansi;
    PUNICODE_STRING name = (PUNICODE_STRING)(Output + 4);
    SIZE_T length;
    BOOLEAN released = FALSE;

    *(ULONG *)Output = 0;
    if (Unicode) *(WCHAR *)(Output + 4) = 0;
    else *(CHAR *)(Output + 4) = 0;

    __try {
        if (*((UCHAR *)Request + 16) && *(USHORT *)Object == 5) {
            if (*((USHORT *)Object + 1) != 216) {
                status = STATUS_INVALID_PARAMETER;
                goto Done;
            }
            if (*((UCHAR *)Object + 77)) *(ULONG *)Output |= 1;
            if (*((UCHAR *)Object + 78)) *(ULONG *)Output |= 2;
            if (*((UCHAR *)Object + 79)) *(ULONG *)Output |= 4;
            status = ProcExpQueryFileName(Unicode, (LONG_PTR)Object,
                (POBJECT_NAME_INFORMATION)(Output + 4), capacity);
            if (NT_SUCCESS(status)) goto CountName;
        }
        status = ObQueryNameString(Object, (POBJECT_NAME_INFORMATION)(Output + 4),
                                   capacity, &returnLength);
        if (!NT_SUCCESS(status) || !name->Length) goto Done;
        if (Unicode) {
            length = name->Length / sizeof(WCHAR);
            memmove((PVOID)(Output + 4), name->Buffer, length * sizeof(WCHAR));
            ((PWCHAR)(Output + 4))[length] = 0;
        } else {
            status = RtlUnicodeStringToAnsiString(&ansi, name, TRUE);
            if (NT_SUCCESS(status)) {
                length = ansi.Length;
                memmove((PVOID)(Output + 4), ansi.Buffer, length);
                ((PCHAR)(Output + 4))[length] = 0;
                RtlFreeAnsiString(&ansi);
            }
        }
CountName:
        length = 0;
        if (Unicode) {
            while (((PWCHAR)(Output + 4))[length]) ++length;
            *OutputSize = 2 * length + 10;
        } else {
            while (((PCHAR)(Output + 4))[length]) ++length;
            *OutputSize = length + 9;
        }
Done:
        ObDereferenceObject(Object);
        released = TRUE;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        status = STATUS_ACCESS_VIOLATION;
        if (!released) {
            __try { ObDereferenceObject(Object); }
            __except (EXCEPTION_EXECUTE_HANDLER) { /* original suppresses this */ }
        }
    }
    return status;
}

/* Original RVA 0x2680. The 32-byte request uses PID at +0, expected object
 * at +8, file-object flag at +16, and source handle at +24. */
NTSTATUS ProcExpQueryObjectName(char Unicode, ULONG *Request,
                              LONG_PTR Output, LONG64 *OutputSize)
{
    ULONG pid = Request[0];
    HANDLE sourceHandle = *(PHANDLE)((PUCHAR)Request + 24);
    PVOID expected = *(PVOID *)((PUCHAR)Request + 8);
    PVOID object = NULL;
    PEPROCESS process;
    HANDLE processHandle, duplicate;
    CLIENT_ID clientId;
    OBJECT_ATTRIBUTES attributes;
    KAPC_STATE apc;
    BOOLEAN synchronized = FALSE;
    NTSTATUS status = STATUS_SUCCESS;
    ULONG capacity = (ULONG)(*OutputSize - 8);

    /* The original initializes Information to eight before resolving the name. */
    *OutputSize = 8;
    if (pid >= 8 && !gAcquireProcessExitSynchronization) {
        InitializeObjectAttributes(&attributes, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
        clientId.UniqueProcess = (HANDLE)(ULONG_PTR)pid;
        clientId.UniqueThread = NULL;
        status = ZwOpenProcess(&processHandle, PROCESS_DUP_HANDLE, &attributes, &clientId);
        if (!NT_SUCCESS(status)) return STATUS_ACCESS_DENIED;
        status = ZwDuplicateObject(processHandle, sourceHandle, NtCurrentProcess(),
                                   &duplicate, 0, 0, DUPLICATE_SAME_ACCESS);
        if (!NT_SUCCESS(status)) {
            ZwClose(processHandle);
            return STATUS_ACCESS_DENIED;
        }
        ZwClose(processHandle);
        status = ObReferenceObjectByHandle(duplicate, 0,
            *((PUCHAR)Request + 16) ? *IoFileObjectType : NULL,
            UserMode, &object, NULL);
        if (!NT_SUCCESS(status)) {
            ObCloseHandle(duplicate, UserMode);
            return STATUS_ACCESS_DENIED;
        }
        ObCloseHandle(duplicate, UserMode);
        if (object != expected) {
            ObDereferenceObject(object);
            return STATUS_ACCESS_DENIED;
        }
    } else {
        status = PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)pid, &process);
        if (!NT_SUCCESS(status)) return status;
        if (gAcquireProcessExitSynchronization) {
            status = gAcquireProcessExitSynchronization(process);
            synchronized = NT_SUCCESS(status);
        }
        if (NT_SUCCESS(status)) {
            KeStackAttachProcess(process, &apc);
            /* Preserve the legacy System-process handle sign extension. */
            if (pid < 8)
                sourceHandle = (HANDLE)((ULONG_PTR)(ULONG)(ULONG_PTR)sourceHandle | 0xFFFFFFFF80000000ULL);
            status = ObReferenceObjectByHandle(sourceHandle, 0, NULL,
                pid >= 8 ? UserMode : KernelMode, &object, NULL);
            if (NT_SUCCESS(status) && object != expected) {
                ObDereferenceObject(object);
                status = STATUS_INVALID_PARAMETER;
            }
            KeUnstackDetachProcess(&apc);
            if (synchronized && gReleaseProcessExitSynchronization)
                gReleaseProcessExitSynchronization(process);
        }
        ObDereferenceObject(process);
        if (!NT_SUCCESS(status)) return status;
    }
    return ProcExpRenderObjectName(Unicode, Request, Output, OutputSize, object, capacity, status);
}
