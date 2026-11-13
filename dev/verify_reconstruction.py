"""Static checks against the original IDA export and native PE structure.

This deliberately does not load either driver or claim runtime equivalence.
Requires pefile in the Python environment.
"""
from pathlib import Path
import hashlib, json, re, struct
import pefile

root = Path(__file__).resolve().parents[1]
data = json.loads((root/'dev/ida_export.json').read_text())
mapping = json.loads((root/'dev/function_map.json').read_text())
core = [f for f in mapping if f['category'] == 'core']
source = '\n'.join(p.read_text(encoding='utf-8') for p in (root/'src').glob('*.c'))
for function in core:
    assert re.search(r'\b'+re.escape(function['recovered'])+r'\s*\([^;]*?\)\s*\{', source, re.S), function
original_dispatch = next(f['code'] for f in data['functions'] if f['ea'] == 0x180001ae0)
recovered_dispatch = (root/'src/procexp_recovered.c').read_text()
original_cases = re.findall(r'case (\d+)LL:', original_dispatch)
recovered_cases = re.findall(r'case (\d+)LL:', recovered_dispatch)
assert original_cases == recovered_cases
binary = root/'build/ninja/PROCEXP152.sys'
pe = pefile.PE(str(binary))
assert pe.FILE_HEADER.Machine == 0x8664
assert pe.OPTIONAL_HEADER.Subsystem == 1
assert pe.OPTIONAL_HEADER.CheckSum == pe.generate_checksum()
imports = {d.dll.decode(): [i.name.decode() if i.name else str(i.ordinal) for i in d.imports]
           for d in pe.DIRECTORY_ENTRY_IMPORT}
assert set(imports) == {'ntoskrnl.exe'}
assert '__C_specific_handler' in imports['ntoskrnl.exe']
assert pe.OPTIONAL_HEADER.DATA_DIRECTORY[4].Size == 0
assert b'P\x00R\x00O\x00C\x00E\x00X\x00P\x001\x005\x002\x00' in binary.read_bytes()
seh_entries = []
for entry in pe.DIRECTORY_ENTRY_EXCEPTION:
    flags = pe.get_data(entry.struct.UnwindData,1)[0] >> 3
    if flags & 3:
        seh_entries.append(hex(entry.struct.BeginAddress))
report = {
    'input_path': data['input'], 'input_sha256': data['sha256'],
    'ida_functions_exported': len(data['functions']),
    'ida_functions_decompiled': sum(bool(f['code']) for f in data['functions']),
    'core_function_definitions_verified': len(core),
    'device_control_cases_match_original': True,
    'ioctl_codes': [hex((int(case) - 2093678592) & 0xffffffff) for case in original_cases],
    'rebuilt_machine': 'AMD64', 'rebuilt_subsystem': 'Native',
    'pe_checksum_verified': True, 'signed': False,
    'exception_handler_import_verified': True,
    'functions_with_exception_handlers': seh_entries,
    'runtime_tested': False, 'runtime_equivalence_proven': False,
    'rebuilt_bytes': binary.stat().st_size,
    'rebuilt_sha256': hashlib.sha256(binary.read_bytes()).hexdigest(),
    'imports': imports,
}
emulation_path = root/'dev/differential_report.json'
if emulation_path.exists():
    emulation=json.loads(emulation_path.read_text())
    assert emulation['original_sha256']==report['input_sha256']
    assert emulation['rebuilt_sha256']==report['rebuilt_sha256']
    assert emulation['summary']['mismatch']==0 and emulation['summary']['unresolved']==0
    report['instruction_emulation']={'summary':emulation['summary'],'limitations':emulation['limitations']}
(root/'dev/verification.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
print(json.dumps({k:v for k,v in report.items() if k != 'imports'},indent=2))
