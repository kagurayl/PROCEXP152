/* Reconstructed from IDA; see dev/RECONSTRUCTION.txt. */
#include "procexp_compat.h"
/* Preserve the original uninitialized-handle path; do not repair it. */
#pragma warning(3:4703)

/* Original RVA 0x1950: sub_180001950 */
NTSTATUS __fastcall ProcExpDuplicateHandle(unsigned int a1, void *a2, ACCESS_MASK a3, void **a4)
{
  NTSTATUS result; // eax
  ULONG Options; // eax
  NTSTATUS v9; // ebx
  HANDLE SourceProcessHandle; // [rsp+40h] [rbp-58h] BYREF
  CLIENT_ID v11; // [rsp+48h] [rbp-50h] BYREF
  struct _OBJECT_ATTRIBUTES v12; // [rsp+58h] [rbp-40h] BYREF

  v12.Length = 48;
  v12.RootDirectory = NULL;
  v12.Attributes = 512;
  v12.ObjectName = NULL;
  v11.UniqueProcess = (HANDLE)(ULONG_PTR)a1;
  v11.UniqueThread = NULL;
  RtlZeroMemory(&v12.SecurityDescriptor, 16);
  result = ZwOpenProcess(&SourceProcessHandle, 0x40u, &v12, (PCLIENT_ID)&v11);
  if ( result >= 0 )
  {
    Options = 2;
    if ( a3 != 0 )
      Options = 0;
    v9 = ZwDuplicateObject(SourceProcessHandle, a2, (HANDLE)0xFFFFFFFFFFFFFFFFLL, a4, a3, 0, Options);
    ZwClose(SourceProcessHandle);
    if ( v9 >= 0 )
      return 0;
    else
      return v9;
  }
  return result;
}


/* Original RVA 0x1A20: sub_180001A20 */
NTSTATUS __fastcall ProcExpCloseRemoteHandle(HANDLE *a1)
{
  NTSTATUS result; // eax
  NTSTATUS v3; // edi
  PVOID v4; // rcx
  struct _KAPC_STATE ApcState; // [rsp+30h] [rbp-38h] BYREF
  PEPROCESS Process; // [rsp+70h] [rbp+8h] BYREF
  PVOID Object; // [rsp+78h] [rbp+10h] BYREF
  struct _OBJECT_HANDLE_INFORMATION HandleInformation; // [rsp+80h] [rbp+18h] BYREF

  result = PsLookupProcessByProcessId((HANDLE)*(unsigned int *)a1, &Process);
  if ( result >= 0 )
  {
    KeStackAttachProcess(Process, &ApcState);
    __try {
    v3 = ObReferenceObjectByHandle(a1[3], 0, NULL, 1, &Object, &HandleInformation);
    if ( v3 >= 0 )
    {
      v4 = Object;
      if ( Object == a1[1] )
      {
        ObCloseHandle(a1[3], 1);
        v4 = Object;
      }
      ObfDereferenceObject(v4);
    }
    } __finally {
      KeUnstackDetachProcess(&ApcState);
      ObfDereferenceObject(Process);
    }
    return v3;
  }
  return result;
}


