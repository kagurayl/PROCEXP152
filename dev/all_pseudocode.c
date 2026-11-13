NTSTATUS __stdcall WdmlibIoValidateDeviceIoControlAccess(PIRP Irp, ULONG RequiredAccess)
{
  NTSTATUS result; // eax

  result = (int)qword_180005208;
  if ( qword_180005208 != nullptr )
    return qword_180005208(Irp, RequiredAccess);
  return result;
}


NTSTATUS __stdcall WdmlibRtlInitUnicodeStringEx(PUNICODE_STRING DestinationString, PCWSTR SourceString)
{
  __int64 v3; // rcx
  PCWSTR v4; // rdi
  bool v5; // zf
  unsigned __int64 v6; // rcx

  if ( SourceString != nullptr )
  {
    v3 = -1;
    v4 = SourceString;
    do
    {
      if ( v3 == 0 )
        break;
      v5 = *v4++ == 0;
      --v3;
    }
    while ( !v5 );
    v6 = -v3 - 2;
    if ( v6 > 0x7FFE )
      return -1073741562;
    DestinationString->Buffer = (PWSTR)SourceString;
    DestinationString->Length = 2 * v6;
    DestinationString->MaximumLength = 2 * v6 + 2;
  }
  else
  {
    DestinationString->Length = 0;
    DestinationString->MaximumLength = 0;
    DestinationString->Buffer = nullptr;
  }
  return 0;
}


void *__cdecl memmove(void *Dst, const void *Src, size_t MaxCount)
{
  void *v3; // r11
  bool v4; // cf
  unsigned __int64 v5; // rdx
  size_t v6; // r9
  size_t v7; // r9
  __int64 v9; // rax
  __int64 v10; // r10
  __int64 v11; // r10
  int k; // eax
  int m; // eax
  __int64 v14; // r10
  __int64 v15; // r10
  __int64 v16; // r9
  __int64 v17; // r10
  __int64 v18; // r10
  int *v19; // rcx
  size_t v20; // r9
  size_t v21; // r9
  __int64 v22; // rax
  __int64 v23; // r10
  __int64 v24; // r10
  int i; // eax
  int j; // eax
  __int64 v27; // r10
  __int64 v28; // r10
  __int64 v29; // r9
  __int64 v30; // r10
  __int64 v31; // r10
  _UNKNOWN *retaddr; // [rsp+0h] [rbp+0h] BYREF

  v3 = Dst;
  v4 = Src < Dst;
  v5 = (_BYTE *)Src - (_BYTE *)Dst;
  if ( v4 )
  {
    v19 = (int *)((char *)Dst + MaxCount);
    if ( MaxCount >= 8 )
    {
      if ( ((unsigned __int8)v19 & 7) != 0 )
      {
        if ( ((unsigned __int8)v19 & 1) != 0 )
        {
          v19 = (int *)((char *)v19 - 1);
          --MaxCount;
          *(_BYTE *)v19 = *((_BYTE *)v19 + v5);
        }
        if ( ((unsigned __int8)v19 & 2) != 0 )
        {
          v19 = (int *)((char *)v19 - 2);
          MaxCount -= 2LL;
          *(_WORD *)v19 = *(_WORD *)((char *)v19 + v5);
        }
        if ( ((unsigned __int8)v19 & 4) != 0 )
        {
          --v19;
          MaxCount -= 4LL;
          *v19 = *(int *)((char *)v19 + v5);
        }
      }
      while ( 1 )
      {
        v20 = MaxCount >> 5;
        if ( MaxCount >> 5 == 0 )
          break;
        if ( v20 < 0x2000 || v5 > 0xFFFFFFFFFFFFF000uLL )
        {
          do
          {
            v22 = *(_QWORD *)((char *)v19 + v5 - 8);
            v23 = *(_QWORD *)((char *)v19 + v5 - 16);
            v19 -= 8;
            *((_QWORD *)v19 + 3) = v22;
            *((_QWORD *)v19 + 2) = v23;
            v24 = *(_QWORD *)((char *)v19 + v5);
            --v20;
            *((_QWORD *)v19 + 1) = *(_QWORD *)((char *)v19 + v5 + 8);
            *(_QWORD *)v19 = v24;
          }
          while ( v20 != 0 );
          MaxCount &= 0x1Fu;
          break;
        }
        do
        {
          for ( i = 32; i != 0; --i )
          {
            v19 -= 32;
            _mm_prefetch((const char *)v19 + v5, 0);
            _mm_prefetch((const char *)v19 + v5 + 64, 0);
          }
          v19 += 1024;
          for ( j = 64; j != 0; --j )
          {
            v27 = *(_QWORD *)((char *)v19 + v5 - 16);
            _mm_stream_si32(v19 - 2, *(_QWORD *)((char *)v19 + v5 - 8));
            _mm_stream_si32(v19 - 4, v27);
            v28 = *(_QWORD *)((char *)v19 + v5 - 32);
            _mm_stream_si32(v19 - 6, *(_QWORD *)((char *)v19 + v5 - 24));
            _mm_stream_si32(v19 - 8, v28);
            v29 = *(_QWORD *)((char *)v19 + v5 - 40);
            v30 = *(_QWORD *)((char *)v19 + v5 - 48);
            v19 -= 16;
            _mm_stream_si32(v19 + 6, v29);
            _mm_stream_si32(v19 + 4, v30);
            v31 = *(_QWORD *)((char *)v19 + v5);
            _mm_stream_si32(v19 + 2, *(_QWORD *)((char *)v19 + v5 + 8));
            _mm_stream_si32(v19, v31);
          }
          MaxCount -= 4096LL;
        }
        while ( MaxCount >= 0x1000 );
        _InterlockedOr8((volatile signed __int8 *)&retaddr, 0);
      }
      v21 = MaxCount >> 3;
      if ( MaxCount >> 3 != 0 )
      {
        do
        {
          v19 -= 2;
          --v21;
          *(_QWORD *)v19 = *(_QWORD *)((char *)v19 + v5);
        }
        while ( v21 != 0 );
        MaxCount &= 7u;
      }
    }
    if ( MaxCount != 0 )
    {
      do
      {
        v19 = (int *)((char *)v19 - 1);
        --MaxCount;
        *(_BYTE *)v19 = *((_BYTE *)v19 + v5);
      }
      while ( MaxCount != 0 );
      return v3;
    }
    else
    {
      return v3;
    }
  }
  else
  {
    if ( MaxCount >= 8 )
    {
      if ( ((unsigned __int8)Dst & 7) != 0 )
      {
        if ( ((unsigned __int8)Dst & 1) != 0 )
        {
          --MaxCount;
          *(_BYTE *)Dst = *((_BYTE *)Dst + v5);
          Dst = (char *)Dst + 1;
        }
        if ( ((unsigned __int8)Dst & 2) != 0 )
        {
          MaxCount -= 2LL;
          *(_WORD *)Dst = *(_WORD *)((char *)Dst + v5);
          Dst = (char *)Dst + 2;
        }
        if ( ((unsigned __int8)Dst & 4) != 0 )
        {
          MaxCount -= 4LL;
          *(_DWORD *)Dst = *(_DWORD *)((char *)Dst + v5);
          Dst = (char *)Dst + 4;
        }
      }
      while ( 1 )
      {
        v6 = MaxCount >> 5;
        if ( MaxCount >> 5 == 0 )
          break;
        if ( v6 < 0x2000 || v5 < 0x1000 )
        {
          do
          {
            v9 = *(_QWORD *)((char *)Dst + v5);
            v10 = *(_QWORD *)((char *)Dst + v5 + 8);
            Dst = (char *)Dst + 32;
            *((_QWORD *)Dst - 4) = v9;
            *((_QWORD *)Dst - 3) = v10;
            v11 = *(_QWORD *)((char *)Dst + v5 - 8);
            --v6;
            *((_QWORD *)Dst - 2) = *(_QWORD *)((char *)Dst + v5 - 16);
            *((_QWORD *)Dst - 1) = v11;
          }
          while ( v6 != 0 );
          MaxCount &= 0x1Fu;
          break;
        }
        do
        {
          for ( k = 32; k != 0; --k )
          {
            _mm_prefetch((const char *)Dst + v5, 0);
            _mm_prefetch((const char *)Dst + v5 + 64, 0);
            Dst = (char *)Dst + 128;
          }
          Dst = (char *)Dst - 4096;
          for ( m = 64; m != 0; --m )
          {
            v14 = *(_QWORD *)((char *)Dst + v5 + 8);
            _mm_stream_si32((int *)Dst, *(_QWORD *)((char *)Dst + v5));
            _mm_stream_si32((int *)Dst + 2, v14);
            v15 = *(_QWORD *)((char *)Dst + v5 + 24);
            _mm_stream_si32((int *)Dst + 4, *(_QWORD *)((char *)Dst + v5 + 16));
            _mm_stream_si32((int *)Dst + 6, v15);
            v16 = *(_QWORD *)((char *)Dst + v5 + 32);
            v17 = *(_QWORD *)((char *)Dst + v5 + 40);
            Dst = (char *)Dst + 64;
            _mm_stream_si32((int *)Dst - 8, v16);
            _mm_stream_si32((int *)Dst - 6, v17);
            v18 = *(_QWORD *)((char *)Dst + v5 - 8);
            _mm_stream_si32((int *)Dst - 4, *(_QWORD *)((char *)Dst + v5 - 16));
            _mm_stream_si32((int *)Dst - 2, v18);
          }
          MaxCount -= 4096LL;
        }
        while ( MaxCount >= 0x1000 );
        _InterlockedOr8((volatile signed __int8 *)&retaddr, 0);
      }
      v7 = MaxCount >> 3;
      if ( MaxCount >> 3 != 0 )
      {
        do
        {
          *(_QWORD *)Dst = *(_QWORD *)((char *)Dst + v5);
          Dst = (char *)Dst + 8;
          --v7;
        }
        while ( v7 != 0 );
        MaxCount &= 7u;
      }
    }
    if ( MaxCount != 0 )
    {
      do
      {
        *(_BYTE *)Dst = *((_BYTE *)Dst + v5);
        Dst = (char *)Dst + 1;
        --MaxCount;
      }
      while ( MaxCount != 0 );
      return v3;
    }
    else
    {
      return v3;
    }
  }
}


// local variable allocation has failed, the output may be wrong!
void *__cdecl memset(void *Dst, int Val, size_t Size)
{
  void *result; // rax
  unsigned int v4; // ecx
  size_t v5; // r9
  unsigned __int64 v6; // r9
  size_t v7; // r9
  size_t i; // r9
  _UNKNOWN *retaddr; // [rsp+0h] [rbp+0h] BYREF

  result = Dst;
  if ( Size >= 8 )
  {
    *(_QWORD *)&Val = 0x101010101010101LL * (unsigned __int8)Val;
    if ( Size >= 0x40 )
    {
      v4 = -(int)Dst & 7;
      if ( v4 != 0 )
      {
        Size -= v4;
        *(_QWORD *)result = *(_QWORD *)&Val;
      }
      Dst = (char *)result + v4;
      v5 = Size;
      Size &= 0x3Fu;
      v6 = v5 >> 6;
      if ( v6 != 0 )
      {
        if ( v6 >= 0x1C00 )
        {
          do
          {
            _mm_stream_si32((int *)Dst, Val);
            _mm_stream_si32((int *)Dst + 2, Val);
            _mm_stream_si32((int *)Dst + 4, Val);
            Dst = (char *)Dst + 64;
            _mm_stream_si32((int *)Dst - 10, Val);
            _mm_stream_si32((int *)Dst - 8, Val);
            --v6;
            _mm_stream_si32((int *)Dst - 6, Val);
            _mm_stream_si32((int *)Dst - 4, Val);
            _mm_stream_si32((int *)Dst - 2, Val);
          }
          while ( v6 != 0 );
          _InterlockedOr8((volatile signed __int8 *)&retaddr, 0);
        }
        else
        {
          do
          {
            *(_QWORD *)Dst = *(_QWORD *)&Val;
            *((_QWORD *)Dst + 1) = *(_QWORD *)&Val;
            *((_QWORD *)Dst + 2) = *(_QWORD *)&Val;
            Dst = (char *)Dst + 64;
            *((_QWORD *)Dst - 5) = *(_QWORD *)&Val;
            *((_QWORD *)Dst - 4) = *(_QWORD *)&Val;
            --v6;
            *((_QWORD *)Dst - 3) = *(_QWORD *)&Val;
            *((_QWORD *)Dst - 2) = *(_QWORD *)&Val;
            *((_QWORD *)Dst - 1) = *(_QWORD *)&Val;
          }
          while ( v6 != 0 );
        }
      }
    }
    v7 = Size;
    Size &= 7u;
    for ( i = v7 >> 3; i != 0; --i )
    {
      *(_QWORD *)Dst = *(_QWORD *)&Val;
      Dst = (char *)Dst + 8;
    }
  }
  for ( ; Size != 0; --Size )
  {
    *(_BYTE *)Dst = Val;
    Dst = (char *)Dst + 1;
  }
  return result;
}


__int64 __fastcall _GSHandlerCheckCommon(__int64 a1, __int64 a2)
{
  __int64 result; // rax
  __int64 v3; // rcx

  result = *(_QWORD *)(a2 + 16);
  v3 = *(_QWORD *)(a2 + 8) + *(unsigned int *)(result + 8);
  if ( (*(_BYTE *)(v3 + 3) & 0xF) != 0 )
    return *(_BYTE *)(v3 + 3) & 0xF0;
  return result;
}


