"""Run original and reconstructed x64 instructions against the same mock kernel.

Dependencies: unicorn, pefile. No driver is installed or executed in the kernel.
Kernel functions are mocked; native Windows exception unwinding is not emulated.
"""
from pathlib import Path
import json, re, struct, hashlib
from unicorn import Uc, UC_ARCH_X86, UC_MODE_64, UC_HOOK_CODE, UcError
from unicorn.x86_const import *
import pefile

ROOT = Path(__file__).resolve().parents[1]
ORIGINAL = Path(json.loads((ROOT/'dev/ida_export.json').read_text())['input'])
REBUILT = ROOT/'build/ninja/PROCEXP152.sys'
MASK = (1 << 64) - 1
STACK, HEAP, THUNKS, STOP = 0x10000000, 0x60000000, 0x70000000, 0x70010000
INPUT, OUTPUT, IOSB = HEAP+0x1000, HEAP+0x2000, HEAP+0x3000
OBJECT, PROCESS, OWNER = HEAP+0x10000, HEAP+0x11000, HEAP+0x12000
TRAP = HEAP+0x13000

def q(value): return struct.pack('<Q',value & MASK)
def u(value): return struct.pack('<I',value & 0xffffffff)

def symbols():
    text=(ROOT/'build/ninja/PROCEXP152.map').read_text()
    return {name:int(address,16) for name,address in re.findall(
        r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{16})',text,re.M)}

class Machine:
    def __init__(self,path,scenario):
        self.pe=pefile.PE(str(path)); self.base=self.pe.OPTIONAL_HEADER.ImageBase
        self.original=path==ORIGINAL; self.scenario=scenario
        self.uc=Uc(UC_ARCH_X86,UC_MODE_64); self.calls=[];self.hooks={};self.alloc=HEAP+0x40000
        self.uc.mem_map(self.base,(self.pe.OPTIONAL_HEADER.SizeOfImage+0xfff)&~0xfff)
        self.uc.mem_write(self.base,self.pe.get_memory_mapped_image())
        self.uc.mem_map(STACK,0x20000);self.uc.mem_write(STACK,b'\xa5'*0x20000)
        self.uc.mem_map(HEAP,0x100000);self.uc.mem_map(THUNKS,0x20000)
        self.uc.mem_write(THUNKS,b'\xc3'*0x20000)
        for module in self.pe.DIRECTORY_ENTRY_IMPORT:
            for item in module.imports:
                if not item.name: continue
                name=item.name.decode(); address=THUNKS+len(self.hooks)*16
                if name in ('PsThreadType','PsProcessType','IoFileObjectType','IoDeviceObjectType'):
                    index=['PsThreadType','PsProcessType','IoFileObjectType','IoDeviceObjectType'].index(name)
                    address=HEAP+0x8000+index*16
                    self.uc.mem_write(address,q(HEAP+0x9000+index*16))
                else:self.hooks[address]=name
                self.uc.mem_write(item.address,q(address))
        self.uc.mem_write(OBJECT,b'\x00'*0x1000)
        self.uc.mem_write(OBJECT,struct.pack('<HH',5,216))
        self.uc.mem_write(OBJECT+0x28,q(OWNER))
        self.uc.mem_write(OBJECT+0x30,q(HEAP+0x18000))
        for offset in (0x38,0x48,0x58):self.uc.mem_write(OBJECT+offset,q(TRAP))
        self.uc.mem_write(TRAP+0x30,q(0x5555));self.uc.mem_write(TRAP+0x38,q(0x6666))
        if scenario.get('file_object'):
            self.uc.mem_write(OBJECT+8,q(HEAP+0x14000))
            self.uc.mem_write(HEAP+0x14000,struct.pack('<H',3))
            self.uc.mem_write(OBJECT+0x50,u(2))
            raw='\\example.txt'.encode('utf-16-le')
            self.uc.mem_write(OBJECT+0x58,struct.pack('<HHIQ',len(raw),len(raw)+2,0,OBJECT+0x100))
            self.uc.mem_write(OBJECT+0x100,raw+b'\x00\x00')
        if scenario.get('bad_object_size'):self.uc.mem_write(OBJECT+2,struct.pack('<H',214))
        self.set_global('gBuildNumber',0x5228,scenario.get('build',9600),4)
        self.set_global('gMutantObjectType',0x5220,HEAP+0xa000)
        for name,rva,hook in [('gAcquireProcessExitSynchronization',0x51e0,'AcquireExitSync'),
                              ('gReleaseProcessExitSynchronization',0x51e8,'ReleaseExitSync'),
                              ('gGetMaximumNonPagedPoolInBytes',0x51f0,'GetPoolSize')]:
            address=THUNKS+0xf000+len(self.hooks)*16
            self.hooks[address]=hook
            enabled=scenario.get('sync',True) if 'ExitSync' in hook else scenario.get('pool',True)
            self.set_global(name,rva,address if enabled else 0)
        self.uc.hook_add(UC_HOOK_CODE,self.on_code)
    def set_global(self,name,original_rva,value,size=8):
        address=self.base+original_rva if self.original else SYMBOLS[name]
        self.uc.mem_write(address,u(value) if size==4 else q(value))
    def readq(self,address):return struct.unpack('<Q',self.uc.mem_read(address,8))[0]
    def readu(self,address):return struct.unpack('<I',self.uc.mem_read(address,4))[0]
    def args(self,n):
        regs=[UC_X86_REG_RCX,UC_X86_REG_RDX,UC_X86_REG_R8,UC_X86_REG_R9]
        sp=self.uc.reg_read(UC_X86_REG_RSP)
        return [self.uc.reg_read(regs[i]) if i<4 else self.readq(sp+0x28+(i-4)*8) for i in range(n)]
    def returned(self,value):
        sp=self.uc.reg_read(UC_X86_REG_RSP)
        self.uc.reg_write(UC_X86_REG_RAX,value & MASK)
        self.uc.reg_write(UC_X86_REG_RSP,sp+8)
        self.uc.reg_write(UC_X86_REG_RIP,self.readq(sp))
    def new(self,size):
        out=self.alloc;self.alloc+=(size+0xff)&~0xff
        if self.alloc>=HEAP+0xf0000:raise RuntimeError('mock heap limit')
        self.uc.mem_write(out,b'\x00'*size);return out
    def string(self,desc,text):
        raw=text.encode('utf-16-le');ptr=desc+16
        self.uc.mem_write(desc,struct.pack('<HHIQ',len(raw),len(raw)+2,0,ptr))
        self.uc.mem_write(ptr,raw+b'\x00\x00')
    def on_code(self,uc,address,size,user):
        if address==STOP:uc.emu_stop();return
        if address not in self.hooks:return
        name=self.hooks[address];a=self.args(7)
        meaningful=[]
        if name=='ObReferenceObjectByHandle':meaningful=a[:4]
        elif name=='ObOpenObjectByPointer':meaningful=[a[i] for i in (0,1,3,4,5)]
        elif name=='ZwOpenProcess':meaningful=[a[1],self.readu(a[2]+24),self.readq(a[3])]
        elif name=='ZwDuplicateObject':meaningful=[a[i] for i in (0,1,2,4,5,6)]
        elif name=='ZwOpenProcessToken':meaningful=a[:2]
        elif name=='ZwQuerySystemInformation':meaningful=[a[0],a[2]]
        elif name=='ZwQueryInformationProcess':meaningful=[a[0],a[1],a[3]]
        elif name=='ZwQueryObject':meaningful=[a[0],a[1],a[3]]
        elif name=='ObQueryNameString':meaningful=[a[0],a[2]]
        elif name=='ExAllocatePoolWithTag':meaningful=a[:3]
        elif name=='KeWaitForSingleObject':meaningful=a[:4]
        elif name in ('ZwClose','ObfDereferenceObject','AcquireExitSync','ReleaseExitSync','KeStackAttachProcess','MmIsAddressValid'):
            meaningful=a[:1]
        elif name=='ObCloseHandle':meaningful=a[:2]
        elif name=='ExFreePoolWithTag':meaningful=a[:2]
        elif name=='RtlUnicodeStringToAnsiString':meaningful=[struct.unpack('<H',self.uc.mem_read(a[1],2))[0],a[2]]
        elif name=='SePrivilegeCheck':
            meaningful=[self.readu(a[0]),self.readu(a[0]+4),self.readq(a[0]+8),self.readu(a[0]+16),a[2]]
        elif name=='IofCompleteRequest':meaningful=[self.readu(a[0]+0x30),self.readq(a[0]+0x38),a[1]]
        elif name=='IoDeleteSymbolicLink':
            length=struct.unpack('<H',self.uc.mem_read(a[0],2))[0]
            meaningful=[length]
            self.calls.append({'api':name,'name':bytes(self.uc.mem_read(self.readq(a[0]+8),length)).decode('utf-16-le')})
        elif name=='IoDeleteDevice':meaningful=a[:1]
        # Windows x64 leaves upper register/stack bits unspecified for narrow
        # arguments. Compare only the widths declared by each kernel ABI.
        widths={'ObReferenceObjectByHandle':[64,32,64,8],
                'ObOpenObjectByPointer':[64,32,32,64,8],
                'ZwOpenProcess':[32,32,64],'ZwDuplicateObject':[64,64,64,32,32,32],
                'ZwOpenProcessToken':[64,32],'ZwQuerySystemInformation':[32,32],
                'ZwQueryInformationProcess':[64,32,32],'ZwQueryObject':[64,32,32],
                'ObQueryNameString':[64,32],'ExAllocatePoolWithTag':[32,64,32],
                'KeWaitForSingleObject':[64,32,8,8],'ObCloseHandle':[64,8],
                'ExFreePoolWithTag':[64,32],'RtlUnicodeStringToAnsiString':[16,8],
                'SePrivilegeCheck':[32,32,64,32,8],'IofCompleteRequest':[32,64,8]}.get(name)
        if widths:meaningful=[value & ((1<<bits)-1) for value,bits in zip(meaningful,widths)]
        self.calls.append({'api':name,'arguments':[hex(value) for value in meaningful]})
        failure=self.scenario.get('fail')==name
        status=self.scenario.get('api_status',{}).get(name,0)
        if failure:status=0xc0000022
        if name in ('ObReferenceObjectByHandle','PsLookupProcessByProcessId'):
            out=a[4] if name=='ObReferenceObjectByHandle' else a[1]
            if status<0x80000000:self.uc.mem_write(out,q(OBJECT if name.startswith('Ob') else PROCESS))
        elif name in ('ObOpenObjectByPointer','ZwOpenProcess','ZwOpenProcessToken','ZwDuplicateObject'):
            index={'ObOpenObjectByPointer':6,'ZwOpenProcess':0,'ZwOpenProcessToken':2,'ZwDuplicateObject':3}[name]
            if status<0x80000000:self.uc.mem_write(a[index],q({'ObOpenObjectByPointer':0x88,'ZwOpenProcess':0x77,'ZwOpenProcessToken':0x99,'ZwDuplicateObject':0x66}[name]))
        elif name=='KeWaitForSingleObject':status=0 if self.scenario.get('terminated') else 0x102
        elif name=='ZwQueryInformationProcess':
            if not status:
                self.uc.mem_write(a[2],u(0x1234));self.uc.mem_write(a[4],u(4))
        elif name=='ZwQuerySystemInformation':
            if not status:
                self.uc.mem_write(a[1],b'\x00'*min(a[2],0x130))
                self.uc.mem_write(a[1],u(1))
                self.uc.mem_write(a[1]+0x18,q(HEAP+0x50000));self.uc.mem_write(a[1]+0x20,u(0x1000))
                if a[3]:self.uc.mem_write(a[3],u(0x130))
                if a[2]==0x130:status=0xc0000004
        elif name=='GetPoolSize':status=0x40000000
        elif name=='AcquireExitSync':pass
        elif name=='ObQueryNameString':
            if not status:
                self.string(a[1],self.scenario.get('name','\\Device\\Example'))
                if a[3]:self.uc.mem_write(a[3],u(64))
        elif name=='ZwQueryObject':
            if not a[2]:
                self.uc.mem_write(a[4],u(0x100));status=0xc0000004
            elif not status:self.string(a[2],'File')
        elif name=='ExAllocatePoolWithTag':status=0 if failure else self.new(a[1])
        elif name=='RtlUnicodeStringToAnsiString':
            if not status:
                length=struct.unpack('<H',self.uc.mem_read(a[1],2))[0];ptr=self.readq(a[1]+8)
                raw=bytes(self.uc.mem_read(ptr,length)).decode('utf-16-le').encode('ascii')
                dest=self.new(len(raw)+1);self.uc.mem_write(dest,raw+b'\x00')
                self.uc.mem_write(a[0],struct.pack('<HHIQ',len(raw),len(raw)+1,0,dest))
        elif name in ('memmove','memcpy'):
            self.uc.mem_write(a[0],bytes(self.uc.mem_read(a[1],a[2])));status=a[0]
        elif name=='memset':self.uc.mem_write(a[0],bytes([a[1]&255])*a[2]);status=a[0]
        elif name=='strncpy':
            raw=bytes(self.uc.mem_read(a[1],a[2]));self.uc.mem_write(a[0],raw);status=a[0]
        elif name=='KeGetCurrentIrql':status=0
        elif name=='RtlInitUnicodeString':
            length=0
            while self.uc.mem_read(a[1]+length,2)!=b'\x00\x00':length+=2
            self.uc.mem_write(a[0],struct.pack('<HHIQ',length,length+2,0,a[1]))
        elif name=='MmIsAddressValid':status=1
        elif name in ('SeCaptureSubjectContext','SeReleaseSubjectContext','IofCompleteRequest',
                      'KeStackAttachProcess','KeUnstackDetachProcess','ObfDereferenceObject',
                      'ObCloseHandle','ZwClose','ExFreePoolWithTag','RtlFreeAnsiString','ReleaseExitSync',
                      'IoDeleteSymbolicLink','IoDeleteDevice'):
            pass
        elif name=='ExGetPreviousMode':status=1
        elif name=='SePrivilegeCheck':status=0 if self.scenario.get('deny_privilege') else 1
        elif name=='KeBugCheckEx':raise RuntimeError('emulated bugcheck')
        else:raise RuntimeError('unimplemented kernel mock: '+name)
        self.returned(status)
    def call(self,address,args):
        sp=STACK+0x18000-8
        self.uc.mem_write(sp,q(STOP))
        regs=[UC_X86_REG_RCX,UC_X86_REG_RDX,UC_X86_REG_R8,UC_X86_REG_R9]
        for i,arg in enumerate(args):
            if i<4:self.uc.reg_write(regs[i],arg&MASK)
            else:self.uc.mem_write(sp+0x28+(i-4)*8,q(arg))
        self.uc.reg_write(UC_X86_REG_RSP,sp)
        self.uc.emu_start(address,STOP+1,count=200000)
        if self.uc.reg_read(UC_X86_REG_RIP)!=STOP:raise RuntimeError('instruction limit')
        return self.uc.reg_read(UC_X86_REG_RAX)&0xffffffff
    def run(self,test):
        raw=bytearray(512)
        raw[:8]=q(test.get('input_value',152))
        if test.get('request'):
            raw[:32]=struct.pack('<IIQQQ',test.get('pid',42),0,
                                  OBJECT+(8 if test.get('wrong_object') else 0),
                                  1 if test.get('file_object') else 0,0x44)
        if test.get('memory'):
            raw[:16]=q(0x44)+q(HEAP+0x50000)
        self.uc.mem_write(INPUT,bytes(raw));self.uc.mem_write(OUTPUT,b'\xcd'*512)
        self.uc.mem_write(HEAP+0x50000,bytes(range(256))*16)
        self.uc.mem_write(IOSB,b'\xcc'*16)
        if test.get('kind')=='dispatch':
            irp,stack=HEAP+0x7000,HEAP+0x7500
            self.uc.mem_write(irp,b'\x00'*0xd0)
            self.uc.mem_write(irp+0x18,q(INPUT));self.uc.mem_write(irp+0xb8,q(stack))
            self.uc.mem_write(irp+0x30,b'\xcc'*16)
            self.uc.mem_write(stack,bytes([test['major']]))
            self.uc.mem_write(stack+8,u(test['output_length']))
            self.uc.mem_write(stack+16,u(test['input_length']))
            self.uc.mem_write(stack+24,u(test['ioctl']))
            addr=self.base+0x2220 if self.original else SYMBOLS['ProcExpDispatch']
            value=self.call(addr,[HEAP+0x6000,irp])
            return {'return':hex(value),'iosb':bytes(self.uc.mem_read(irp+0x30,16)).hex(),
                    'output':bytes(self.uc.mem_read(INPUT,512)).hex(),'kernel_calls':self.calls}
        if test.get('kind')=='unload':
            self.uc.mem_write(HEAP+0x6008,q(HEAP+0x6500))
            addr=self.base+0x3280 if self.original else SYMBOLS['ProcExpUnload']
            self.call(addr,[HEAP+0x6000])
            return {'kernel_calls':self.calls}
        addr=self.base+0x1ae0 if self.original else SYMBOLS['ProcExpDeviceControl']
        value=self.call(addr,[0,1,INPUT,test['input_length'],OUTPUT,test['output_length'],test['ioctl'],IOSB])
        return {'return':hex(value),'iosb':bytes(self.uc.mem_read(IOSB,16)).hex(),
                'output':bytes(self.uc.mem_read(OUTPUT,512)).hex(),'kernel_calls':self.calls}

def make_tests():
    lengths={0:(32,64),4:(32,0),8:(4,4),12:(8,8),20:(32,8),32:(8,8),36:(16,16),
             40:(8,24),44:(8,8),52:(8,4),60:(8,8),64:(32,64),68:(8,4),72:(32,64),76:(32,64),80:(0,8)}
    tests=[]
    for offset,(il,ol) in lengths.items():
        for badil,badol in [(0,0),(1,1),(4,4),(8,8),(16,24),(32,64),(64,128)]:
            if (badil,badol)==(il,ol):continue
            tests.append({'label':f'length_{offset:02x}_{badil}_{badol}','ioctl':0x83350000+offset,
                          'input_length':badil,'output_length':badol,'request':True})
        base={'label':f'valid_{offset:02x}','ioctl':0x83350000+offset,'input_length':il,'output_length':ol}
        if offset in (0,4,20,64,72,76):base['request']=True
        if offset in (32,68):base['input_value']=OBJECT if offset==32 else HEAP+0x50000
        if offset==36:base['memory']=True
        tests.append(base)
        if offset in (0,64,72,76):
            for name,scenario in [('no_sync',{'sync':False}),('system_pid',{'pid':4}),
                                  ('ref_failure',{'fail':'ObReferenceObjectByHandle'}),
                                  ('sync_failure',{'fail':'AcquireExitSync'}),
                                  ('lookup_failure',{'fail':'PsLookupProcessByProcessId'})]:
                tests.append(dict(base,label=base['label']+'_'+name,**scenario))
            tests.append(dict(base,label=base['label']+'_wrong_object',wrong_object=True))
        if offset in (0,72):
            for name,scenario in [('file_name',{'file_object':True}),
                                  ('file_bad_size',{'file_object':True,'bad_object_size':True}),
                                  ('file_query_failure',{'file_object':True,'fail':'ObQueryNameString'}),
                                  ('ansi_failure',{'fail':'RtlUnicodeStringToAnsiString'}),
                                  ('query_failure',{'fail':'ObQueryNameString'})]:
                tests.append(dict(base,label=base['label']+'_'+name,**scenario))
        if offset in (12,40,44,52,60,20,4):
            tests.append(dict(base,label=base['label']+'_ref_failure',fail='ObReferenceObjectByHandle'))
        if offset in (40,):tests.append(dict(base,label=base['label']+'_terminated',terminated=True))
    for version in [0,151,152,153,0xffffffff]:
        tests.append({'label':f'version_{version}','ioctl':0x83350008,'input_length':4,'output_length':4,'input_value':version})
    for code in [0,0x83350010,0x83350018,0x83350054,0xffffffff]:
        tests.append({'label':f'unknown_{code:x}','ioctl':code,'input_length':0,'output_length':0})
    tests.append({'label':'pool_missing','ioctl':0x83350050,'input_length':0,'output_length':8,'pool':False})
    tests.append({'label':'pending_open_process','ioctl':0x8335003c,'input_length':8,'output_length':8,'api_status':{'ZwOpenProcess':259}})
    for api in ('ObOpenObjectByPointer','ZwOpenProcessToken'):
        tests.append({'label':'token_failure_'+api,'ioctl':0x8335000c,'input_length':8,'output_length':8,'fail':api})
    tests.append({'label':'ignored_owner_open_failure','ioctl':0x8335002c,'input_length':8,'output_length':8,'fail':'ObOpenObjectByPointer'})
    for label,major,extra in [('create',0,{}),('create_denied',0,{'deny_privilege':True}),
                              ('close',2,{}),('unknown_major',3,{}),('device_control',14,{})]:
        tests.append(dict(label='dispatch_'+label,kind='dispatch',major=major,
                          ioctl=0x83350008,input_length=4,output_length=4,**extra))
    tests.append({'label':'unload','kind':'unload','ioctl':0,'input_length':0,'output_length':0})
    return tests

if __name__=='__main__':
    SYMBOLS=symbols();results=[]
    for test in make_tests():
        pair=[]
        for path in (ORIGINAL,REBUILT):
            try: pair.append(Machine(path,test).run(test))
            except (UcError,RuntimeError,ValueError) as exc: pair.append({'unresolved':str(exc)})
        unresolved=any('unresolved' in item for item in pair)
        passed=not unresolved and pair[0]==pair[1]
        results.append({'test':test,'status':'unresolved' if unresolved else ('pass' if passed else 'mismatch'),
                        **({'original':pair[0],'rebuilt':pair[1]} if not passed else {})})
        if not passed: print(test['label'],results[-1]['status'],[(r.get('unresolved') or {k:v for k,v in r.items() if k!='output'}) for r in pair])
    report={'method':'Unicorn x64 original/rebuilt instruction differential execution with mock kernel APIs',
            'original_sha256':hashlib.sha256(ORIGINAL.read_bytes()).hexdigest(),
            'rebuilt_sha256':hashlib.sha256(REBUILT.read_bytes()).hexdigest(),
            'summary':{status:sum(r['status']==status for r in results) for status in ('pass','mismatch','unresolved')},
            'limitations':['Mock kernel APIs, not Windows kernel execution','Native exception dispatch/unwinding is not emulated',
                           'No concurrency, kernel lifetime, PatchGuard, or OS-version compatibility validation',
                           'Passing cases do not prove equivalence on every input or undefined behavior'],
            'results':results}
    (ROOT/'dev/differential_report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report['summary']))