/* Original RVA 0x1AE0: sub_180001AE0 */
__int64 __fastcall ProcExpDeviceControl(
        __int64 a1,
        __int64 a2,
        HANDLE *a3,
        int a4,
        PHANDLE ProcessHandle,
        size_t MaxCount,
        ULONG ReturnLength,
        NTSTATUS *Object)
{
  NTSTATUS *v8; // rbx
  __int64 v10; // rax
  char v11; // r10
  NTSTATUS v12; // r14d
  NTSTATUS v13; // eax
  HANDLE v14; // rcx
  _WORD *v15; // rcx
  unsigned __int64 v16; // rcx
  _DWORD *v17; // rdx
  HANDLE v18; // rcx
  NTSTATUS v19; // eax
  NTSTATUS *v20; // rcx
  _QWORD *v21; // r8
  NTSTATUS v22; // eax
  NTSTATUS *v23; // rcx
  void *v24; // r10
  HANDLE v25; // rcx
  NTSTATUS v26; // edi
  __int64 result; // rax
  HANDLE Handle; // [rsp+40h] [rbp-C0h] BYREF
  union _LARGE_INTEGER Timeout; // [rsp+48h] [rbp-B8h] BYREF
  struct _CLIENT_ID ClientId; // [rsp+50h] [rbp-B0h] BYREF
  struct _OBJECT_ATTRIBUTES ObjectAttributes; // [rsp+60h] [rbp-A0h] BYREF
  struct _KAPC_STATE ApcState; // [rsp+90h] [rbp-70h] BYREF
  UCHAR SystemInformation[0x130]; // [rsp+C0h] [rbp-40h] BYREF
  /* module ImageBase at +0x18 */ // [rsp+D8h] [rbp-28h]
  /* module ImageSize at +0x20 */ // [rsp+E0h] [rbp-20h]

  v8 = Object;
  v10 = ReturnLength + 2093678592;
  v11 = 1;
  *Object = 0;
  *((_QWORD *)v8 + 1) = 0;
  switch ( v10 )
  {
    case 0LL:
      v11 = 0;
      goto LABEL_29;
    case 4LL:
      if ( a4 != 32 || (_DWORD)MaxCount != 0 )
        goto LABEL_79;
      *v8 = ProcExpCloseRemoteHandle(a3);
      break;
    case 8LL:
      if ( a4 != 4 || (_DWORD)MaxCount != 4 )
        goto LABEL_79;
      if ( *(_DWORD *)a3 <= 0x98u )
      {
        *(_DWORD *)ProcessHandle = 152;
        *((_QWORD *)v8 + 1) = 4;
      }
      else
      {
        *v8 = -1073741735;
      }
      break;
    case 12LL:
      if ( a4 != 8 || (_DWORD)MaxCount != 8 )
        goto LABEL_79;
      v12 = ObReferenceObjectByHandle(*a3, 0, NULL, 1, (PVOID *)&Object, NULL);
      if ( v12 >= 0 )
      {
        v12 = ObOpenObjectByPointer(Object, 0x200u, NULL, 0x10000000u, NULL, 0, &Handle);
        ObfDereferenceObject(Object);
      }
      *v8 = v12;
      if ( v12 >= 0 )
      {
        v13 = ZwOpenProcessToken(Handle, 8u, ProcessHandle);
        v14 = Handle;
        *v8 = v13;
        ZwClose(v14);
      }
      *((_QWORD *)v8 + 1) = 8;
      break;
    case 20LL:
      if ( a4 != 32 || (_DWORD)MaxCount != 8 )
        goto LABEL_79;
      *v8 = ProcExpDuplicateHandle(*(_DWORD *)a3, a3[3], 0x10000000u, ProcessHandle);
      *((_QWORD *)v8 + 1) = 8;
      break;
    case 32LL:
      if ( a4 != 8 )
        goto LABEL_79;
      if ( (_DWORD)MaxCount != 8 )
        goto LABEL_79;
      v15 = *a3;
      if ( *(_WORD *)*a3 != 5 || v15[1] != 216 )
        goto LABEL_79;
      *ProcessHandle = *((void **)v15 + 4);
      *((_QWORD *)v8 + 1) = 8;
      break;
    case 36LL:
      if ( a4 != 16 )
        goto LABEL_79;
      *((_QWORD *)v8 + 1) = (unsigned int)MaxCount;
      *v8 = ProcExpReadThreadMemory((LONG_PTR)a3, (_BYTE *)ProcessHandle, (_QWORD *)(v8 + 2));
      break;
    case 40LL:
      if ( a4 != 8 || (_DWORD)MaxCount != 24 )
        goto LABEL_79;
      v18 = *a3;
      Timeout.QuadPart = 0;
      v19 = ObReferenceObjectByHandle(v18, 0, *PsThreadType, 1, (PVOID *)&Object, NULL);
      if ( v19 < 0 )
        goto LABEL_63;
      if ( KeWaitForSingleObject(Object, Executive, 0, 0, &Timeout) != 0 )
      {
        v20 = Object;
        if ( gBuildNumber < 0x23F0 )
        {
          if ( gBuildNumber < 0x1770 )
          {
            if ( gBuildNumber == 3790 )
              v21 = *((_QWORD **)Object + 7);
            else
              v21 = *((_QWORD **)Object + 9);
          }
          else
          {
            v21 = *((_QWORD **)Object + 7);
          }
        }
        else
        {
          v21 = *((_QWORD **)Object + 11);
        }
        ProcessHandle[1] = v21 + 8;
        *ProcessHandle = (void *)v21[7];
        ProcessHandle[2] = (void *)v21[6];
        ObfDereferenceObject(v20);
        v19 = 0;
        *((_QWORD *)v8 + 1) = 24;
LABEL_63:
        *v8 = v19;
      }
      else
      {
        *v8 = -1073741749;
      }
      break;
    case 44LL:
      if ( a4 != 8 || (_DWORD)MaxCount != 8 )
        goto LABEL_79;
      v22 = ObReferenceObjectByHandle(*a3, 0, gMutantObjectType, 1, (PVOID *)&Object, NULL);
      if ( v22 >= 0 )
      {
        v23 = Object;
        v24 = *((void **)Object + 5);
        if ( v24 != NULL )
        {
          ObOpenObjectByPointer(v24, 0x200u, NULL, 0x10000000u, *PsThreadType, 0, ProcessHandle);
          v23 = Object;
        }
        else
        {
          *ProcessHandle = NULL;
        }
        ObfDereferenceObject(v23);
        v22 = 0;
        *((_QWORD *)v8 + 1) = 8;
      }
      *v8 = v22;
      break;
    case 52LL:
      if ( a4 != 8 || (_DWORD)MaxCount != 4 )
        goto LABEL_79;
      v25 = *a3;
      *(_DWORD *)ProcessHandle = -1;
      v26 = ObReferenceObjectByHandle(v25, 0x400u, *PsProcessType, 1, (PVOID *)&Object, NULL);
      if ( v26 >= 0 )
      {
        KeStackAttachProcess((PRKPROCESS)Object, &ApcState);
        v26 = ZwQueryInformationProcess(
                (HANDLE)0xFFFFFFFFFFFFFFFFLL,
                (PROCESSINFOCLASS)34,
                ProcessHandle,
                4u,
                &ReturnLength);
        KeUnstackDetachProcess(&ApcState);
        ObfDereferenceObject(Object);
        if ( v26 >= 0 )
          *((_QWORD *)v8 + 1) = ReturnLength;
      }
      *v8 = v26;
      break;
    case 60LL:
      if ( a4 != 8 || (_DWORD)MaxCount != 8 )
        goto LABEL_79;
      ClientId.UniqueProcess = *a3;
      ClientId.UniqueThread = NULL;
      RtlZeroMemory(&ObjectAttributes.SecurityDescriptor, 16);
      ObjectAttributes.Length = 48;
      memset(&ObjectAttributes.RootDirectory, 0, 20);
      *v8 = ZwOpenProcess(ProcessHandle, 0x10000000u, &ObjectAttributes, &ClientId);
      *((_QWORD *)v8 + 1) = 8;
      break;
    case 64LL:
      v11 = 0;
      goto LABEL_33;
    case 68LL:
      if ( a4 != 8 || (unsigned int)MaxCount < 4 )
        goto LABEL_79;
      (*(_QWORD *)(SystemInformation + 0x18)) = 0;
      if ( ZwQuerySystemInformation((SYSTEM_INFORMATION_CLASS)11, SystemInformation, 0x130u, NULL) < 0 )
      {
        v16 = (*(_QWORD *)(SystemInformation + 0x18)) + (*(_DWORD *)(SystemInformation + 0x20));
        v17 = *a3;
        if ( (unsigned __int64)*a3 < (*(_QWORD *)(SystemInformation + 0x18))
          || (unsigned __int64)v17 > v16
          || (unsigned __int64)v17 + (unsigned int)MaxCount > v16 )
        {
          *v8 = -1073741811;
        }
        else
        {
          if ( (_DWORD)MaxCount == 4 )
            *(_DWORD *)ProcessHandle = *v17;
          else
            memmove(ProcessHandle, v17, (unsigned int)MaxCount);
          *((_QWORD *)v8 + 1) = (unsigned int)MaxCount;
          *v8 = 0;
        }
      }
      else
      {
        *v8 = -1073741801;
      }
      break;
    case 72LL:
LABEL_29:
      if ( a4 != 32 || (unsigned int)MaxCount < 8 )
        goto LABEL_79;
      *((_QWORD *)v8 + 1) = (unsigned int)MaxCount;
      *v8 = ProcExpQueryObjectName(v11, (unsigned int *)a3, (__int64)ProcessHandle, (__int64 *)v8 + 1);
      break;
    case 76LL:
LABEL_33:
      if ( a4 != 32 || (unsigned int)MaxCount < 8 )
        goto LABEL_79;
      *((_QWORD *)v8 + 1) = (unsigned int)MaxCount;
      *v8 = ProcExpQueryObjectType(v11, (unsigned int *)a3, (__int64)ProcessHandle, (_QWORD *)v8 + 1);
      break;
    case 80LL:
      if ( (_DWORD)MaxCount == 8 )
      {
        if ( gGetMaximumNonPagedPoolInBytes != NULL )
        {
          *ProcessHandle = (void *)gGetMaximumNonPagedPoolInBytes();
          *((_QWORD *)v8 + 1) = 8;
          *v8 = 0;
        }
        else
        {
          *v8 = -1073741503;
        }
      }
      else
      {
LABEL_79:
        *v8 = -1073741811;
      }
      break;
    default:
      *v8 = -1073741649;
      break;
  }
  result = (unsigned int)*v8;
  if ( (_DWORD)result == 259 )
  {
    *v8 = -1073741823;
    return 3221225473LL;
  }
  return result;
}


