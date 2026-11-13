#pragma once

NTSTATUS __fastcall ProcExpDuplicateHandle(unsigned int a1, void *a2, ACCESS_MASK a3, void **a4);
NTSTATUS __fastcall ProcExpCloseRemoteHandle(HANDLE *a1);
__int64 __fastcall ProcExpDeviceControl(
        __int64 a1,
        __int64 a2,
        HANDLE *a3,
        int a4,
        PHANDLE ProcessHandle,
        size_t MaxCount,
        ULONG ReturnLength,
        NTSTATUS *Object);
NTSTATUS __fastcall ProcExpQueryFileName(char a1, __int64 a2, struct _OBJECT_NAME_INFORMATION *a3, ULONG a4);
NTSTATUS __fastcall ProcExpQueryThreadContext(void *a1, _QWORD *a2, _QWORD *a3);
NTSTATUS __fastcall ProcExpQueryMutantOwner(void *a1, void **a2, _QWORD *a3);
NTSTATUS __fastcall ProcExpQueryObjectType(char a1, unsigned int *a2, __int64 a3, _QWORD *a4);
NTSTATUS __fastcall ProcExpDuplicateRequestHandle(__int64 a1, void **a2);
NTSTATUS __fastcall ProcExpQueryExecuteFlags(void *a1, _DWORD *a2, _QWORD *a3);
NTSTATUS __fastcall ProcExpReadThreadMemory(__int64 a1, _BYTE *a2, _QWORD *a3);
NTSTATUS __fastcall ProcExpReopenHandle(void *a1, void **a2, ACCESS_MASK a3);
PVOID ProcExpQueryModules();

DRIVER_INITIALIZE DriverEntry;
DRIVER_DISPATCH ProcExpDispatch;
DRIVER_UNLOAD ProcExpUnload;
POBJECT_TYPE ProcExpLookupObjectType(PCWSTR Name);
NTSTATUS ProcExpQueryObjectName(char Unicode, ULONG *Request, LONG_PTR Output, LONG64 *OutputSize);
