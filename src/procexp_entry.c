#include "procexp_compat.h"

/* Original RVA 0x2E40. Hex-Rays split UNICODE_STRING/OBJECT_ATTRIBUTES
 * into overlapping scalar locals; reconstruct the actual structures here. */
POBJECT_TYPE ProcExpLookupObjectType(PCWSTR Name)
{
    POBJECT_TYPE type;
    UNICODE_STRING objectName;
    OBJECT_ATTRIBUTES attributes;
    HANDLE handle;
    PVOID object;
    PWCHAR buffer;
    SIZE_T chars = 0;
    NTSTATUS status;

    if (!gGetObjectType) return NULL;
    type = gGetObjectType(*PsProcessType);
    while (Name[chars]) ++chars;
    buffer = ExAllocatePoolWithTag(PagedPool, chars * sizeof(WCHAR) + 26, 'TOEP');
    if (!buffer) return NULL;
    RtlCopyMemory(buffer, L"\\ObjectTypes\\", 26);
    RtlCopyMemory((PUCHAR)buffer + 26, Name, chars * sizeof(WCHAR));
    objectName.Buffer = buffer;
    objectName.Length = objectName.MaximumLength = (USHORT)(chars * sizeof(WCHAR) + 26);
    InitializeObjectAttributes(&attributes, &objectName, OBJ_KERNEL_HANDLE, NULL, NULL);
    status = ObOpenObjectByName(&attributes, type, KernelMode, NULL, 0xF0001, NULL, &handle);
    ExFreePoolWithTag(buffer, 0);
    if (!NT_SUCCESS(status)) return NULL;
    status = ObReferenceObjectByHandle(handle, 0xF0001, type, KernelMode, &object, NULL);
    ZwClose(handle);
    if (!NT_SUCCESS(status)) return NULL;
    ObDereferenceObject(object);
    return (POBJECT_TYPE)object;
}

/* Original RVA 0x15D0. Wdmsec.lib supplies the statically linked WDK helpers. */
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNICODE_STRING name, deviceName, symbolicName;
    UNICODE_STRING sddl = RTL_CONSTANT_STRING(L"D:P(A;;GA;;;SY)(A;;GA;;;BA)");
    PDEVICE_OBJECT device = NULL;
    NTSTATUS status;
    UNREFERENCED_PARAMETER(RegistryPath);

    PsGetVersion(NULL, NULL, &gBuildNumber, NULL);
    if (gBuildNumber < 2600) return STATUS_NOT_SUPPORTED;
    if (gBuildNumber >= 6000) {
        RtlInitUnicodeString(&name, L"PsAcquireProcessExitSynchronization");
        gAcquireProcessExitSynchronization = (PROCEXP_ACQUIRE_EXIT_SYNC)MmGetSystemRoutineAddress(&name);
        RtlInitUnicodeString(&name, L"PsReleaseProcessExitSynchronization");
        gReleaseProcessExitSynchronization = (PROCEXP_RELEASE_EXIT_SYNC)MmGetSystemRoutineAddress(&name);
        RtlInitUnicodeString(&name, L"MmGetMaximumNonPagedPoolInBytes");
        gGetMaximumNonPagedPoolInBytes = (PROCEXP_GET_POOL_SIZE)MmGetSystemRoutineAddress(&name);
    }
    if (gBuildNumber >= 7600) {
        RtlInitUnicodeString(&name, L"ObGetObjectType");
        gGetObjectType = (PROCEXP_GET_OBJECT_TYPE)MmGetSystemRoutineAddress(&name);
    }
    gMutantObjectType = ProcExpLookupObjectType(L"Mutant");
    RtlInitUnicodeString(&deviceName, L"\\Device\\PROCEXP152");
    status = IoCreateDeviceSecure(DriverObject, 0, &deviceName, 0x8335,
                                0, FALSE, &sddl, NULL, &device);
    if (NT_SUCCESS(status)) {
        device->Flags &= ~DO_DEVICE_INITIALIZING;
        RtlInitUnicodeString(&symbolicName, L"\\DosDevices\\PROCEXP152");
        status = IoCreateSymbolicLink(&symbolicName, &deviceName);
        DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ProcExpDispatch;
        DriverObject->MajorFunction[IRP_MJ_CLOSE] = ProcExpDispatch;
        DriverObject->MajorFunction[IRP_MJ_CREATE] = ProcExpDispatch;
        DriverObject->DriverUnload = ProcExpUnload;
    }
    if (!NT_SUCCESS(status) && device) IoDeleteDevice(device);
    return status;
}

/* Original RVA 0x2220. Use documented union members rather than IDA aliases. */
NTSTATUS ProcExpDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;
    PRIVILEGE_SET privileges;
    SECURITY_SUBJECT_CONTEXT context;
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Information = 0;
    if (stack->MajorFunction == IRP_MJ_CREATE) {
        privileges.PrivilegeCount = 1;
        privileges.Control = PRIVILEGE_SET_ALL_NECESSARY;
        privileges.Privilege[0].Luid.LowPart = SE_DEBUG_PRIVILEGE;
        privileges.Privilege[0].Luid.HighPart = 0;
        privileges.Privilege[0].Attributes = 0;
        SeCaptureSubjectContext(&context);
        if (!SePrivilegeCheck(&privileges, &context, ExGetPreviousMode()))
            status = STATUS_ACCESS_DENIED;
        SeReleaseSubjectContext(&context);
    } else if (stack->MajorFunction == IRP_MJ_DEVICE_CONTROL) {
        status = (NTSTATUS)ProcExpDeviceControl((LONG_PTR)stack->FileObject, 1,
            Irp->AssociatedIrp.SystemBuffer,
            stack->Parameters.DeviceIoControl.InputBufferLength,
            Irp->AssociatedIrp.SystemBuffer,
            stack->Parameters.DeviceIoControl.OutputBufferLength,
            stack->Parameters.DeviceIoControl.IoControlCode,
            (NTSTATUS *)&Irp->IoStatus);
    } else {
        /* The original also rejects IRP_MJ_CLOSE despite registering it. */
        status = STATUS_INVALID_DEVICE_REQUEST;
    }
    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

/* Original RVA 0x3280. */
VOID ProcExpUnload(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING symbolicName;
    RtlInitUnicodeString(&symbolicName, L"\\DosDevices\\PROCEXP152");
    IoDeleteSymbolicLink(&symbolicName);
    IoDeleteDevice(DriverObject->DeviceObject);
}