/* Original RVA 0x2320: sub_180002320 */
NTSTATUS __fastcall ProcExpQueryFileName(char a1, __int64 a2, struct _OBJECT_NAME_INFORMATION *a3, ULONG a4)
{
  __int64 v4; // r12
  UNICODE_STRING *p_Name; // rbx
  struct _OBJECT_NAME_INFORMATION *v9; // rdi
  void *v10; // rcx
  NTSTATUS result; // eax
  ULONG v12; // r8d
  int v13; // ebx
  struct _STRING DestinationString; // [rsp+20h] [rbp-38h] BYREF
  ULONG ReturnLength; // [rsp+68h] [rbp+10h] BYREF

  v4 = a4;
  p_Name = &a3->Name;
  v9 = a3;
  if ( (*(_BYTE *)(a2 + 80) & 2) == 0 || (unsigned __int16)(**(_WORD **)(a2 + 8) - 7) <= 1u )
    return -1073741823;
  if ( a1 != 0 )
    a3->Name.Length = 0;
  else
    LOBYTE(a3->Name.Length) = 0;
  __try {
  v10 = *(void **)(a2 + 8);
  if ( v10 != NULL )
  {
    result = ObQueryNameString(v10, a3, a4, &ReturnLength);
    if ( result < 0 )
      return result;
    if ( a1 != 0 )
    {
      ReturnLength = p_Name->Length >> 1;
      memmove(p_Name, p_Name->Buffer, 2LL * ReturnLength);
      *(&p_Name->Length + ReturnLength) = 0;
      p_Name = (UNICODE_STRING *)((char *)p_Name + 2 * ReturnLength);
    }
    else
    {
      result = RtlUnicodeStringToAnsiString(&DestinationString, p_Name, 1u);
      if ( result < 0 )
        return result;
      memmove(p_Name, DestinationString.Buffer, DestinationString.Length);
      *((_BYTE *)&p_Name->Length + DestinationString.Length) = 0;
      ReturnLength = DestinationString.Length;
      RtlFreeAnsiString(&DestinationString);
      v9 = (struct _OBJECT_NAME_INFORMATION *)((char *)p_Name + ReturnLength);
    }
  }
  v12 = *(unsigned __int16 *)(a2 + 88) >> 1;
  ReturnLength = v12;
  if ( v12 != 0 )
  {
    if ( a1 != 0 )
    {
      if ( (char *)a3 + v4 < (char *)(&p_Name->MaximumLength + v12) )
        return -1073741820;
      memmove(p_Name, *(const void **)(a2 + 96), 2LL * v12);
      *(&p_Name->Length + ReturnLength) = 0;
    }
    else
    {
      result = RtlUnicodeStringToAnsiString(&DestinationString, (PCUNICODE_STRING)(a2 + 88), 1u);
      v13 = result;
      if ( result < 0 )
        return result;
      if ( (char *)a3 + v4 >= (char *)&v9->Name.Length + DestinationString.Length + 1 )
      {
        memmove(v9, DestinationString.Buffer, DestinationString.Length);
        *((_BYTE *)&v9->Name.Length + DestinationString.Length) = 0;
      }
      else
      {
        v13 = -1073741820;
      }
      RtlFreeAnsiString(&DestinationString);
      if ( v13 < 0 )
        return v13;
    }
  }
  return 0;
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    return STATUS_ACCESS_VIOLATION;
  }
}

