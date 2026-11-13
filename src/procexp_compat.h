#pragma once
#include <ntifs.h>
#include <wdmsec.h>
#include <intrin.h>
#include <stdbool.h>

typedef unsigned char _BYTE;
typedef unsigned short _WORD;
typedef unsigned int _DWORD;
typedef unsigned __int64 _QWORD;

/* Exported kernel routines omitted by the public ntifs.h surface. */
typedef ULONG SYSTEM_INFORMATION_CLASS;
NTSYSAPI NTSTATUS NTAPI ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG);
NTSYSAPI NTSTATUS NTAPI ZwQueryInformationProcess(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
NTSYSAPI NTSTATUS NTAPI ZwOpenProcessToken(HANDLE, ACCESS_MASK, PHANDLE);
NTKERNELAPI NTSTATUS NTAPI ObOpenObjectByName(POBJECT_ATTRIBUTES, POBJECT_TYPE,
    KPROCESSOR_MODE, PACCESS_STATE, ACCESS_MASK, PVOID, PHANDLE);

/* Preserve lvalue access used in the decompiler's register-width expressions. */
#define LOBYTE(x) (*((UCHAR *)&(x)))
#define LODWORD(x) (*((ULONG *)&(x)))

C_ASSERT(sizeof(OBJECT_ATTRIBUTES) == 48);
C_ASSERT(sizeof(CLIENT_ID) == 16);
C_ASSERT(FIELD_OFFSET(IO_STACK_LOCATION, Parameters.DeviceIoControl.IoControlCode) == 0x18);
C_ASSERT(FIELD_OFFSET(IO_STATUS_BLOCK, Information) == 8);

typedef NTSTATUS (NTAPI *PROCEXP_ACQUIRE_EXIT_SYNC)(PEPROCESS);
typedef VOID (NTAPI *PROCEXP_RELEASE_EXIT_SYNC)(PEPROCESS);
typedef SIZE_T (NTAPI *PROCEXP_GET_POOL_SIZE)(VOID);
typedef POBJECT_TYPE (NTAPI *PROCEXP_GET_OBJECT_TYPE)(PVOID);

#include "procexp_globals.h"
#include "procexp_functions.h"
