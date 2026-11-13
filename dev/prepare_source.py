"""Normalize the archived IDA output. Hand reconstructed routines live in src/."""
from pathlib import Path
import json, re

root = Path(__file__).resolve().parents[1]
export = json.loads((root/'dev/ida_export.json').read_text())
skip = {0x1800015d0,0x180002220,0x180002680,0x180002e40,0x180003280}
functions = [f for f in export['functions'] if 0x180001950 <= f['ea'] <= 0x180003370 and f['ea'] not in skip]
aliases = {
 'sub_180001950':'ProcExpDuplicateHandle',
 'sub_180001A20':'ProcExpCloseRemoteHandle',
 'sub_180001AE0':'ProcExpDeviceControl',
 'sub_180002220':'ProcExpDispatch',
 'sub_180002320':'ProcExpQueryFileName',
 'sub_180002500':'ProcExpQueryThreadContext',
 'sub_1800025E0':'ProcExpQueryMutantOwner',
 'sub_180002680':'ProcExpQueryObjectName',
 'sub_180002B40':'ProcExpQueryObjectType',
 'sub_180002E40':'ProcExpLookupObjectType',
 'sub_180002FD0':'ProcExpDuplicateRequestHandle',
 'sub_180002FF0':'ProcExpQueryExecuteFlags',
 'sub_1800030A0':'ProcExpReadThreadMemory',
 'sub_180003280':'ProcExpUnload',
 'sub_1800032F0':'ProcExpReopenHandle',
 'sub_180003370':'ProcExpQueryModules',
 'qword_1800051E0':'gAcquireProcessExitSynchronization',
 'qword_1800051E8':'gReleaseProcessExitSynchronization',
 'qword_1800051F0':'gGetMaximumNonPagedPoolInBytes',
 'qword_180005218':'gGetObjectType',
 'ObjectType':'gMutantObjectType',
 'BuildNumber':'gBuildNumber',
}
def normalize(code):
    # Call-site argument hints are not C syntax.
    code=re.sub(r'\b\w+: (?=[(&\w])','',code)
    code=code.replace('nullptr','NULL')
    code=code.replace('(unsigned int)v11','(unsigned int)(ULONG_PTR)v11')
    code=code.replace('(unsigned int)v10','(unsigned int)(ULONG_PTR)v10')
    code=code.replace('(unsigned int)v4','(unsigned int)(ULONG_PTR)v4')
    code=code.replace('(POBJECT_TYPE)PsThreadType','*PsThreadType')
    code=code.replace('(POBJECT_TYPE)PsProcessType','*PsProcessType')
    code=code.replace('(POBJECT_TYPE)v14','(v14 ? *v14 : NULL)')
    code=re.sub(r'\*\(_OWORD \*\)&(\w+)\.SecurityDescriptor = 0;',r'RtlZeroMemory(&\1.SecurityDescriptor, 16);',code)
    for old,new in aliases.items(): code=re.sub(r'\b'+old+r'\b',new,code)
    return code
prototypes=[]
bodies=[]
for f in functions:
    code=normalize(f['code'])
    if f['ea']==0x180001950:
        code=code.replace('unsigned __int128 v11;','CLIENT_ID v11;')
        code=code.replace('v11 = a1;','v11.UniqueProcess = (HANDLE)(ULONG_PTR)a1;\n  v11.UniqueThread = NULL;')
    if f['ea']==0x180001a20:
        code=code.replace('    v3 = ObReferenceObjectByHandle', '    __try {\n    v3 = ObReferenceObjectByHandle',1)
        code=code.replace('    KeUnstackDetachProcess(&ApcState);\n    ObfDereferenceObject(Process);',
                          '    } __finally {\n      KeUnstackDetachProcess(&ApcState);\n      ObfDereferenceObject(Process);\n    }')
    if f['ea']==0x180002320:
        code=code.replace('  v10 = *(void **)(a2 + 8);','  __try {\n  v10 = *(void **)(a2 + 8);',1)
        code=code.rsplit('  return 0;\n}',1)[0]+'  return 0;\n  } __except (EXCEPTION_EXECUTE_HANDLER) {\n    return STATUS_ACCESS_VIOLATION;\n  }\n}'
    if f['ea']==0x180001ae0:
        code=code.replace('ProcExpReadThreadMemory(a3, ProcessHandle, v8 + 2)',
                          'ProcExpReadThreadMemory((LONG_PTR)a3, (_BYTE *)ProcessHandle, (_QWORD *)(v8 + 2))')
        # Hex-Rays split one 0x130-byte module buffer across three locals.
        code=code.replace('char SystemInformation[24];','UCHAR SystemInformation[0x130];')
        code=code.replace('unsigned __int64 v34;','/* module ImageBase at +0x18 */')
        code=code.replace('unsigned int v35;','/* module ImageSize at +0x20 */')
        code=re.sub(r'\bv34\b','(*(_QWORD *)(SystemInformation + 0x18))',code)
        code=re.sub(r'\bv35\b','(*(_DWORD *)(SystemInformation + 0x20))',code)
        code=code.replace('ProcessExecuteFlags','(PROCESSINFOCLASS)34')
        code=code.replace('SystemModuleInformation','(SYSTEM_INFORMATION_CLASS)11')
        # First argument is a file object, unused except by an inlined helper.
        code=code.replace('gGetMaximumNonPagedPoolInBytes(a1)','gGetMaximumNonPagedPoolInBytes()')
    code=code.replace('ProcessExecuteFlags','(PROCESSINFOCLASS)34')
    code=code.replace('SystemModuleInformation','(SYSTEM_INFORMATION_CLASS)11')
    # Original RVA 0x3140 reads CR8 inline; avoid the new WDK's imported helper.
    code=code.replace('CurrentIrql = KeGetCurrentIrql();','CurrentIrql = (UCHAR)__readcr8();')
    prototypes.append(code.split('\n{',1)[0]+';')
    bodies.append(f'/* Original RVA 0x{f["ea"]-0x180000000:04X}: {f["name"]} */\n'+code)
(root/'src/procexp_recovered.c').write_text('/* Reconstructed from IDA; see dev/RECONSTRUCTION.txt. */\n#include "procexp_compat.h"\n/* Preserve the original uninitialized-handle path; do not repair it. */\n#pragma warning(3:4703)\n\n'+'\n\n'.join(bodies)+'\n',encoding='utf-8')
(root/'src/procexp_functions.h').write_text('#pragma once\n\n'+'\n'.join(prototypes)+'\n\nDRIVER_INITIALIZE DriverEntry;\nDRIVER_DISPATCH ProcExpDispatch;\nDRIVER_UNLOAD ProcExpUnload;\nPOBJECT_TYPE ProcExpLookupObjectType(PCWSTR Name);\nNTSTATUS ProcExpQueryObjectName(char Unicode, ULONG *Request, LONG_PTR Output, LONG64 *OutputSize);\n',encoding='utf-8')
(root/'dev/function_map.json').write_text(json.dumps([{'rva':hex(f['ea']-0x180000000),'original':f['name'],'recovered':aliases.get(f['name'],f['name']),'category':'core' if 0x1800015d0<=f['ea']<=0x180003370 else 'wdk_or_runtime'} for f in export['functions']],indent=2)+'\n')
print('Generated',len(functions),'core function bodies')