/* Original RVA 0x2500: sub_180002500 */
NTSTATUS __fastcall ProcExpQueryThreadContext(void *a1, _QWORD *a2, _QWORD *a3)
{
  NTSTATUS result; // eax
  PVOID v6; // rcx
  __int64 v7; // rdx
  PVOID Object; // [rsp+50h] [rbp+18h] BYREF
  union _LARGE_INTEGER Timeout; // [rsp+58h] [rbp+20h] BYREF

  *a3 = 0;
  Timeout.QuadPart = 0;
  result = ObReferenceObjectByHandle(a1, 0, *PsThreadType, 1, &Object, NULL);
  if ( result >= 0 )
  {
    if ( KeWaitForSingleObject(Object, Executive, 0, 0, &Timeout) != 0 )
    {
      v6 = Object;
      if ( gBuildNumber < 0x23F0 )
      {
        if ( gBuildNumber >= 0x1770 || gBuildNumber == 3790 )
          v7 = *((_QWORD *)Object + 7);
        else
          v7 = *((_QWORD *)Object + 9);
      }
      else
      {
        v7 = *((_QWORD *)Object + 11);
      }
      a2[1] = v7 + 64;
      *a2 = *(_QWORD *)(v7 + 56);
      a2[2] = *(_QWORD *)(v7 + 48);
      ObfDereferenceObject(v6);
      *a3 = 24;
      return 0;
    }
    else
    {
      return -1073741749;
    }
  }
  return result;
}