__int64 __fastcall _GSHandlerCheck(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
{
  _GSHandlerCheckCommon(a2, a4);
  return 1;
}


void __cdecl _security_check_cookie(uintptr_t StackCookie)
{
  __int64 v1; // rcx

  if ( StackCookie != BugCheckParameter2 )
ReportFailure:
    _report_gsfailure(StackCookie);
  v1 = __ROL8__(StackCookie, 16);
  if ( (_WORD)v1 != 0 )
  {
    StackCookie = __ROR8__(v1, 16);
    goto ReportFailure;
  }
}


void __cdecl __noreturn _report_gsfailure(uintptr_t StackCookie)
{
  KeBugCheckEx(0xF7u, StackCookie, BugCheckParameter2, BugCheckParameter3, 0);
}


NTSTATUS __stdcall DriverEntry(_DRIVER_OBJECT *DriverObject, PUNICODE_STRING RegistryPath)
{
  ULONG v3; // eax
  __int64 (__fastcall *SystemRoutineAddress)(POBJECT_TYPE *); // rax
  struct _OBJECT_TYPE *v6; // r14
  char *v7; // rcx
  int v8; // ebx
  NTSTATUS v9; // ebx
  int v10; // ebx
  struct _UNICODE_STRING DestinationString; // [rsp+50h] [rbp-B0h] BYREF
  PDEVICE_OBJECT DeviceObject; // [rsp+60h] [rbp-A0h] BYREF
  int v13; // [rsp+68h] [rbp-98h] BYREF
  PVOID P; // [rsp+70h] [rbp-90h]
  struct _UNICODE_STRING DeviceName; // [rsp+78h] [rbp-88h] BYREF
  WCHAR SourceString[20]; // [rsp+88h] [rbp-78h] BYREF
  WCHAR v17[24]; // [rsp+B0h] [rbp-50h] BYREF
  struct _UNICODE_STRING SymbolicLinkName; // [rsp+E0h] [rbp-20h] BYREF
  int v19; // [rsp+F0h] [rbp-10h] BYREF
  __int64 v20; // [rsp+F8h] [rbp-8h]
  int *v21; // [rsp+100h] [rbp+0h]
  int v22; // [rsp+108h] [rbp+8h]
  __int128 v23; // [rsp+110h] [rbp+10h]
  HANDLE Handle; // [rsp+150h] [rbp+50h] BYREF
  PVOID Object; // [rsp+158h] [rbp+58h] BYREF

  wcscpy(SourceString, L"\\Device\\PROCEXP152");
  wcscpy(v17, L"\\DosDevices\\PROCEXP152");
  PsGetVersion(nullptr, nullptr, &BuildNumber, nullptr);
  v3 = BuildNumber;
  if ( BuildNumber < 0xA28 )
    return -1073741637;
  if ( BuildNumber >= 0x1770 )
  {
    RtlInitUnicodeString(&DestinationString, L"PsAcquireProcessExitSynchronization");
    qword_1800051E0 = (__int64 (__fastcall *)(_QWORD))MmGetSystemRoutineAddress(&DestinationString);
    RtlInitUnicodeString(&DestinationString, L"PsReleaseProcessExitSynchronization");
    qword_1800051E8 = (__int64 (__fastcall *)(_QWORD))MmGetSystemRoutineAddress(&DestinationString);
    RtlInitUnicodeString(&DestinationString, L"MmGetMaximumNonPagedPoolInBytes");
    qword_1800051F0 = (__int64 (*)(void))MmGetSystemRoutineAddress(&DestinationString);
    v3 = BuildNumber;
  }
  if ( v3 < 0x1DB0 )
  {
    SystemRoutineAddress = (__int64 (__fastcall *)(POBJECT_TYPE *))qword_180005218;
  }
  else
  {
    RtlInitUnicodeString(&DestinationString, L"ObGetObjectType");
    SystemRoutineAddress = (__int64 (__fastcall *)(POBJECT_TYPE *))MmGetSystemRoutineAddress(&DestinationString);
    qword_180005218 = (__int64)SystemRoutineAddress;
  }
  if ( SystemRoutineAddress != nullptr )
  {
    v6 = (struct _OBJECT_TYPE *)SystemRoutineAddress(PsProcessType);
    SystemRoutineAddress = (__int64 (__fastcall *)(POBJECT_TYPE *))ExAllocatePoolWithTag(PagedPool, 0x26u, 0x544F4550u);
    P = SystemRoutineAddress;
    if ( SystemRoutineAddress != nullptr )
    {
      v13 = 2490406;
      *(_OWORD *)SystemRoutineAddress = xmmword_180004230;
      *((_QWORD *)SystemRoutineAddress + 2) = 0x73006500700079LL;
      *((_WORD *)SystemRoutineAddress + 12) = 92;
      v7 = (char *)P;
      *(_QWORD *)((char *)P + 26) = 0x6100740075004DLL;
      *(_DWORD *)(v7 + 34) = 7602286;
      v21 = &v13;
      v19 = 48;
      v20 = 0;
      v22 = 512;
      v23 = 0;
      v8 = ObOpenObjectByName(&v19, v6, 0, 0, 983041, 0, &Handle);
      ExFreePoolWithTag(P, 0);
      if ( v8 >= 0 )
      {
        v9 = ObReferenceObjectByHandle(Handle, 0xF0001u, v6, 0, &Object, nullptr);
        ZwClose(Handle);
        if ( v9 >= 0 )
        {
          ObfDereferenceObject(Object);
          SystemRoutineAddress = (__int64 (__fastcall *)(POBJECT_TYPE *))Object;
        }
        else
        {
          SystemRoutineAddress = nullptr;
        }
      }
      else
      {
        SystemRoutineAddress = nullptr;
      }
    }
  }
  ObjectType = (POBJECT_TYPE)SystemRoutineAddress;
  RtlInitUnicodeString(&DeviceName, SourceString);
  v10 = WdmlibIoCreateDeviceSecure(
          DriverObject,
          0,
          &DeviceName,
          0x8335u,
          0,
          0,
          &DefaultSDDLString,
          nullptr,
          &DeviceObject);
  if ( v10 < 0
    || (DeviceObject->Flags &= ~0x80u,
        RtlInitUnicodeString(&SymbolicLinkName, v17),
        v10 = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName),
        DriverObject->MajorFunction[14] = (PDRIVER_DISPATCH)sub_180002220,
        DriverObject->MajorFunction[2] = (PDRIVER_DISPATCH)sub_180002220,
        DriverObject->MajorFunction[0] = (PDRIVER_DISPATCH)sub_180002220,
        DriverObject->DriverUnload = (PDRIVER_UNLOAD)sub_180003280,
        v10 < 0) )
  {
    if ( DeviceObject != nullptr )
      IoDeleteDevice(DeviceObject);
  }
  return v10;
}


NTSTATUS __fastcall sub_180001950(unsigned int a1, void *a2, ACCESS_MASK a3, void **a4)
{
  NTSTATUS result; // eax
  ULONG Options; // eax
  NTSTATUS v9; // ebx
  HANDLE SourceProcessHandle; // [rsp+40h] [rbp-58h] BYREF
  unsigned __int128 v11; // [rsp+48h] [rbp-50h] BYREF
  struct _OBJECT_ATTRIBUTES v12; // [rsp+58h] [rbp-40h] BYREF

  v12.Length = 48;
  v12.RootDirectory = nullptr;
  v12.Attributes = 512;
  v12.ObjectName = nullptr;
  v11 = a1;
  *(_OWORD *)&v12.SecurityDescriptor = 0;
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


NTSTATUS __fastcall sub_180001A20(HANDLE *a1)
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
    v3 = ObReferenceObjectByHandle(a1[3], 0, nullptr, 1, &Object, &HandleInformation);
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
    KeUnstackDetachProcess(&ApcState);
    ObfDereferenceObject(Process);
    return v3;
  }
  return result;
}


