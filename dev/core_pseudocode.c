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