/* Original RVA 0x25E0: sub_1800025E0 */
NTSTATUS __fastcall ProcExpQueryMutantOwner(void *a1, void **a2, _QWORD *a3)
{
  NTSTATUS result; // eax
  PVOID v6; // rcx
  void *v7; // r10
  PVOID Object; // [rsp+68h] [rbp+20h] BYREF

  result = ObReferenceObjectByHandle(a1, 0, gMutantObjectType, 1, &Object, NULL);
  if ( result >= 0 )
  {
    v6 = Object;
    v7 = *((void **)Object + 5);
    if ( v7 != NULL )
    {
      ObOpenObjectByPointer(v7, 0x200u, NULL, 0x10000000u, *PsThreadType, 0, a2);
      v6 = Object;
    }
    else
    {
      *a2 = NULL;
    }
    ObfDereferenceObject(v6);
    *a3 = 8;
    return 0;
  }
  return result;
}


/* Original RVA 0x2B40: sub_180002B40 */
NTSTATUS __fastcall ProcExpQueryObjectType(char a1, unsigned int *a2, __int64 a3, _QWORD *a4)
{
  void *v4; // r14
  unsigned __int64 v5; // rbx
  NTSTATUS v9; // edi
  NTSTATUS result; // eax
  void *v11; // r12
  UNICODE_STRING *PoolWithTag; // rax
  UNICODE_STRING *v13; // r14
  unsigned int v14; // eax
  unsigned int Length; // ecx
  unsigned __int64 v16; // rbx
  __int64 v17; // rax
  ULONG v19; // eax
  const char *Buffer; // rdx
  __int64 v21; // rax
  struct _STRING DestinationString; // [rsp+30h] [rbp-48h] BYREF
  struct _KAPC_STATE ApcState; // [rsp+40h] [rbp-38h] BYREF
  ULONG ReturnLength; // [rsp+C8h] [rbp+50h] BYREF
  PEPROCESS Process; // [rsp+D0h] [rbp+58h] BYREF
  PVOID Object; // [rsp+D8h] [rbp+60h] BYREF

  v4 = *((void **)a2 + 3);
  v5 = *a4 - 8LL;
  Object = NULL;
  v9 = -1073741811;
  if ( *a2 >= 8 && gAcquireProcessExitSynchronization == NULL )
    goto LABEL_34;
  result = PsLookupProcessByProcessId((HANDLE)*a2, &Process);
  if ( result < 0 )
    return result;
  if ( gAcquireProcessExitSynchronization == NULL || (v9 = gAcquireProcessExitSynchronization(Process)) >= 0 )
  {
    KeStackAttachProcess(Process, &ApcState);
    v11 = (void *)((unsigned int)(ULONG_PTR)v4 | 0xFFFFFFFF80000000uLL);
    if ( *a2 >= 8 )
      v11 = v4;
    if ( ObReferenceObjectByHandle(v11, 0, NULL, *a2 >= 8, &Object, NULL) >= 0 && Object != *((PVOID *)a2 + 1) )
    {
      ObfDereferenceObject(Object);
      Object = NULL;
    }
    if ( gReleaseProcessExitSynchronization != NULL )
      gReleaseProcessExitSynchronization(Process);
    ObfDereferenceObject(Process);
    if ( Object == NULL )
    {
      KeUnstackDetachProcess(&ApcState);
      return -1073741790;
    }
    v9 = ZwQueryObject(v11, ObjectTypeInformation, NULL, 0, &ReturnLength);
    if ( v9 == -1073741820 )
    {
      PoolWithTag = (UNICODE_STRING *)ExAllocatePoolWithTag(PagedPool, ReturnLength, 0x58637250u);
      v13 = PoolWithTag;
      if ( PoolWithTag == NULL )
      {
        v9 = -1073741801;
        goto LABEL_33;
      }
      v9 = ZwQueryObject(v11, ObjectTypeInformation, PoolWithTag, ReturnLength, NULL);
      if ( v9 >= 0 )
      {
        if ( a1 != 0 )
        {
          v14 = v5 - 2;
          ReturnLength = v5 - 2;
          Length = v13->Length;
          if ( (int)v5 - 2 > Length )
          {
            ReturnLength = v13->Length;
            v14 = Length;
          }
          *(_WORD *)(a3 + 4) = 0;
          v16 = v14;
          memmove((void *)(a3 + 4), v13->Buffer, v14);
          v17 = -1;
          *(_WORD *)(a3 + 2 * (v16 >> 1) + 4) = 0;
          while ( *(_WORD *)(a3 + 2 * v17++ + 6) != 0 )
            ;
          *a4 = 2 * v17 + 10;
          ExFreePoolWithTag(v13, 0);
          goto LABEL_33;
        }
        v9 = RtlUnicodeStringToAnsiString(&DestinationString, v13, 1u);
        if ( v9 >= 0 )
        {
          v19 = DestinationString.Length;
          if ( v5 <= DestinationString.Length )
            v19 = v5 - 1;
          Buffer = DestinationString.Buffer;
          ReturnLength = v19;
          *(_BYTE *)(a3 + 4) = 0;
          strncpy((char *)(a3 + 4), Buffer, v19);
          *(_BYTE *)(ReturnLength + a3 + 4) = 0;
          RtlFreeAnsiString(&DestinationString);
          v21 = -1;
          do
            ++v21;
          while ( *(_BYTE *)(a3 + v21 + 4) != 0 );
          *a4 = v21 + 9;
        }
      }
      ExFreePoolWithTag(v13, 0);
    }
LABEL_33:
    ObfDereferenceObject(Object);
    KeUnstackDetachProcess(&ApcState);
    if ( v9 >= 0 )
      return v9;
LABEL_34:
    *a4 = 0;
    return v9;
  }
  ObfDereferenceObject(Process);
  return v9;
}