__int64 __fastcall sub_180001AE0(
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
  char SystemInformation[24]; // [rsp+C0h] [rbp-40h] BYREF
  unsigned __int64 v34; // [rsp+D8h] [rbp-28h]
  unsigned int v35; // [rsp+E0h] [rbp-20h]

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
      *v8 = sub_180001A20(a3);
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
      v12 = ObReferenceObjectByHandle(*a3, 0, nullptr, 1, (PVOID *)&Object, nullptr);
      if ( v12 >= 0 )
      {
        v12 = ObOpenObjectByPointer(Object, 0x200u, nullptr, 0x10000000u, nullptr, 0, &Handle);
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
      *v8 = sub_180001950(*(_DWORD *)a3, a3[3], 0x10000000u, ProcessHandle);
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
      *v8 = sub_1800030A0(a3, ProcessHandle, v8 + 2);
      break;
    case 40LL:
      if ( a4 != 8 || (_DWORD)MaxCount != 24 )
        goto LABEL_79;
      v18 = *a3;
      Timeout.QuadPart = 0;
      v19 = ObReferenceObjectByHandle(v18, 0, (POBJECT_TYPE)PsThreadType, 1, (PVOID *)&Object, nullptr);
      if ( v19 < 0 )
        goto LABEL_63;
      if ( KeWaitForSingleObject(Object, Executive, 0, 0, &Timeout) != 0 )
      {
        v20 = Object;
        if ( BuildNumber < 0x23F0 )
        {
          if ( BuildNumber < 0x1770 )
          {
            if ( BuildNumber == 3790 )
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
      v22 = ObReferenceObjectByHandle(*a3, 0, ObjectType, 1, (PVOID *)&Object, nullptr);
      if ( v22 >= 0 )
      {
        v23 = Object;
        v24 = *((void **)Object + 5);
        if ( v24 != nullptr )
        {
          ObOpenObjectByPointer(v24, 0x200u, nullptr, 0x10000000u, (POBJECT_TYPE)PsThreadType, 0, ProcessHandle);
          v23 = Object;
        }
        else
        {
          *ProcessHandle = nullptr;
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
      v26 = ObReferenceObjectByHandle(v25, 0x400u, (POBJECT_TYPE)PsProcessType, 1, (PVOID *)&Object, nullptr);
      if ( v26 >= 0 )
      {
        KeStackAttachProcess((PRKPROCESS)Object, &ApcState);
        v26 = ZwQueryInformationProcess(
                (HANDLE)0xFFFFFFFFFFFFFFFFLL,
                ProcessExecuteFlags,
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
      ClientId.UniqueThread = nullptr;
      *(_OWORD *)&ObjectAttributes.SecurityDescriptor = 0;
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
      v34 = 0;
      if ( ZwQuerySystemInformation(SystemModuleInformation, SystemInformation, 0x130u, nullptr) < 0 )
      {
        v16 = v34 + v35;
        v17 = *a3;
        if ( (unsigned __int64)*a3 < v34
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
      *v8 = sub_180002680(v11, (unsigned int *)a3, (__int64)ProcessHandle, (__int64 *)v8 + 1);
      break;
    case 76LL:
LABEL_33:
      if ( a4 != 32 || (unsigned int)MaxCount < 8 )
        goto LABEL_79;
      *((_QWORD *)v8 + 1) = (unsigned int)MaxCount;
      *v8 = sub_180002B40(v11, (unsigned int *)a3, (__int64)ProcessHandle, (_QWORD *)v8 + 1);
      break;
    case 80LL:
      if ( (_DWORD)MaxCount == 8 )
      {
        if ( qword_1800051F0 != nullptr )
        {
          *ProcessHandle = (void *)qword_1800051F0(a1);
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


__int64 __fastcall sub_180002220(__int64 a1, IRP *a2)
{
  unsigned int v3; // ebx
  struct _IO_STACK_LOCATION *CurrentStackLocation; // rcx
  HANDLE *ProcessHandle; // r8
  KPROCESSOR_MODE PreviousMode; // al
  size_t MaxCount; // [rsp+28h] [rbp-70h]
  struct _PRIVILEGE_SET RequiredPrivileges; // [rsp+50h] [rbp-48h] BYREF
  struct _SECURITY_SUBJECT_CONTEXT SubjectContext; // [rsp+68h] [rbp-30h] BYREF

  v3 = 0;
  CurrentStackLocation = a2->Tail.Overlay.CurrentStackLocation;
  ProcessHandle = (HANDLE *)a2->AssociatedIrp.MasterIrp;
  a2->IoStatus.Information = 0;
  if ( CurrentStackLocation->MajorFunction != 0 )
  {
    if ( CurrentStackLocation->MajorFunction == 14 )
    {
      LOBYTE(a1) = 1;
      LODWORD(MaxCount) = CurrentStackLocation->Parameters.Read.Length;
      v3 = sub_180001AE0(
             (__int64)CurrentStackLocation->FileObject,
             a1,
             ProcessHandle,
             CurrentStackLocation->Parameters.Create.Options,
             ProcessHandle,
             MaxCount,
             CurrentStackLocation->Parameters.Read.ByteOffset.LowPart,
             (NTSTATUS *)&a2->IoStatus.0);
    }
    else
    {
      v3 = -1073741808;
    }
  }
  else
  {
    RequiredPrivileges.Privilege[0].Luid = (LUID)20LL;
    RequiredPrivileges.PrivilegeCount = 1;
    RequiredPrivileges.Control = 1;
    RequiredPrivileges.Privilege[0].Attributes = 0;
    SeCaptureSubjectContext(&SubjectContext);
    PreviousMode = ExGetPreviousMode();
    if ( SePrivilegeCheck(&RequiredPrivileges, &SubjectContext, PreviousMode) == 0 )
      v3 = -1073741790;
    SeReleaseSubjectContext(&SubjectContext);
  }
  a2->IoStatus.Status = v3;
  IofCompleteRequest(a2, 0);
  return v3;
}


NTSTATUS __fastcall sub_180002320(char a1, __int64 a2, struct _OBJECT_NAME_INFORMATION *a3, ULONG a4)
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
  v10 = *(void **)(a2 + 8);
  if ( v10 != nullptr )
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
}


NTSTATUS __fastcall sub_180002500(void *a1, _QWORD *a2, _QWORD *a3)
{
  NTSTATUS result; // eax
  PVOID v6; // rcx
  __int64 v7; // rdx
  PVOID Object; // [rsp+50h] [rbp+18h] BYREF
  union _LARGE_INTEGER Timeout; // [rsp+58h] [rbp+20h] BYREF

  *a3 = 0;
  Timeout.QuadPart = 0;
  result = ObReferenceObjectByHandle(a1, 0, (POBJECT_TYPE)PsThreadType, 1, &Object, nullptr);
  if ( result >= 0 )
  {
    if ( KeWaitForSingleObject(Object, Executive, 0, 0, &Timeout) != 0 )
    {
      v6 = Object;
      if ( BuildNumber < 0x23F0 )
      {
        if ( BuildNumber >= 0x1770 || BuildNumber == 3790 )
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


NTSTATUS __fastcall sub_1800025E0(void *a1, void **a2, _QWORD *a3)
{
  NTSTATUS result; // eax
  PVOID v6; // rcx
  void *v7; // r10
  PVOID Object; // [rsp+68h] [rbp+20h] BYREF

  result = ObReferenceObjectByHandle(a1, 0, ObjectType, 1, &Object, nullptr);
  if ( result >= 0 )
  {
    v6 = Object;
    v7 = *((void **)Object + 5);
    if ( v7 != nullptr )
    {
      ObOpenObjectByPointer(v7, 0x200u, nullptr, 0x10000000u, (POBJECT_TYPE)PsThreadType, 0, a2);
      v6 = Object;
    }
    else
    {
      *a2 = nullptr;
    }
    ObfDereferenceObject(v6);
    *a3 = 8;
    return 0;
  }
  return result;
}


NTSTATUS __fastcall sub_180002680(char a1, unsigned int *a2, __int64 a3, __int64 *a4)
{
  NTSTATUS v7; // r14d
  __int64 v8; // rax
  bool v9; // bl
  void *v10; // r13
  void *v11; // rax
  __int64 v12; // rbx
  NTSTATUS result; // eax
  POBJECT_TYPE *v14; // r8
  _WORD *v15; // rcx
  void *v16; // rcx
  char v17; // al
  ULONG v18; // edi
  __int64 v19; // rbx
  unsigned __int16 v20; // ax
  ULONG v21; // eax
  __int64 v22; // rdi
  __int64 v23; // rdi
  ULONG Length[2]; // [rsp+48h] [rbp-D0h]
  void *ProcessHandle; // [rsp+50h] [rbp-C8h] BYREF
  void *TargetHandle; // [rsp+58h] [rbp-C0h] BYREF
  struct _STRING DestinationString; // [rsp+60h] [rbp-B8h] BYREF
  struct _CLIENT_ID ClientId; // [rsp+70h] [rbp-A8h] BYREF
  struct _OBJECT_ATTRIBUTES v29; // [rsp+80h] [rbp-98h] BYREF
  struct _KAPC_STATE ApcState; // [rsp+B0h] [rbp-68h] BYREF
  ULONG ReturnLength; // [rsp+120h] [rbp+8h] BYREF
  PVOID Object; // [rsp+128h] [rbp+10h] BYREF
  PEPROCESS Process; // [rsp+130h] [rbp+18h] BYREF
  __int64 *v34; // [rsp+138h] [rbp+20h]

  v34 = a4;
  v7 = 0;
  v8 = *a4;
  *(_QWORD *)Length = *a4 - 8;
  Object = nullptr;
  v9 = false;
  *a4 = v8 - *(_QWORD *)Length;
  v10 = *((void **)a2 + 3);
  v11 = (void *)*a2;
  if ( (unsigned int)v11 >= 8 && qword_1800051E0 == nullptr )
  {
    ClientId.UniqueThread = nullptr;
    ClientId.UniqueProcess = v11;
    v29.Length = 48;
    v29.RootDirectory = nullptr;
    v29.Attributes = 512;
    v29.ObjectName = nullptr;
    *(_OWORD *)&v29.SecurityDescriptor = 0;
    if ( ZwOpenProcess(&ProcessHandle, 0x40u, &v29, &ClientId) < 0 )
      return -1073741790;
    v12 = -1;
    if ( ZwDuplicateObject(ProcessHandle, v10, (HANDLE)0xFFFFFFFFFFFFFFFFLL, &TargetHandle, 0, 0, 2u) < 0 )
    {
      ZwClose(ProcessHandle);
      return -1073741790;
    }
    ZwClose(ProcessHandle);
    if ( *((_BYTE *)a2 + 16) != 0 )
      v14 = IoFileObjectType;
    else
      v14 = nullptr;
    if ( ObReferenceObjectByHandle(TargetHandle, 0, (POBJECT_TYPE)v14, 1, &Object, nullptr) < 0 )
    {
      ObCloseHandle(TargetHandle, 1);
      return -1073741790;
    }
    ObCloseHandle(TargetHandle, 1);
    v15 = Object;
    if ( Object != *((PVOID *)a2 + 1) )
    {
      ObfDereferenceObject(Object);
      return -1073741790;
    }
LABEL_28:
    *(_DWORD *)a3 = 0;
    if ( a1 != 0 )
      *(_WORD *)(a3 + 4) = 0;
    else
      *(_BYTE *)(a3 + 4) = 0;
    if ( *((_BYTE *)a2 + 16) != 0 && *v15 == 5 )
    {
      if ( v15[1] == 216 )
      {
        if ( *((_BYTE *)v15 + 77) != 0 )
          *(_DWORD *)a3 |= 1u;
        if ( *((_BYTE *)v15 + 78) != 0 )
          *(_DWORD *)a3 |= 2u;
        if ( *((_BYTE *)v15 + 79) != 0 )
          *(_DWORD *)a3 |= 4u;
        v18 = Length[0];
        v7 = sub_180002320(a1, (__int64)v15, (struct _OBJECT_NAME_INFORMATION *)(a3 + 4), Length[0]);
        if ( v7 >= 0 )
        {
          if ( a1 != 0 )
          {
            do
              ++v12;
            while ( *(_WORD *)(a3 + 2 * v12 + 4) != 0 );
            v19 = 2 * v12 + 10;
            goto LABEL_62;
          }
          do
            ++v12;
          while ( *(_BYTE *)(a3 + v12 + 4) != 0 );
          goto LABEL_61;
        }
        v15 = Object;
        v17 = 1;
LABEL_50:
        if ( v17 == 0 )
        {
LABEL_64:
          ObfDereferenceObject(v15);
          return v7;
        }
        v7 = ObQueryNameString(v15, (POBJECT_NAME_INFORMATION)(a3 + 4), v18, &ReturnLength);
        if ( v7 < 0 || (v20 = *(_WORD *)(a3 + 4)) == 0 )
        {
LABEL_63:
          v15 = Object;
          goto LABEL_64;
        }
        if ( a1 != 0 )
        {
          v21 = v20 >> 1;
          v22 = v21;
          ReturnLength = v21;
          memmove((void *)(a3 + 4), *(const void **)(a3 + 12), 2LL * v21);
          *(_WORD *)(a3 + 2 * v22 + 4) = 0;
        }
        else
        {
          v7 = RtlUnicodeStringToAnsiString(&DestinationString, (PCUNICODE_STRING)(a3 + 4), 1u);
          if ( v7 >= 0 )
          {
            v23 = DestinationString.Length;
            memmove((void *)(a3 + 4), DestinationString.Buffer, DestinationString.Length);
            *(_BYTE *)(v23 + a3 + 4) = 0;
            RtlFreeAnsiString(&DestinationString);
          }
        }
        if ( a1 != 0 )
        {
          do
            ++v12;
          while ( *(_WORD *)(a3 + 2 * v12 + 4) != 0 );
          v19 = 2 * v12 + 10;
          goto LABEL_62;
        }
        do
          ++v12;
        while ( *(_BYTE *)(a3 + v12 + 4) != 0 );
LABEL_61:
        v19 = v12 + 9;
LABEL_62:
        *v34 = v19;
        goto LABEL_63;
      }
      v17 = 0;
      v7 = -1073741811;
    }
    else
    {
      v17 = 1;
    }
    v18 = Length[0];
    goto LABEL_50;
  }
  result = PsLookupProcessByProcessId((HANDLE)*a2, &Process);
  v7 = result;
  if ( result < 0 )
    return result;
  if ( qword_1800051E0 != nullptr )
  {
    v7 = qword_1800051E0(Process);
    v9 = v7 >= 0;
  }
  if ( v7 >= 0 )
  {
    KeStackAttachProcess(Process, &ApcState);
    v16 = (void *)((unsigned int)v10 | 0xFFFFFFFF80000000uLL);
    if ( *a2 >= 8 )
      v16 = v10;
    v7 = ObReferenceObjectByHandle(v16, 0, nullptr, *a2 >= 8, &Object, nullptr);
    if ( v7 >= 0 && Object != *((PVOID *)a2 + 1) )
    {
      ObfDereferenceObject(Object);
      v7 = -1073741811;
    }
    KeUnstackDetachProcess(&ApcState);
    if ( v9 && qword_1800051E8 != nullptr )
      qword_1800051E8(Process);
  }
  ObfDereferenceObject(Process);
  if ( v7 >= 0 )
  {
    v12 = -1;
    v15 = Object;
    goto LABEL_28;
  }
  return v7;
}


NTSTATUS __fastcall sub_180002B40(char a1, unsigned int *a2, __int64 a3, _QWORD *a4)
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
  Object = nullptr;
  v9 = -1073741811;
  if ( *a2 >= 8 && qword_1800051E0 == nullptr )
    goto LABEL_34;
  result = PsLookupProcessByProcessId((HANDLE)*a2, &Process);
  if ( result < 0 )
    return result;
  if ( qword_1800051E0 == nullptr || (v9 = qword_1800051E0(Process)) >= 0 )
  {
    KeStackAttachProcess(Process, &ApcState);
    v11 = (void *)((unsigned int)v4 | 0xFFFFFFFF80000000uLL);
    if ( *a2 >= 8 )
      v11 = v4;
    if ( ObReferenceObjectByHandle(v11, 0, nullptr, *a2 >= 8, &Object, nullptr) >= 0 && Object != *((PVOID *)a2 + 1) )
    {
      ObfDereferenceObject(Object);
      Object = nullptr;
    }
    if ( qword_1800051E8 != nullptr )
      qword_1800051E8(Process);
    ObfDereferenceObject(Process);
    if ( Object == nullptr )
    {
      KeUnstackDetachProcess(&ApcState);
      return -1073741790;
    }
    v9 = ZwQueryObject(v11, ObjectTypeInformation, nullptr, 0, &ReturnLength);
    if ( v9 == -1073741820 )
    {
      PoolWithTag = (UNICODE_STRING *)ExAllocatePoolWithTag(PagedPool, ReturnLength, 0x58637250u);
      v13 = PoolWithTag;
      if ( PoolWithTag == nullptr )
      {
        v9 = -1073741801;
        goto LABEL_33;
      }
      v9 = ZwQueryObject(v11, ObjectTypeInformation, PoolWithTag, ReturnLength, nullptr);
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


PVOID __fastcall sub_180002E40(_WORD *Src)
{
  struct _OBJECT_TYPE *v2; // rbp
  __int64 v3; // rbx
  unsigned int v4; // ebx
  _OWORD *PoolWithTag; // rax
  int v6; // ebx
  NTSTATUS v7; // ebx
  _WORD v9[4]; // [rsp+40h] [rbp-58h] BYREF
  PVOID P; // [rsp+48h] [rbp-50h]
  int v11; // [rsp+50h] [rbp-48h] BYREF
  __int64 v12; // [rsp+58h] [rbp-40h]
  _WORD *v13; // [rsp+60h] [rbp-38h]
  int v14; // [rsp+68h] [rbp-30h]
  __int128 v15; // [rsp+70h] [rbp-28h]
  HANDLE Handle; // [rsp+A8h] [rbp+10h] BYREF
  PVOID Object; // [rsp+B0h] [rbp+18h] BYREF

  if ( qword_180005218 == nullptr )
    return nullptr;
  v2 = (struct _OBJECT_TYPE *)qword_180005218(PsProcessType);
  v3 = -1;
  do
    ++v3;
  while ( Src[v3] != 0 );
  v4 = 2 * v3;
  PoolWithTag = ExAllocatePoolWithTag(PagedPool, v4 + 26, 0x544F4550u);
  P = PoolWithTag;
  if ( PoolWithTag == nullptr )
    return nullptr;
  v9[0] = v4 + 26;
  v9[1] = v4 + 26;
  *PoolWithTag = xmmword_180004230;
  *((_QWORD *)PoolWithTag + 2) = 0x73006500700079LL;
  *((_WORD *)PoolWithTag + 12) = 92;
  memmove((char *)P + 26, Src, v4);
  v11 = 48;
  v13 = v9;
  v14 = 512;
  v12 = 0;
  v15 = 0;
  v6 = ObOpenObjectByName(&v11, v2, 0, 0, 983041, 0, &Handle);
  ExFreePoolWithTag(P, 0);
  if ( v6 < 0 )
    return nullptr;
  v7 = ObReferenceObjectByHandle(Handle, 0xF0001u, v2, 0, &Object, nullptr);
  ZwClose(Handle);
  if ( v7 < 0 )
    return nullptr;
  ObfDereferenceObject(Object);
  return Object;
}


NTSTATUS __fastcall sub_180002FD0(__int64 a1, void **a2)
{
  return sub_180001950(*(_DWORD *)a1, *(void **)(a1 + 24), 0x10000000u, a2);
}


NTSTATUS __fastcall sub_180002FF0(void *a1, _DWORD *a2, _QWORD *a3)
{
  NTSTATUS result; // eax
  NTSTATUS v6; // ebx
  struct _KAPC_STATE ApcState; // [rsp+30h] [rbp-38h] BYREF
  ULONG ReturnLength; // [rsp+78h] [rbp+10h] BYREF
  PRKPROCESS PROCESS; // [rsp+88h] [rbp+20h] BYREF

  *a2 = -1;
  result = ObReferenceObjectByHandle(a1, 0x400u, (POBJECT_TYPE)PsProcessType, 1, (PVOID *)&PROCESS, nullptr);
  if ( result >= 0 )
  {
    KeStackAttachProcess(PROCESS, &ApcState);
    v6 = ZwQueryInformationProcess((HANDLE)0xFFFFFFFFFFFFFFFFLL, ProcessExecuteFlags, a2, 4u, &ReturnLength);
    KeUnstackDetachProcess(&ApcState);
    ObfDereferenceObject(PROCESS);
    result = v6;
    if ( v6 >= 0 )
      *a3 = ReturnLength;
  }
  return result;
}


NTSTATUS __fastcall sub_1800030A0(__int64 a1, _BYTE *a2, _QWORD *a3)
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
  result = ObReferenceObjectByHandle(*(HANDLE *)a1, 0, (POBJECT_TYPE)PsThreadType, 1, &Object, nullptr);
  if ( result < 0 )
  {
    *a3 = 0;
    return result;
  }
  v8 = *((_QWORD *)Object + 6);
  v9 = *((_QWORD *)Object + 5);
  ReturnLength = 1000;
  for ( i = (unsigned int *)ExAllocatePoolWithTag(NonPagedPool, 0x3E8u, 0x58637250u);
        i != nullptr;
        i = (unsigned int *)ExAllocatePoolWithTag(NonPagedPool, ReturnLength, 0x58637250u) )
  {
    if ( ZwQuerySystemInformation(SystemModuleInformation, i, ReturnLength, &ReturnLength) == 0 )
      break;
    ExFreePoolWithTag(i, 0);
    ReturnLength += 1000;
  }
  CurrentIrql = KeGetCurrentIrql();
  __writecr8(2u);
  v12 = *(char **)(a1 + 8);
  v13 = *(unsigned int *)a3;
  if ( i != nullptr && *i != 0 )
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
  if ( i != nullptr )
    ExFreePoolWithTag(i, 0);
  return 0;
}


void __fastcall sub_180003280(__int64 a1)
{
  struct _UNICODE_STRING DestinationString; // [rsp+20h] [rbp-48h] BYREF
  WCHAR SourceString[28]; // [rsp+30h] [rbp-38h] BYREF

  wcscpy(SourceString, L"\\DosDevices\\PROCEXP152");
  RtlInitUnicodeString(&DestinationString, SourceString);
  IoDeleteSymbolicLink(&DestinationString);
  IoDeleteDevice(*(PDEVICE_OBJECT *)(a1 + 8));
}


NTSTATUS __fastcall sub_1800032F0(void *a1, void **a2, ACCESS_MASK a3)
{
  NTSTATUS result; // eax
  NTSTATUS v6; // ebx
  PVOID Object; // [rsp+68h] [rbp+20h] BYREF

  result = ObReferenceObjectByHandle(a1, 0, nullptr, 1, &Object, nullptr);
  if ( result >= 0 )
  {
    v6 = ObOpenObjectByPointer(Object, 0x200u, nullptr, a3, nullptr, 0, a2);
    ObfDereferenceObject(Object);
    return v6;
  }
  return result;
}


PVOID sub_180003370()
{
  PVOID result; // rax
  void *v1; // rbx
  ULONG ReturnLength; // [rsp+30h] [rbp+8h] BYREF

  ReturnLength = 1000;
  result = ExAllocatePoolWithTag(NonPagedPool, 0x3E8u, 0x58637250u);
  v1 = result;
  if ( result != nullptr )
  {
    while ( ZwQuerySystemInformation(SystemModuleInformation, v1, ReturnLength, &ReturnLength) != 0 )
    {
      ExFreePoolWithTag(v1, 0);
      ReturnLength += 1000;
      result = ExAllocatePoolWithTag(NonPagedPool, ReturnLength, 0x58637250u);
      v1 = result;
      if ( result == nullptr )
        return result;
    }
    return v1;
  }
  return result;
}


// attributes: thunk
char *__cdecl strncpy(char *Dest, const char *Source, size_t Count)
{
  return __imp_strncpy(Dest, Source, Count);
}


// attributes: thunk
void __stdcall RtlInitUnicodeString(PUNICODE_STRING DestinationString, PCWSTR SourceString)
{
  __imp_RtlInitUnicodeString(DestinationString, SourceString);
}


// attributes: thunk
NTSTATUS __stdcall RtlUnicodeStringToAnsiString(
        PANSI_STRING DestinationString,
        PCUNICODE_STRING SourceString,
        BOOLEAN AllocateDestinationString)
{
  return __imp_RtlUnicodeStringToAnsiString(DestinationString, SourceString, AllocateDestinationString);
}


// attributes: thunk
void __stdcall RtlFreeAnsiString(PANSI_STRING AnsiString)
{
  __imp_RtlFreeAnsiString(AnsiString);
}


// attributes: thunk
NTSTATUS __stdcall KeWaitForSingleObject(
        PVOID Object,
        KWAIT_REASON WaitReason,
        KPROCESSOR_MODE WaitMode,
        BOOLEAN Alertable,
        PLARGE_INTEGER Timeout)
{
  return __imp_KeWaitForSingleObject(Object, WaitReason, WaitMode, Alertable, Timeout);
}


// attributes: thunk
PVOID __stdcall ExAllocatePoolWithTag(POOL_TYPE PoolType, SIZE_T NumberOfBytes, ULONG Tag)
{
  return __imp_ExAllocatePoolWithTag(PoolType, NumberOfBytes, Tag);
}


// attributes: thunk
void __stdcall ExFreePoolWithTag(PVOID P, ULONG Tag)
{
  __imp_ExFreePoolWithTag(P, Tag);
}


// attributes: thunk
KPROCESSOR_MODE ExGetPreviousMode(void)
{
  return __imp_ExGetPreviousMode();
}


// attributes: thunk
PVOID __stdcall MmGetSystemRoutineAddress(PUNICODE_STRING SystemRoutineName)
{
  return __imp_MmGetSystemRoutineAddress(SystemRoutineName);
}


// attributes: thunk
void __stdcall SeCaptureSubjectContext(PSECURITY_SUBJECT_CONTEXT SubjectContext)
{
  __imp_SeCaptureSubjectContext(SubjectContext);
}


// attributes: thunk
void __stdcall SeReleaseSubjectContext(PSECURITY_SUBJECT_CONTEXT SubjectContext)
{
  __imp_SeReleaseSubjectContext(SubjectContext);
}


// attributes: thunk
void __stdcall IofCompleteRequest(PIRP Irp, CCHAR PriorityBoost)
{
  __imp_IofCompleteRequest(Irp, PriorityBoost);
}


// attributes: thunk
NTSTATUS __stdcall IoCreateSymbolicLink(PUNICODE_STRING SymbolicLinkName, PUNICODE_STRING DeviceName)
{
  return __imp_IoCreateSymbolicLink(SymbolicLinkName, DeviceName);
}


// attributes: thunk
void __stdcall IoDeleteDevice(PDEVICE_OBJECT DeviceObject)
{
  __imp_IoDeleteDevice(DeviceObject);
}


// attributes: thunk
NTSTATUS __stdcall IoDeleteSymbolicLink(PUNICODE_STRING SymbolicLinkName)
{
  return __imp_IoDeleteSymbolicLink(SymbolicLinkName);
}


// attributes: thunk
NTSTATUS __stdcall ObReferenceObjectByHandle(
        HANDLE Handle,
        ACCESS_MASK DesiredAccess,
        POBJECT_TYPE ObjectType,
        KPROCESSOR_MODE AccessMode,
        PVOID *Object,
        POBJECT_HANDLE_INFORMATION HandleInformation)
{
  return __imp_ObReferenceObjectByHandle(Handle, DesiredAccess, ObjectType, AccessMode, Object, HandleInformation);
}


// attributes: thunk
LONG_PTR __stdcall ObfDereferenceObject(PVOID Object)
{
  return __imp_ObfDereferenceObject(Object);
}


// attributes: thunk
NTSTATUS __stdcall ZwClose(HANDLE Handle)
{
  return __imp_ZwClose(Handle);
}


// attributes: thunk
BOOLEAN __stdcall MmIsAddressValid(PVOID VirtualAddress)
{
  return __imp_MmIsAddressValid(VirtualAddress);
}


// attributes: thunk
BOOLEAN __stdcall PsGetVersion(
        PULONG MajorVersion,
        PULONG MinorVersion,
        PULONG BuildNumber,
        PUNICODE_STRING CSDVersion)
{
  return __imp_PsGetVersion(MajorVersion, MinorVersion, BuildNumber, CSDVersion);
}


// attributes: thunk
NTSTATUS __stdcall ZwOpenProcess(
        PHANDLE ProcessHandle,
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        PCLIENT_ID ClientId)
{
  return __imp_ZwOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
}


// attributes: thunk
void __stdcall KeStackAttachProcess(PRKPROCESS PROCESS, PRKAPC_STATE ApcState)
{
  __imp_KeStackAttachProcess(PROCESS, ApcState);
}


// attributes: thunk
void __stdcall KeUnstackDetachProcess(PRKAPC_STATE ApcState)
{
  __imp_KeUnstackDetachProcess(ApcState);
}


// attributes: thunk
BOOLEAN __stdcall SePrivilegeCheck(
        PPRIVILEGE_SET RequiredPrivileges,
        PSECURITY_SUBJECT_CONTEXT SubjectSecurityContext,
        KPROCESSOR_MODE AccessMode)
{
  return __imp_SePrivilegeCheck(RequiredPrivileges, SubjectSecurityContext, AccessMode);
}


// attributes: thunk
NTSTATUS __stdcall PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process)
{
  return __imp_PsLookupProcessByProcessId(ProcessId, Process);
}


// attributes: thunk
NTSTATUS __stdcall ObOpenObjectByPointer(
        PVOID Object,
        ULONG HandleAttributes,
        PACCESS_STATE PassedAccessState,
        ACCESS_MASK DesiredAccess,
        POBJECT_TYPE ObjectType,
        KPROCESSOR_MODE AccessMode,
        PHANDLE Handle)
{
  return __imp_ObOpenObjectByPointer(
           Object,
           HandleAttributes,
           PassedAccessState,
           DesiredAccess,
           ObjectType,
           AccessMode,
           Handle);
}


// attributes: thunk
NTSTATUS __stdcall ObQueryNameString(
        PVOID Object,
        POBJECT_NAME_INFORMATION ObjectNameInfo,
        ULONG Length,
        PULONG ReturnLength)
{
  return __imp_ObQueryNameString(Object, ObjectNameInfo, Length, ReturnLength);
}


// attributes: thunk
NTSTATUS __stdcall ZwQueryObject(
        HANDLE Handle,
        OBJECT_INFORMATION_CLASS ObjectInformationClass,
        PVOID ObjectInformation,
        ULONG ObjectInformationLength,
        PULONG ReturnLength)
{
  return __imp_ZwQueryObject(Handle, ObjectInformationClass, ObjectInformation, ObjectInformationLength, ReturnLength);
}


// attributes: thunk
NTSTATUS __stdcall ZwDuplicateObject(
        HANDLE SourceProcessHandle,
        HANDLE SourceHandle,
        HANDLE TargetProcessHandle,
        PHANDLE TargetHandle,
        ACCESS_MASK DesiredAccess,
        ULONG HandleAttributes,
        ULONG Options)
{
  return __imp_ZwDuplicateObject(
           SourceProcessHandle,
           SourceHandle,
           TargetProcessHandle,
           TargetHandle,
           DesiredAccess,
           HandleAttributes,
           Options);
}


// attributes: thunk
NTSTATUS __stdcall ZwOpenProcessToken(HANDLE ProcessHandle, ACCESS_MASK DesiredAccess, PHANDLE TokenHandle)
{
  return __imp_ZwOpenProcessToken(ProcessHandle, DesiredAccess, TokenHandle);
}


// attributes: thunk
NTSTATUS __stdcall ZwQueryInformationProcess(
        HANDLE ProcessHandle,
        PROCESSINFOCLASS ProcessInformationClass,
        PVOID ProcessInformation,
        ULONG ProcessInformationLength,
        PULONG ReturnLength)
{
  return __imp_ZwQueryInformationProcess(
           ProcessHandle,
           ProcessInformationClass,
           ProcessInformation,
           ProcessInformationLength,
           ReturnLength);
}


// attributes: thunk
NTSTATUS __stdcall ZwQuerySystemInformation(
        SYSTEM_INFORMATION_CLASS SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength)
{
  return __imp_ZwQuerySystemInformation(
           SystemInformationClass,
           SystemInformation,
           SystemInformationLength,
           ReturnLength);
}


// attributes: thunk
NTSTATUS __stdcall ObCloseHandle(HANDLE Handle, KPROCESSOR_MODE PreviousMode)
{
  return __imp_ObCloseHandle(Handle, PreviousMode);
}


// attributes: thunk
__int64 __fastcall ObOpenObjectByName(__int64 a1, __int64 a2, __int64 a3, __int64 a4, int a5, __int64 a6, __int64 a7)
{
  return __imp_ObOpenObjectByName(a1, a2, a3, a4, a5, a6, a7);
}


// attributes: thunk
EXCEPTION_DISPOSITION __cdecl _C_specific_handler(
        struct _EXCEPTION_RECORD *ExceptionRecord,
        void *EstablisherFrame,
        struct _CONTEXT *ContextRecord,
        struct _DISPATCHER_CONTEXT *DispatcherContext)
{
  return __C_specific_handler(ExceptionRecord, EstablisherFrame, ContextRecord, DispatcherContext);
}


// attributes: thunk
NTSTATUS __stdcall IoCreateDevice(
        PDRIVER_OBJECT DriverObject,
        ULONG DeviceExtensionSize,
        PUNICODE_STRING DeviceName,
        ULONG DeviceType,
        ULONG DeviceCharacteristics,
        BOOLEAN Exclusive,
        PDEVICE_OBJECT *DeviceObject)
{
  return __imp_IoCreateDevice(
           DriverObject,
           DeviceExtensionSize,
           DeviceName,
           DeviceType,
           DeviceCharacteristics,
           Exclusive,
           DeviceObject);
}


// attributes: thunk
NTSTATUS __stdcall ZwSetSecurityObject(
        HANDLE Handle,
        SECURITY_INFORMATION SecurityInformation,
        PSECURITY_DESCRIPTOR SecurityDescriptor)
{
  return __imp_ZwSetSecurityObject(Handle, SecurityInformation, SecurityDescriptor);
}


// attributes: thunk
int snwprintf(wchar_t *Dest, size_t Count, const wchar_t *Format, ...)
{
  return _snwprintf(Dest, Count, Format);
}


// attributes: thunk
ULONG __stdcall RtlLengthSecurityDescriptor(PSECURITY_DESCRIPTOR SecurityDescriptor)
{
  return __imp_RtlLengthSecurityDescriptor(SecurityDescriptor);
}


// attributes: thunk
__int64 SeCaptureSecurityDescriptor()
{
  return __imp_SeCaptureSecurityDescriptor();
}


// attributes: thunk
NTSTATUS __stdcall RtlCreateSecurityDescriptor(PSECURITY_DESCRIPTOR SecurityDescriptor, ULONG Revision)
{
  return __imp_RtlCreateSecurityDescriptor(SecurityDescriptor, Revision);
}


// attributes: thunk
NTSTATUS __stdcall RtlSetDaclSecurityDescriptor(
        PSECURITY_DESCRIPTOR SecurityDescriptor,
        BOOLEAN DaclPresent,
        PACL Dacl,
        BOOLEAN DaclDefaulted)
{
  return __imp_RtlSetDaclSecurityDescriptor(SecurityDescriptor, DaclPresent, Dacl, DaclDefaulted);
}


// attributes: thunk
NTSTATUS __stdcall RtlAbsoluteToSelfRelativeSD(
        PSECURITY_DESCRIPTOR AbsoluteSecurityDescriptor,
        PSECURITY_DESCRIPTOR SelfRelativeSecurityDescriptor,
        PULONG BufferLength)
{
  return __imp_RtlAbsoluteToSelfRelativeSD(AbsoluteSecurityDescriptor, SelfRelativeSecurityDescriptor, BufferLength);
}


// attributes: thunk
BOOLEAN __stdcall IoIsWdmVersionAvailable(UCHAR MajorVersion, UCHAR MinorVersion)
{
  return __imp_IoIsWdmVersionAvailable(MajorVersion, MinorVersion);
}


// attributes: thunk
wchar_t *__cdecl wcschr(const wchar_t *Str, wchar_t Ch)
{
  return __imp_wcschr(Str, Ch);
}


// attributes: thunk
int __cdecl wcsnicmp(const wchar_t *Str1, const wchar_t *Str2, size_t MaxCount)
{
  return _wcsnicmp(Str1, Str2, MaxCount);
}


// attributes: thunk
ULONG __stdcall RtlLengthSid(PSID Sid)
{
  return __imp_RtlLengthSid(Sid);
}


// attributes: thunk
NTSTATUS __stdcall RtlAddAccessAllowedAce(PACL Acl, ULONG AceRevision, ACCESS_MASK AccessMask, PSID Sid)
{
  return __imp_RtlAddAccessAllowedAce(Acl, AceRevision, AccessMask, Sid);
}


// attributes: thunk
NTSTATUS __stdcall RtlGetSaclSecurityDescriptor(
        PSECURITY_DESCRIPTOR SecurityDescriptor,
        PBOOLEAN SaclPresent,
        PACL *Sacl,
        PBOOLEAN SaclDefaulted)
{
  return __imp_RtlGetSaclSecurityDescriptor(SecurityDescriptor, SaclPresent, Sacl, SaclDefaulted);
}


// attributes: thunk
NTSTATUS __stdcall RtlGetDaclSecurityDescriptor(
        PSECURITY_DESCRIPTOR SecurityDescriptor,
        PBOOLEAN DaclPresent,
        PACL *Dacl,
        PBOOLEAN DaclDefaulted)
{
  return __imp_RtlGetDaclSecurityDescriptor(SecurityDescriptor, DaclPresent, Dacl, DaclDefaulted);
}


// attributes: thunk
NTSTATUS __stdcall RtlGetGroupSecurityDescriptor(
        PSECURITY_DESCRIPTOR SecurityDescriptor,
        PSID *Group,
        PBOOLEAN GroupDefaulted)
{
  return __imp_RtlGetGroupSecurityDescriptor(SecurityDescriptor, Group, GroupDefaulted);
}


// attributes: thunk
NTSTATUS __stdcall RtlGetOwnerSecurityDescriptor(
        PSECURITY_DESCRIPTOR SecurityDescriptor,
        PSID *Owner,
        PBOOLEAN OwnerDefaulted)
{
  return __imp_RtlGetOwnerSecurityDescriptor(SecurityDescriptor, Owner, OwnerDefaulted);
}


// attributes: thunk
NTSTATUS __stdcall ZwOpenKey(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes)
{
  return __imp_ZwOpenKey(KeyHandle, DesiredAccess, ObjectAttributes);
}


// attributes: thunk
NTSTATUS __stdcall ZwCreateKey(
        PHANDLE KeyHandle,
        ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        ULONG TitleIndex,
        PUNICODE_STRING Class,
        ULONG CreateOptions,
        PULONG Disposition)
{
  return __imp_ZwCreateKey(KeyHandle, DesiredAccess, ObjectAttributes, TitleIndex, Class, CreateOptions, Disposition);
}


// attributes: thunk
NTSTATUS __stdcall ZwQueryValueKey(
        HANDLE KeyHandle,
        PUNICODE_STRING ValueName,
        KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
        PVOID KeyValueInformation,
        ULONG Length,
        PULONG ResultLength)
{
  return __imp_ZwQueryValueKey(
           KeyHandle,
           ValueName,
           KeyValueInformationClass,
           KeyValueInformation,
           Length,
           ResultLength);
}


// attributes: thunk
NTSTATUS __stdcall ZwSetValueKey(
        HANDLE KeyHandle,
        PUNICODE_STRING ValueName,
        ULONG TitleIndex,
        ULONG Type,
        PVOID Data,
        ULONG DataSize)
{
  return __imp_ZwSetValueKey(KeyHandle, ValueName, TitleIndex, Type, Data, DataSize);
}


// attributes: thunk
void __stdcall RtlFreeUnicodeString_0(PUNICODE_STRING UnicodeString)
{
  RtlFreeUnicodeString(UnicodeString);
}


// attributes: thunk
void __stdcall __noreturn KeBugCheckEx(
        ULONG BugCheckCode,
        ULONG_PTR BugCheckParameter1,
        ULONG_PTR BugCheckParameter2,
        ULONG_PTR BugCheckParameter3,
        ULONG_PTR BugCheckParameter4)
{
  __imp_KeBugCheckEx(BugCheckCode, BugCheckParameter1, BugCheckParameter2, BugCheckParameter3, BugCheckParameter4);
}


PVOID WdmlibInit()
{
  __int64 (__usercall *SystemRoutineAddress)@<rax>(PDRIVER_OBJECT@<rcx>, ULONG@<edx>, PUNICODE_STRING@<r8>, int, char, __int64, __int64, PDEVICE_OBJECT); // rax
  PVOID result; // rax
  struct _UNICODE_STRING DestinationString; // [rsp+20h] [rbp-18h] BYREF

  RtlInitUnicodeString(&DestinationString, L"IoCreateDeviceSecure");
  SystemRoutineAddress = (__int64 (__usercall *)@<rax>(PDRIVER_OBJECT@<rcx>, ULONG@<edx>, PUNICODE_STRING@<r8>, int, char, __int64, __int64, PDEVICE_OBJECT))MmGetSystemRoutineAddress(&DestinationString);
  if ( SystemRoutineAddress == nullptr )
    SystemRoutineAddress = IoDevObjCreateDeviceSecure;
  qword_180005200 = (__int64)SystemRoutineAddress;
  RtlInitUnicodeString(&DestinationString, L"IoValidateDeviceIoControlAccess");
  result = MmGetSystemRoutineAddress(&DestinationString);
  byte_1800051F8 = 1;
  qword_180005208 = (NTSTATUS (__fastcall *)(_QWORD, _QWORD))result;
  return result;
}


NTSTATUS __stdcall WdmlibIoCreateDeviceSecure(
        PDRIVER_OBJECT DriverObject,
        ULONG DeviceExtensionSize,
        PUNICODE_STRING DeviceName,
        ULONG DeviceType,
        ULONG DeviceCharacteristics,
        BOOLEAN Exclusive,
        PCUNICODE_STRING DefaultSDDLString,
        LPCGUID DeviceClassGuid,
        PDEVICE_OBJECT *DeviceObject)
{
  if ( byte_1800051F8 == 0 )
    WdmlibInit();
  return qword_180005200(
           DriverObject,
           DeviceExtensionSize,
           DeviceName,
           DeviceType,
           DeviceCharacteristics,
           Exclusive,
           DefaultSDDLString,
           DeviceClassGuid,
           DeviceObject);
}


NTSTATUS __fastcall IopDevObjApplyPostCreationSettings(PVOID Object, __int64 a2)
{
  NTSTATUS result; // eax
  NTSTATUS v5; // ebx
  HANDLE Handle; // [rsp+40h] [rbp-18h] BYREF
  SECURITY_INFORMATION SecurityInformation; // [rsp+70h] [rbp+18h]
  ACCESS_MASK DesiredAccess; // [rsp+78h] [rbp+20h] BYREF

  if ( (*(_BYTE *)a2 & 2) == 0 )
    return 0;
  result = SeUtilSecurityInfoFromSecurityDescriptor(*(PSECURITY_DESCRIPTOR *)(a2 + 8));
  if ( result >= 0 )
  {
    SeSetSecurityAccessMask(SecurityInformation, &DesiredAccess);
    result = ObOpenObjectByPointer(Object, 0x200u, nullptr, DesiredAccess, IoDeviceObjectType, 0, &Handle);
    if ( result >= 0 )
    {
      v5 = ZwSetSecurityObject(Handle, SecurityInformation, *(PSECURITY_DESCRIPTOR *)(a2 + 8));
      ZwClose(Handle);
      return v5;
    }
  }
  return result;
}


__int64 __fastcall IoDevObjCreateDeviceSecure(
        PDRIVER_OBJECT DriverObject,
        ULONG DeviceExtensionSize,
        PUNICODE_STRING DeviceName,
        ULONG a4,
        ULONG a5,
        BOOLEAN a6,
        __int64 a7,
        __int64 a8,
        PDEVICE_OBJECT DeviceObject)
{
  PDEVICE_OBJECT v9; // rsi
  ULONG DeviceCharacteristics; // ebp
  __int64 result; // rax
  char v16; // al
  NTSTATUS updated; // ebx
  BOOLEAN Exclusive; // cl
  ULONG v19[5]; // [rsp+40h] [rbp-58h] BYREF
  BOOLEAN v20; // [rsp+54h] [rbp-44h]
  _BYTE v21[24]; // [rsp+58h] [rbp-40h] BYREF
  __int64 v22; // [rsp+B0h] [rbp+18h] BYREF

  v9 = DeviceObject;
  DeviceObject = nullptr;
  DeviceCharacteristics = a5;
  *(_QWORD *)&v9->Type = 0;
  if ( DeviceName == nullptr && (DeviceCharacteristics & 0x80u) == 0 )
    return 3221225485LL;
  if ( a8 == 0 )
  {
    PpRegStateInitEmptyCreationSettings(v19);
LABEL_8:
    v16 = v19[0];
    if ( (v19[0] & 2) == 0 )
    {
      updated = SeSddlSecurityDescriptorFromSDDL(a7, 1, &v22);
      if ( updated < 0
        || (PpRegStateLoadSecurityDescriptor(v22, v19), a8 != 0)
        && (PpRegStateInitEmptyCreationSettings(v21),
            PpRegStateLoadSecurityDescriptor(v22, v21),
            (updated = PpRegStateUpdateStackCreationSettings(a8, v21)) < 0) )
      {
$Exit$30769:
        PpRegStateFreeStackCreationSettings(v19);
        return (unsigned int)updated;
      }
      v16 = v19[0];
    }
    Exclusive = a6;
    if ( (v16 & 1) != 0 )
      a4 = v19[1];
    if ( (v16 & 4) != 0 )
      DeviceCharacteristics = v19[4];
    if ( (v16 & 8) != 0 )
      Exclusive = v20;
    updated = IoCreateDevice(
                DriverObject,
                DeviceExtensionSize,
                DeviceName,
                a4,
                DeviceCharacteristics,
                Exclusive,
                &DeviceObject);
    if ( updated >= 0 )
    {
      updated = IopDevObjApplyPostCreationSettings(DeviceObject, (__int64)v19);
      if ( updated >= 0 )
        *(_QWORD *)&v9->Type = DeviceObject;
      else
        IoDeleteDevice(DeviceObject);
    }
    goto $Exit$30769;
  }
  result = PpRegStateReadCreateClassCreationSettings(a8, DriverObject, v19);
  if ( (int)result >= 0 )
    goto LABEL_8;
  return result;
}


void __fastcall PpRegStateLoadSecurityDescriptor(__int64 a1, __int64 a2)
{
  *(_DWORD *)a2 = 2;
  *(_QWORD *)(a2 + 8) = a1;
}


void __fastcall PpRegStateFreeStackCreationSettings(__int64 a1)
{
  if ( (*(_BYTE *)a1 & 2) != 0 )
    ExFreePoolWithTag(*(PVOID *)(a1 + 8), 0);
}


__int64 __fastcall PiRegStateOpenClassKey(unsigned int *a1, unsigned int a2, int a3, int *a4, _QWORD *a5)
{
  __int64 result; // rax
  int WstrKey; // eax
  int v11; // ebx
  int v12; // [rsp+70h] [rbp-B8h] BYREF
  HANDLE Handle; // [rsp+78h] [rbp-B0h] BYREF
  _QWORD v14[2]; // [rsp+80h] [rbp-A8h] BYREF
  wchar_t Dest[40]; // [rsp+90h] [rbp-98h] BYREF

  *a5 = 0;
  if ( a4 != nullptr )
    *a4 = 0;
  result = CmRegUtilOpenExistingWstrKey(
             0,
             L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Class",
             131097,
             &Handle);
  if ( (int)result >= 0 )
  {
    snwprintf(
      Dest,
      0x27u,
      L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
      *a1,
      *((unsigned __int16 *)a1 + 2),
      *((unsigned __int16 *)a1 + 3),
      *((unsigned __int8 *)a1 + 8),
      *((unsigned __int8 *)a1 + 9),
      *((unsigned __int8 *)a1 + 10),
      *((unsigned __int8 *)a1 + 11),
      *((unsigned __int8 *)a1 + 12),
      *((unsigned __int8 *)a1 + 13),
      *((unsigned __int8 *)a1 + 14),
      *((unsigned __int8 *)a1 + 15));
    Dest[38] = 0;
    if ( a3 != 0 )
    {
      WstrKey = CmRegUtilCreateWstrKey((_DWORD)Handle, (unsigned int)Dest, a2, 0, 0, (__int64)&v12, (__int64)v14);
    }
    else
    {
      WstrKey = CmRegUtilOpenExistingWstrKey(Handle, Dest, a2, v14);
      v12 = 2;
    }
    v11 = WstrKey;
    ZwClose(Handle);
    if ( v11 >= 0 )
    {
      *a5 = v14[0];
      if ( a4 != nullptr )
        *a4 = v12;
    }
    return (unsigned int)v11;
  }
  return result;
}


__int64 __fastcall PpRegStateInitEmptyCreationSettings(__int64 a1)
{
  *(_DWORD *)a1 = 0;
  *(_QWORD *)(a1 + 8) = 0;
  *(_DWORD *)(a1 + 4) = 0;
  *(_DWORD *)(a1 + 16) = 0;
  *(_DWORD *)(a1 + 20) = 0;
  return 0;
}


__int64 __fastcall PiRegStateReadStackCreationSettingsFromKey(void *a1, __int64 a2)
{
  int FullBuffer; // eax
  __int64 v5; // r9
  int v6; // ebx
  PVOID v7; // rax
  int Dword; // eax
  int v9; // eax
  int v10; // eax
  void *v11; // rcx
  PVOID P[9]; // [rsp+30h] [rbp-48h] BYREF
  char v14; // [rsp+88h] [rbp+10h]
  PVOID SecurityDescriptor; // [rsp+98h] [rbp+20h] BYREF

  *(_DWORD *)a2 = 0;
  *(_QWORD *)(a2 + 8) = 0;
  *(_DWORD *)(a2 + 4) = 0;
  *(_DWORD *)(a2 + 16) = 0;
  *(_DWORD *)(a2 + 20) = 0;
  P[0] = nullptr;
  FullBuffer = CmRegUtilWstrValueGetFullBuffer(a1, (__int64)P);
  v6 = FullBuffer;
  if ( FullBuffer < 0 )
  {
    if ( FullBuffer == -1073741772 )
    {
      SecurityDescriptor = nullptr;
      v6 = 0;
    }
  }
  else
  {
    LOBYTE(v5) = 1;
    v6 = SeCaptureSecurityDescriptor((char *)P[0] + *((unsigned int *)P[0] + 2), 0, 1, v5, &SecurityDescriptor);
  }
  if ( P[0] != nullptr )
    ExFreePoolWithTag(P[0], 0);
  if ( v6 < 0 )
    goto $ErrorExit$30792;
  if ( SecurityDescriptor != nullptr )
  {
    v6 = SeUtilSecurityInfoFromSecurityDescriptor(SecurityDescriptor);
    if ( v6 < 0 )
      goto $ErrorExit$30792;
    if ( v14 != 0 )
    {
      ExFreePoolWithTag(SecurityDescriptor, 0);
    }
    else
    {
      v7 = SecurityDescriptor;
      *(_DWORD *)a2 |= 2u;
      *(_QWORD *)(a2 + 8) = v7;
    }
  }
  Dword = CmRegUtilWstrValueGetDword(a1, L"DeviceType", 0, a2 + 4);
  v6 = Dword;
  if ( Dword < 0 )
  {
    if ( Dword != -1073741772 )
      goto $ErrorExit$30792;
  }
  else
  {
    *(_DWORD *)a2 |= 1u;
  }
  v9 = CmRegUtilWstrValueGetDword(a1, L"DeviceCharacteristics", 0, a2 + 16);
  v6 = v9;
  if ( v9 < 0 )
  {
    if ( v9 != -1073741772 )
      goto $ErrorExit$30792;
  }
  else
  {
    *(_DWORD *)a2 |= 4u;
  }
  v10 = CmRegUtilWstrValueGetDword(a1, L"Exclusive", 0, a2 + 20);
  v6 = v10;
  if ( v10 >= 0 )
  {
    *(_DWORD *)a2 |= 8u;
    return (unsigned int)v6;
  }
  if ( v10 == -1073741772 )
    return 0;
$ErrorExit$30792:
  v11 = *(void **)(a2 + 8);
  if ( v11 != nullptr )
    ExFreePoolWithTag(v11, 0);
  *(_DWORD *)a2 = 0;
  *(_QWORD *)(a2 + 8) = 0;
  *(_DWORD *)(a2 + 4) = 0;
  *(_DWORD *)(a2 + 16) = 0;
  *(_DWORD *)(a2 + 20) = 0;
  return (unsigned int)v6;
}


__int64 __fastcall PpRegStateReadCreateClassCreationSettings(unsigned int *a1, __int64 a2, __int64 a3)
{
  __int64 result; // rax
  int v6; // ebx
  int StackCreationSettingsFromKey; // eax
  void *v8; // rcx
  HANDLE v9; // rdi
  void *v10; // [rsp+30h] [rbp-18h] BYREF
  _BYTE v11[8]; // [rsp+60h] [rbp+18h] BYREF
  HANDLE Handle; // [rsp+68h] [rbp+20h] BYREF

  *(_DWORD *)a3 = 0;
  *(_QWORD *)(a3 + 8) = 0;
  *(_DWORD *)(a3 + 4) = 0;
  *(_DWORD *)(a3 + 16) = 0;
  *(_DWORD *)(a3 + 20) = 0;
  Handle = nullptr;
  v10 = nullptr;
  result = PiRegStateOpenClassKey(a1, 0xF003Fu, 1, (int *)v11, &Handle);
  if ( (int)result >= 0 )
  {
    if ( *(_DWORD *)v11 == 2 )
    {
      v6 = CmRegUtilOpenExistingWstrKey(Handle, L"Properties", 131097, &v10);
      ZwClose(Handle);
      if ( v6 < 0 )
      {
        if ( v6 == -1073741772 )
          return 0;
        return (unsigned int)v6;
      }
      StackCreationSettingsFromKey = PiRegStateReadStackCreationSettingsFromKey(v10, a3);
      v8 = v10;
      v6 = StackCreationSettingsFromKey;
    }
    else
    {
      v9 = Handle;
      if ( *(_QWORD *)(a2 + 48) == -24 )
      {
        v6 = -1073741670;
      }
      else
      {
        v6 = CmRegUtilWstrValueSetUcString(Handle);
        if ( v6 >= 0 )
        {
          wcscpy((wchar_t *)v11, L"1");
          v6 = CmRegUtilWstrValueSetWstrString(v9, L"NoDisplayClass", (PCWSTR)v11);
          if ( v6 >= 0 )
            v6 = CmRegUtilWstrValueSetWstrString(v9, L"NoUseClass", (PCWSTR)v11);
        }
      }
      v8 = v9;
    }
    ZwClose(v8);
    return (unsigned int)v6;
  }
  return result;
}


__int64 __fastcall PpRegStateUpdateStackCreationSettings(unsigned int *a1, __int64 a2)
{
  __int64 result; // rax
  __int64 v4; // r9
  int v5; // eax
  void *v6; // rbx
  int WstrKey; // ebx
  ULONG v8; // eax
  HANDLE KeyHandle[3]; // [rsp+40h] [rbp-18h] BYREF
  PVOID P; // [rsp+70h] [rbp+18h] BYREF
  HANDLE Handle; // [rsp+78h] [rbp+20h] BYREF

  result = PiRegStateOpenClassKey(a1, 0xF003Fu, 0, nullptr, &Handle);
  if ( (int)result >= 0 )
  {
    v5 = dword_180005210;
    v6 = &unk_180005000;
    if ( dword_180005210 == 0 )
    {
      LOBYTE(v4) = 1;
      if ( (int)SeCaptureSecurityDescriptor(&unk_180005000, 0, 1, v4, &P) < 0 )
      {
        v5 = 2;
        dword_180005210 = 2;
      }
      else
      {
        dword_180005210 = 1;
        ExFreePoolWithTag(P, 0);
        v5 = dword_180005210;
      }
    }
    if ( v5 != 1 )
      v6 = nullptr;
    P = v6;
    WstrKey = CmRegUtilCreateWstrKey(
                (_DWORD)Handle,
                (unsigned int)L"Properties",
                983103,
                0,
                (__int64)v6,
                0,
                (__int64)KeyHandle);
    ZwClose(Handle);
    if ( WstrKey >= 0 )
    {
      v8 = RtlLengthSecurityDescriptor(*(PSECURITY_DESCRIPTOR *)(a2 + 8));
      WstrKey = CmRegUtilWstrValueSetFullBuffer(KeyHandle[0], v8);
      ZwClose(KeyHandle[0]);
    }
    return (unsigned int)WstrKey;
  }
  return result;
}


__int64 __fastcall SepSddlParseWideStringUlong(unsigned __int16 *a1, unsigned __int16 **a2, unsigned int *a3)
{
  unsigned __int16 *v5; // r10
  unsigned __int16 *v6; // r9
  bool v7; // cf
  int v8; // edx
  unsigned int v9; // r8d
  int v10; // ecx
  unsigned __int16 v11; // ax

  *a3 = 0;
  *a2 = a1;
  v5 = a1;
  v6 = a1;
  v7 = *a1 < 0x30u;
  if ( *a1 != 48 )
  {
LABEL_6:
    if ( !v7 )
    {
      v8 = 10;
      if ( *a1 <= 0x39u )
        goto LABEL_9;
    }
    goto LABEL_8;
  }
  if ( a1[1] != 120 && a1[1] != 88 )
  {
    v7 = *a1 < 0x30u;
    goto LABEL_6;
  }
  v5 = a1 + 2;
  v6 = a1 + 2;
LABEL_8:
  v8 = 16;
LABEL_9:
  v9 = 0;
  while ( 1 )
  {
    v11 = *v6;
    if ( *v6 == 0 )
      break;
    if ( v11 < 0x30u || v11 > 0x39u )
    {
      if ( v8 != 16 )
        break;
      if ( v11 < 0x41u || v11 > 0x46u )
      {
        if ( v11 < 0x61u || v11 > 0x66u )
          break;
        v10 = v11 - 87;
      }
      else
      {
        v10 = v11 - 55;
      }
    }
    else
    {
      v10 = v11 - 48;
    }
    if ( v10 + v8 * v9 < v9 )
      return 0;
    v9 = v10 + v8 * v9;
    ++v6;
  }
  if ( v6 != v5 )
  {
    *a2 = v6;
    *a3 = v9;
    return 1;
  }
  return 0;
}


NTSTATUS __fastcall SepSddlAddAceToAcl(
        const void **a1,
        _DWORD *a2,
        __int64 a3,
        __int64 a4,
        ACCESS_MASK AccessMask,
        int a6,
        PSID Sid)
{
  struct _ACL *v7; // rdi
  size_t v8; // r12
  ULONG v11; // ebx
  int v12; // ebp
  unsigned int v13; // ebx
  struct _ACL *PoolWithTag; // rax
  struct _ACL *v15; // r13

  v7 = (struct _ACL *)*a1;
  v8 = (unsigned int)*a2;
  v11 = RtlLengthSid(Sid) + 8;
  v12 = v11 + v8;
  if ( v11 + (unsigned int)v8 > v7->AclSize )
  {
    v13 = v8 + a6 * v11;
    PoolWithTag = (struct _ACL *)ExAllocatePoolWithTag(PagedPool, v13, 0x6C416553u);
    v15 = PoolWithTag;
    if ( PoolWithTag == nullptr )
      return -1073741670;
    memset(PoolWithTag, 0, v13);
    memmove(v15, *a1, v8);
    v15->AclSize = v13;
    ExFreePoolWithTag(v7, 0);
    *a1 = v15;
    v7 = v15;
  }
  *a2 = v12;
  return RtlAddAccessAllowedAce(v7, 2u, AccessMask, Sid);
}


__int64 __fastcall SepSddlLookupAccessMaskInTable(wchar_t *Str1, _DWORD *a2, _QWORD *a3)
{
  unsigned int v6; // ebx
  wchar_t **i; // rdi

  v6 = 0;
  for ( i = &off_180005150; wcsnicmp(Str1, *i, *((unsigned int *)i + 2)) != 0; i += 2 )
  {
    if ( ++v6 >= 8 )
    {
      *a2 = 0;
      *a3 = Str1;
      return 0;
    }
  }
  *a2 = *((_DWORD *)&off_180005150 + 4 * v6 + 3);
  *a3 = &Str1[*((unsigned int *)&off_180005150 + 4 * v6 + 2)];
  return 1;
}


__int64 __fastcall SepSddlGetSidForString(wchar_t *Str1, _QWORD *a2, _QWORD *a3)
{
  __int64 v6; // rdi
  unsigned int *i; // rsi
  __int64 v9; // rdi

  v6 = 0;
  for ( i = (unsigned int *)&unk_180005044; wcsnicmp(Str1, (const wchar_t *)&unk_180005030 + 12 * v6 + 6, *i) != 0; i += 6 )
  {
    v6 = (unsigned int)(v6 + 1);
    if ( (unsigned int)v6 >= 0xC )
    {
      *a2 = 0;
      return 3221225587LL;
    }
  }
  v9 = 3 * v6;
  *a3 = &Str1[*((unsigned int *)&unk_180005030 + 2 * v9 + 5)];
  if ( *((_DWORD *)&unk_180005030 + 2 * v9 + 2) != 1 || IoIsWdmVersionAvailable(1u, 0x20u) != 0 )
    *a2 = *(LUID *)((char *)&SeExports->SeCreateTokenPrivilege + *((_QWORD *)&unk_180005030 + v9));
  else
    *a2 = 0;
  return 0;
}


__int64 __fastcall SepSddlGetAclForString(wchar_t *Str1, const void **a2, wchar_t **a3)
{
  wchar_t *v5; // rbx
  unsigned int SidForString; // esi
  unsigned int v7; // r12d
  wchar_t *v8; // rax
  __int64 v10; // rcx
  wchar_t *v11; // rdi
  bool v12; // zf
  wchar_t *v13; // rax
  int v14; // edi
  wchar_t *i; // rax
  unsigned int v16; // r14d
  _QWORD *PoolWithTag; // rax
  unsigned int v18; // ebp
  PVOID v19; // rax
  int v20; // r12d
  ACCESS_MASK AccessMask; // ebp
  unsigned __int16 *j; // rbx
  __int64 k; // rax
  unsigned __int16 *v24; // rax
  _WORD *v25; // rbx
  _WORD *v26; // [rsp+40h] [rbp-58h] BYREF
  PSID Sid[10]; // [rsp+48h] [rbp-50h] BYREF
  ACCESS_MASK v28; // [rsp+A0h] [rbp+8h] BYREF
  int v29; // [rsp+A8h] [rbp+10h] BYREF
  int v30; // [rsp+B0h] [rbp+18h] BYREF
  unsigned __int16 *v31; // [rsp+B8h] [rbp+20h] BYREF

  *a2 = nullptr;
  v5 = Str1;
  SidForString = 0;
  v7 = 0;
  Sid[0] = nullptr;
  v8 = wcschr(Str1, 0x3Au);
  *a3 = v8;
  if ( v8 == v5 )
    return 3221225485LL;
  if ( v8 != nullptr )
  {
    v13 = v8 - 1;
  }
  else
  {
    v10 = -1;
    v11 = v5;
    do
    {
      if ( v10 == 0 )
        break;
      v12 = *v11++ == 0;
      --v10;
    }
    while ( !v12 );
    v13 = &v5[-v10 - 2];
  }
  *a3 = v13;
  v14 = 0;
  for ( i = v5; i < *a3; ++i )
  {
    if ( *i == 59 )
    {
      ++v7;
    }
    else if ( *i != 32 )
    {
      v14 = 1;
    }
  }
  v16 = v7 / 5;
  if ( v7 != 5 * (v7 / 5) || v7 == 0 && v14 != 0 )
    return (unsigned int)-1073741811;
  if ( v16 == 0 )
  {
    PoolWithTag = ExAllocatePoolWithTag(PagedPool, 8u, 0x6C416553u);
    *a2 = PoolWithTag;
    if ( PoolWithTag != nullptr )
    {
      *PoolWithTag = 0;
      *(_BYTE *)*a2 = 2;
      *((_BYTE *)*a2 + 1) = 0;
      *((_WORD *)*a2 + 1) = 8;
      *((_WORD *)*a2 + 2) = 0;
      *((_WORD *)*a2 + 3) = 0;
      return SidForString;
    }
    return (unsigned int)-1073741670;
  }
  v18 = 48 * v16 + 8;
  if ( v18 > 0xFFFF )
    v18 = 0xFFFF;
  v19 = ExAllocatePoolWithTag(PagedPool, v18, 0x6C416553u);
  *a2 = v19;
  if ( v19 == nullptr )
    return (unsigned int)-1073741670;
  v29 = 8;
  memset(v19, 0, v18);
  *(_BYTE *)*a2 = 2;
  v20 = 0;
  *((_BYTE *)*a2 + 1) = 0;
  *((_WORD *)*a2 + 1) = v18;
  *((_WORD *)*a2 + 2) = 0;
  *((_WORD *)*a2 + 3) = 0;
  while ( 1 )
  {
    AccessMask = 0;
    v28 = 0;
    while ( *v5 == 32 )
      ++v5;
    if ( *v5 == 40 )
LABEL_30:
      ++v5;
    if ( *v5 == 32 )
      goto LABEL_30;
    if ( wcsnicmp(v5, aA, 1u) != 0 )
      break;
    for ( j = v5 + 2; *j == 32; ++j )
      ;
    if ( *j != 59 )
    {
      SidForString = -1073741811;
      goto LABEL_75;
    }
    do
      ++j;
    while ( *j == 32 );
    if ( *j != 59 )
    {
      do
      {
        while ( *j == 32 )
          ++j;
        if ( (unsigned int)SepSddlLookupAccessMaskInTable(j, &v30, &v31) != 0 )
        {
          AccessMask |= v30;
          v24 = v31;
          v28 = AccessMask;
        }
        else
        {
          SepSddlParseWideStringUlong(j, &v31, &v28);
          v24 = v31;
          AccessMask = v28;
          if ( v31 == j )
          {
            SidForString = -1073741811;
            goto LABEL_40;
          }
        }
        j = v24;
      }
      while ( *v24 != 59 );
    }
    ++j;
LABEL_40:
    if ( SidForString != 0 )
      goto LABEL_75;
    for ( k = 2; k != 0; --k )
    {
      while ( *j == 32 )
        ++j;
      if ( *j != 59 )
        SidForString = -1073741811;
      ++j;
    }
    if ( SidForString != 0 )
      goto LABEL_75;
    while ( *j == 32 )
      ++j;
    SidForString = SepSddlGetSidForString(j, Sid, &v26);
    if ( SidForString != 0 )
      goto LABEL_75;
    v25 = v26;
    if ( v26 == nullptr )
      goto LABEL_71;
    if ( *v26 == 32 )
    {
      do
        ++v25;
      while ( *v25 == 32 );
      v26 = v25;
    }
    if ( *v25 != 41 )
    {
LABEL_71:
      SidForString = -1073741705;
      goto LABEL_75;
    }
    v5 = v25 + 1;
    if ( Sid[0] != nullptr )
    {
      SidForString = SepSddlAddAceToAcl(a2, &v29, 0, 0, AccessMask, v16 - v20, Sid[0]);
      if ( SidForString != 0 )
        goto LABEL_75;
    }
    if ( *v5 == 40 )
      ++v5;
    if ( ++v20 >= v16 )
      goto LABEL_74;
  }
  SidForString = -1073741811;
LABEL_74:
  if ( SidForString != 0 )
  {
LABEL_75:
    ExFreePoolWithTag((PVOID)*a2, 0);
    *a2 = nullptr;
  }
  else
  {
    *((_WORD *)*a2 + 1) = v29;
  }
  return SidForString;
}


__int64 __fastcall SepSddlDaclFromSDDLString(_WORD *a1, int a2, int *a3, PVOID *a4)
{
  wchar_t *v7; // rcx
  int v8; // edi
  unsigned int AclForString; // ebx
  wchar_t *i; // rax
  PVOID P; // [rsp+40h] [rbp+8h] BYREF
  wchar_t *v12; // [rsp+50h] [rbp+18h] BYREF

  *a4 = nullptr;
  *a3 = 0;
  while ( *a1 == 32 )
    ++a1;
  if ( *a1 != 68 || a1[1] != 58 )
    return 3221225485LL;
  v7 = a1 + 2;
  v8 = a2 != 0 ? 8 : 0;
  if ( *v7 == 80 )
  {
    ++v7;
    v8 |= 0x1000u;
  }
  AclForString = SepSddlGetAclForString(v7, (const void **)&P, &v12);
  if ( AclForString != 0 )
    goto LABEL_15;
  for ( i = v12; *i == 32; ++i )
    ;
  if ( *i != 0 )
  {
    AclForString = -1073741811;
LABEL_15:
    if ( P != nullptr )
      ExFreePoolWithTag(P, 0);
    return AclForString;
  }
  *a4 = P;
  *a3 = v8;
  return AclForString;
}


__int64 __fastcall SepSddlSecurityDescriptorFromSDDLString(_WORD *a1, int a2, _QWORD *a3)
{
  void *v4; // rdi
  int v5; // ebx
  PVOID PoolWithTag; // rax
  PACL Dacl; // [rsp+20h] [rbp-38h] BYREF
  _BYTE SecurityDescriptor[2]; // [rsp+28h] [rbp-30h] BYREF
  __int16 v10; // [rsp+2Ah] [rbp-2Eh]
  ULONG BufferLength; // [rsp+70h] [rbp+18h] BYREF
  int v12; // [rsp+78h] [rbp+20h] BYREF

  Dacl = nullptr;
  v4 = nullptr;
  *a3 = 0;
  v5 = SepSddlDaclFromSDDLString(a1, a2, &v12, (PVOID *)&Dacl);
  if ( v5 >= 0 )
  {
    RtlCreateSecurityDescriptor(SecurityDescriptor, 1u);
    RtlSetDaclSecurityDescriptor(SecurityDescriptor, 1u, Dacl, 0);
    BufferLength = 0;
    v10 |= v12;
    RtlAbsoluteToSelfRelativeSD(SecurityDescriptor, nullptr, &BufferLength);
    PoolWithTag = ExAllocatePoolWithTag(PagedPool, BufferLength, 0x64536553u);
    v4 = PoolWithTag;
    if ( PoolWithTag != nullptr )
    {
      v5 = RtlAbsoluteToSelfRelativeSD(SecurityDescriptor, PoolWithTag, &BufferLength);
      if ( v5 >= 0 )
      {
        ExFreePoolWithTag(Dacl, 0);
        *a3 = v4;
        return (unsigned int)v5;
      }
    }
    else
    {
      v5 = -1073741670;
    }
  }
  if ( Dacl != nullptr )
    ExFreePoolWithTag(Dacl, 0);
  if ( v4 != nullptr )
    ExFreePoolWithTag(v4, 0);
  return (unsigned int)v5;
}


__int64 __fastcall SeSddlSecurityDescriptorFromSDDL(unsigned __int16 *a1, int a2, _QWORD *a3)
{
  unsigned __int64 v3; // r9
  _WORD *v7; // rcx
  _WORD *PoolWithTag; // rax
  _WORD *v10; // rsi
  unsigned int v11; // ebx

  v3 = *a1;
  if ( a1[1] == v3 + 2 )
  {
    v7 = *((_WORD **)a1 + 1);
    if ( v7[v3 >> 1] == 0 )
      return SepSddlSecurityDescriptorFromSDDLString(v7, a2, a3);
  }
  PoolWithTag = ExAllocatePoolWithTag(PagedPool, v3 + 2, 0x73546553u);
  v10 = PoolWithTag;
  if ( PoolWithTag != nullptr )
  {
    memmove(PoolWithTag, *((const void **)a1 + 1), *a1);
    v10[(unsigned __int64)*a1 >> 1] = 0;
    v11 = SepSddlSecurityDescriptorFromSDDLString(v10, a2, a3);
    ExFreePoolWithTag(v10, 0);
    return v11;
  }
  else
  {
    *a3 = 0;
    return 3221225626LL;
  }
}


void __fastcall SeSetSecurityAccessMask(char a1, _DWORD *a2)
{
  *a2 = 0;
  if ( (a1 & 1) != 0 || (a1 & 2) != 0 )
    *a2 = 0x80000;
  if ( (a1 & 4) != 0 )
    *a2 |= 0x40000u;
  if ( (a1 & 8) != 0 )
    *a2 |= 0x1000000u;
}


NTSTATUS __fastcall SeUtilSecurityInfoFromSecurityDescriptor(
        PSECURITY_DESCRIPTOR SecurityDescriptor,
        unsigned __int8 *a2,
        _DWORD *a3)
{
  NTSTATUS result; // eax
  int v7; // ebx
  PSID Owner; // [rsp+20h] [rbp-28h] BYREF
  PACL Sacl[4]; // [rsp+28h] [rbp-20h] BYREF
  unsigned __int8 OwnerDefaulted; // [rsp+58h] [rbp+10h] BYREF
  unsigned __int8 SaclPresent; // [rsp+68h] [rbp+20h] BYREF

  *a2 = 0;
  *a3 = 0;
  result = RtlGetOwnerSecurityDescriptor(SecurityDescriptor, &Owner, &OwnerDefaulted);
  if ( result >= 0 )
  {
    v7 = Owner != nullptr;
    result = RtlGetGroupSecurityDescriptor(SecurityDescriptor, &Owner, &OwnerDefaulted);
    if ( result >= 0 )
    {
      if ( Owner != nullptr )
        v7 |= 2u;
      result = RtlGetSaclSecurityDescriptor(SecurityDescriptor, &SaclPresent, Sacl, &OwnerDefaulted);
      if ( result >= 0 )
      {
        if ( SaclPresent != 0 )
          v7 |= 8u;
        result = RtlGetDaclSecurityDescriptor(SecurityDescriptor, &SaclPresent, Sacl, &OwnerDefaulted);
        if ( result >= 0 )
        {
          if ( SaclPresent != 0 )
            v7 |= 4u;
          *a2 = OwnerDefaulted;
          *a3 = v7;
          return 0;
        }
      }
    }
  }
  return result;
}


// attributes: thunk
void __stdcall RtlFreeUnicodeString(PUNICODE_STRING UnicodeString)
{
  __imp_RtlFreeUnicodeString(UnicodeString);
}


__int64 __fastcall CmpRegUtilAllocateUnicodeString(__int64 a1, unsigned __int16 a2)
{
  PVOID PoolWithTag; // rax

  *(_WORD *)a1 = 0;
  *(_WORD *)(a1 + 2) = a2 + 2;
  PoolWithTag = ExAllocatePoolWithTag(PagedPool, a2 + 2LL, 0x63557050u);
  *(_QWORD *)(a1 + 8) = PoolWithTag;
  return PoolWithTag == nullptr ? 0xC000009A : 0;
}


__int64 __fastcall CmRegUtilUcValueSetUcString(HANDLE KeyHandle, PUNICODE_STRING ValueName, unsigned __int16 *a3)
{
  unsigned __int64 v4; // rdx
  int v7; // ebx
  const void *v8; // rdx
  struct _UNICODE_STRING UnicodeString; // [rsp+30h] [rbp-18h] BYREF

  v4 = *a3;
  if ( a3[1] - (unsigned int)v4 < 2 )
  {
    v7 = CmpRegUtilAllocateUnicodeString((__int64)&UnicodeString, v4);
    if ( v7 >= 0 )
    {
      v8 = *((const void **)a3 + 1);
      UnicodeString.Length = *a3;
      memmove(UnicodeString.Buffer, v8, UnicodeString.Length);
      UnicodeString.Buffer[(unsigned __int64)UnicodeString.Length >> 1] = 0;
      v7 = ZwSetValueKey(KeyHandle, ValueName, 0, 1u, UnicodeString.Buffer, UnicodeString.Length + 2);
      RtlFreeUnicodeString(&UnicodeString);
    }
  }
  else
  {
    *(_WORD *)(*((_QWORD *)a3 + 1) + 2 * (v4 >> 1)) = 0;
    return (unsigned int)ZwSetValueKey(KeyHandle, ValueName, 0, 1u, *((PVOID *)a3 + 1), *a3 + 2);
  }
  return (unsigned int)v7;
}


NTSTATUS __fastcall CmRegUtilUcValueSetFullBuffer(
        void *a1,
        struct _UNICODE_STRING *a2,
        ULONG a3,
        void *Data,
        ULONG DataSize)
{
  return ZwSetValueKey(a1, a2, 0, a3, Data, DataSize);
}


NTSTATUS __fastcall CmRegUtilUcValueGetDword(void *a1, struct _UNICODE_STRING *a2, int a3, _DWORD *a4)
{
  NTSTATUS result; // eax
  ULONG v7; // [rsp+30h] [rbp-28h] BYREF
  _DWORD v8[4]; // [rsp+38h] [rbp-20h] BYREF

  result = ZwQueryValueKey(a1, a2, KeyValuePartialInformation, v8, 0x10u, &v7);
  if ( result >= 0 )
  {
    if ( v8[1] == 4 )
      a3 = v8[3];
    else
      result = -1073741788;
  }
  *a4 = a3;
  return result;
}


__int64 __fastcall CmRegUtilCreateUcKey(
        void *a1,
        struct _UNICODE_STRING *a2,
        ACCESS_MASK a3,
        ULONG a4,
        void *a5,
        _DWORD *a6,
        _QWORD *a7)
{
  NTSTATUS v7; // r11d
  void *v8; // rdx
  int v9; // ecx
  struct _OBJECT_ATTRIBUTES v11; // [rsp+40h] [rbp-38h] BYREF
  void *v12; // [rsp+80h] [rbp+8h] BYREF

  v11.Length = 48;
  v11.RootDirectory = a1;
  v11.Attributes = 576;
  v11.SecurityQualityOfService = nullptr;
  v11.SecurityDescriptor = a5;
  v11.ObjectName = a2;
  v7 = ZwCreateKey(&v12, a3, &v11, 0, nullptr, a4, (PULONG)&a5);
  if ( v7 >= 0 )
  {
    v9 = (int)a5;
    v8 = v12;
  }
  else
  {
    v8 = nullptr;
    v9 = 0;
  }
  *a7 = v8;
  if ( a6 != nullptr )
    *a6 = v9;
  return (unsigned int)v7;
}


NTSTATUS __fastcall CmRegUtilOpenExistingUcKey(void *a1, struct _UNICODE_STRING *a2, ACCESS_MASK a3, _QWORD *a4)
{
  NTSTATUS result; // eax
  struct _OBJECT_ATTRIBUTES v6; // [rsp+20h] [rbp-38h] BYREF
  void *v7; // [rsp+60h] [rbp+8h] BYREF

  *a4 = 0;
  v6.Length = 48;
  v6.RootDirectory = a1;
  v6.Attributes = 576;
  v6.SecurityDescriptor = nullptr;
  v6.SecurityQualityOfService = nullptr;
  v6.ObjectName = a2;
  result = ZwOpenKey(&v7, a3, &v6);
  if ( result >= 0 )
    *a4 = v7;
  return result;
}


NTSTATUS __fastcall CmRegUtilWstrValueSetWstrString(HANDLE KeyHandle, PCWSTR SourceString, PCWSTR a3)
{
  NTSTATUS result; // eax
  struct _UNICODE_STRING DestinationString; // [rsp+20h] [rbp-28h] BYREF
  struct _UNICODE_STRING ValueName; // [rsp+30h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&DestinationString, a3);
  if ( result >= 0 )
  {
    result = WdmlibRtlInitUnicodeStringEx(&ValueName, SourceString);
    if ( result >= 0 )
      return CmRegUtilUcValueSetUcString(KeyHandle, &ValueName, &DestinationString.Length);
  }
  return result;
}


NTSTATUS __fastcall CmRegUtilUcValueSetWstrString(HANDLE KeyHandle, PUNICODE_STRING ValueName, PCWSTR SourceString)
{
  NTSTATUS result; // eax
  struct _UNICODE_STRING DestinationString; // [rsp+20h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&DestinationString, SourceString);
  if ( result >= 0 )
    return CmRegUtilUcValueSetUcString(KeyHandle, ValueName, &DestinationString.Length);
  return result;
}


NTSTATUS __fastcall CmRegUtilWstrValueSetUcString(HANDLE KeyHandle, const WCHAR *a2, unsigned __int16 *a3)
{
  NTSTATUS result; // eax
  struct _UNICODE_STRING DestinationString; // [rsp+20h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&DestinationString, a2);
  if ( result >= 0 )
    return CmRegUtilUcValueSetUcString(KeyHandle, &DestinationString, a3);
  return result;
}


int __fastcall CmRegUtilWstrValueSetFullBuffer(HANDLE KeyHandle, const WCHAR *a2, ULONG a3, void *a4, ULONG DataSize)
{
  int result; // eax
  struct _UNICODE_STRING ValueName; // [rsp+30h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&ValueName, a2);
  if ( result >= 0 )
    return ZwSetValueKey(KeyHandle, &ValueName, 0, a3, a4, DataSize);
  return result;
}


int __fastcall CmRegUtilWstrValueGetDword(void *a1, const WCHAR *a2, int a3, _DWORD *a4)
{
  int result; // eax
  struct _UNICODE_STRING DestinationString; // [rsp+20h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&DestinationString, a2);
  if ( result >= 0 )
    return CmRegUtilUcValueGetDword(a1, &DestinationString, a3, a4);
  return result;
}


NTSTATUS __fastcall CmRegUtilCreateWstrKey(
        void *a1,
        const WCHAR *a2,
        ACCESS_MASK a3,
        ULONG a4,
        void *a5,
        _DWORD *a6,
        _QWORD *a7)
{
  NTSTATUS result; // eax
  struct _UNICODE_STRING DestinationString; // [rsp+40h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&DestinationString, a2);
  if ( result >= 0 )
    return CmRegUtilCreateUcKey(a1, &DestinationString, a3, a4, a5, a6, a7);
  return result;
}


int __fastcall CmRegUtilOpenExistingWstrKey(void *a1, const WCHAR *a2, ACCESS_MASK a3, _QWORD *a4)
{
  int result; // eax
  struct _UNICODE_STRING DestinationString; // [rsp+20h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&DestinationString, a2);
  if ( result >= 0 )
    return CmRegUtilOpenExistingUcKey(a1, &DestinationString, a3, a4);
  return result;
}


__int64 __fastcall CmRegUtilUcValueGetFullBuffer(
        HANDLE KeyHandle,
        PUNICODE_STRING ValueName,
        int a3,
        int a4,
        _QWORD *a5)
{
  _QWORD *v5; // rbp
  unsigned int v9; // eax
  ULONG Length; // edi
  _DWORD *PoolWithTag; // rbx
  NTSTATUS v13; // edi
  _DWORD *v14; // rax
  ULONG ResultLength; // [rsp+68h] [rbp+20h] BYREF

  v5 = a5;
  *a5 = 0;
  v9 = (ValueName->Length + 31) & 0x1FFF8;
  Length = v9 + a4;
  if ( v9 + a4 < v9 )
    return 3221225621LL;
  PoolWithTag = ExAllocatePoolWithTag(NonPagedPool, Length, 0x62527050u);
  if ( PoolWithTag == nullptr )
    return 3221225626LL;
  v13 = ZwQueryValueKey(KeyHandle, ValueName, KeyValueFullInformation, PoolWithTag, Length, &ResultLength);
  if ( v13 < 0 )
  {
    ExFreePoolWithTag(PoolWithTag, 0);
    if ( v13 != -2147483643 && v13 != -1073741789 )
      return (unsigned int)v13;
    v14 = ExAllocatePoolWithTag(NonPagedPool, ResultLength, 0x62527050u);
    PoolWithTag = v14;
    if ( v14 == nullptr )
      return 3221225626LL;
    v13 = ZwQueryValueKey(KeyHandle, ValueName, KeyValueFullInformation, v14, ResultLength, &ResultLength);
    if ( v13 < 0 )
    {
LABEL_13:
      ExFreePoolWithTag(PoolWithTag, 0);
      return (unsigned int)v13;
    }
  }
  if ( a3 != 0 && PoolWithTag[1] != a3 )
  {
    v13 = -1073741788;
    goto LABEL_13;
  }
  *v5 = PoolWithTag;
  return 0;
}


NTSTATUS __fastcall CmRegUtilWstrValueGetFullBuffer(HANDLE KeyHandle, const WCHAR *a2, int a3, int a4, _QWORD *a5)
{
  NTSTATUS result; // eax
  struct _UNICODE_STRING DestinationString; // [rsp+30h] [rbp-18h] BYREF

  result = WdmlibRtlInitUnicodeStringEx(&DestinationString, a2);
  if ( result >= 0 )
    return CmRegUtilUcValueGetFullBuffer(KeyHandle, &DestinationString, a3, a4, a5);
  return result;
}


void __cdecl _security_init_cookie()
{
  unsigned __int64 v0; // rax

  v0 = BugCheckParameter2;
  if ( BugCheckParameter2 == 0 || BugCheckParameter2 == 0x2B992DDFA232LL )
  {
    v0 = ((unsigned __int64)&BugCheckParameter2 ^ MEMORY[0xFFFFF78000000320]) & 0xFFFFFFFFFFFFLL;
    if ( v0 == 0 )
      v0 = 0x2B992DDFA232LL;
    BugCheckParameter2 = v0;
  }
  BugCheckParameter3 = ~v0;
}


NTSTATUS __stdcall start(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
  PUNICODE_STRING v2; // r8
  _DRIVER_OBJECT *v3; // r9

  _security_init_cookie();
  return DriverEntry(v3, v2);
}