/* Original RVA 0x2FD0: sub_180002FD0 */
NTSTATUS __fastcall ProcExpDuplicateRequestHandle(__int64 a1, void **a2)
{
  return ProcExpDuplicateHandle(*(_DWORD *)a1, *(void **)(a1 + 24), 0x10000000u, a2);
}


/* Original RVA 0x2FF0: sub_180002FF0 */
NTSTATUS __fastcall ProcExpQueryExecuteFlags(void *a1, _DWORD *a2, _QWORD *a3)
{
  NTSTATUS result; // eax
  NTSTATUS v6; // ebx
  struct _KAPC_STATE ApcState; // [rsp+30h] [rbp-38h] BYREF
  ULONG ReturnLength; // [rsp+78h] [rbp+10h] BYREF
  PRKPROCESS PROCESS; // [rsp+88h] [rbp+20h] BYREF

  *a2 = -1;
  result = ObReferenceObjectByHandle(a1, 0x400u, *PsProcessType, 1, (PVOID *)&PROCESS, NULL);
  if ( result >= 0 )
  {
    KeStackAttachProcess(PROCESS, &ApcState);
    v6 = ZwQueryInformationProcess((HANDLE)0xFFFFFFFFFFFFFFFFLL, (PROCESSINFOCLASS)34, a2, 4u, &ReturnLength);
    KeUnstackDetachProcess(&ApcState);
    ObfDereferenceObject(PROCESS);
    result = v6;
    if ( v6 >= 0 )
      *a3 = ReturnLength;
  }
  return result;
}


/* Original RVA 0x30A0: sub_1800030A0 */
NTSTATUS __fastcall ProcExpReadThreadMemory(__int64 a1, _BYTE *a2, _QWORD *a3)
{
  unsigned int v6; // esi
  NTSTATUS result; // eax
  unsigned __int64 v8; // rbp
  unsigned __int64 v9; // r12
  unsigned int *i; // r14
  unsigned __int8 CurrentIrql; // r13
  char *v12; // rbx
  size_t v13; // r9
  unsigned int *v14; // rcx
  unsigned __int64 v15; // rdx
  size_t v16; // rdi
  char *v17; // rsi
  __int64 v18; // r15
  char v19; // al
  ULONG ReturnLength; // [rsp+70h] [rbp+8h] BYREF
  PVOID Object; // [rsp+88h] [rbp+20h] BYREF

  v6 = 0;
  result = ObReferenceObjectByHandle(*(HANDLE *)a1, 0, *PsThreadType, 1, &Object, NULL);
  if ( result < 0 )
  {
    *a3 = 0;
    return result;
  }
  v8 = *((_QWORD *)Object + 6);
  v9 = *((_QWORD *)Object + 5);
  ReturnLength = 1000;
  for ( i = (unsigned int *)ExAllocatePoolWithTag(NonPagedPool, 0x3E8u, 0x58637250u);
        i != NULL;
        i = (unsigned int *)ExAllocatePoolWithTag(NonPagedPool, ReturnLength, 0x58637250u) )
  {
    if ( ZwQuerySystemInformation((SYSTEM_INFORMATION_CLASS)11, i, ReturnLength, &ReturnLength) == 0 )
      break;
    ExFreePoolWithTag(i, 0);
    ReturnLength += 1000;
  }
  CurrentIrql = (UCHAR)__readcr8();
  __writecr8(2u);
  v12 = *(char **)(a1 + 8);
  v13 = *(unsigned int *)a3;
  if ( i != NULL && *i != 0 )
  {
    v14 = i + 8;
    do
    {
      v15 = *((_QWORD *)v14 - 1);
      if ( (unsigned __int64)v12 >= v15 && (unsigned __int64)v12 < v15 + *v14 )
      {
        v16 = v13;
        if ( (unsigned __int64)&v12[v13] >= v15 )
          goto LABEL_15;
      }
      ++v6;
      v14 += 74;
    }
    while ( v6 < *i );
  }
  v16 = v13;
  if ( (unsigned __int64)&v12[v13] <= v9 && (unsigned __int64)v12 >= v8 )
  {
LABEL_15:
    if ( (_DWORD)v13 != 0 )
    {
      v17 = v12;
      v18 = a2 - v12;
      do
      {
        if ( MmIsAddressValid(v17) != 0 )
          v19 = *v12;
        else
          v19 = 0;
        v12[v18] = v19;
        ++v17;
        ++v12;
        --v16;
      }
      while ( v16 != 0 );
    }
    goto LABEL_23;
  }
  memset(a2, 0, v13);
LABEL_23:
  __writecr8(CurrentIrql);
  ObfDereferenceObject(Object);
  if ( i != NULL )
    ExFreePoolWithTag(i, 0);
  return 0;
}


/* Original RVA 0x32F0: sub_1800032F0 */
NTSTATUS __fastcall ProcExpReopenHandle(void *a1, void **a2, ACCESS_MASK a3)
{
  NTSTATUS result; // eax
  NTSTATUS v6; // ebx
  PVOID Object; // [rsp+68h] [rbp+20h] BYREF

  result = ObReferenceObjectByHandle(a1, 0, NULL, 1, &Object, NULL);
  if ( result >= 0 )
  {
    v6 = ObOpenObjectByPointer(Object, 0x200u, NULL, a3, NULL, 0, a2);
    ObfDereferenceObject(Object);
    return v6;
  }
  return result;
}


/* Original RVA 0x3370: sub_180003370 */
PVOID ProcExpQueryModules()
{
  PVOID result; // rax
  void *v1; // rbx
  ULONG ReturnLength; // [rsp+30h] [rbp+8h] BYREF

  ReturnLength = 1000;
  result = ExAllocatePoolWithTag(NonPagedPool, 0x3E8u, 0x58637250u);
  v1 = result;
  if ( result != NULL )
  {
    while ( ZwQuerySystemInformation((SYSTEM_INFORMATION_CLASS)11, v1, ReturnLength, &ReturnLength) != 0 )
    {
      ExFreePoolWithTag(v1, 0);
      ReturnLength += 1000;
      result = ExAllocatePoolWithTag(NonPagedPool, ReturnLength, 0x58637250u);
      v1 = result;
      if ( result == NULL )
        return result;
    }
    return v1;
  }
  return result;
}

