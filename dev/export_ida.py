from pathlib import Path
import urllib.request, json, base64, zlib

root = Path(__file__).resolve().parents[1]
(root/'dev').mkdir(parents=True, exist_ok=True)
code = r'''
import ida_nalt, ida_funcs, ida_hexrays, ida_bytes, ida_typeinf, ida_segment, idautils, ida_name, json
functions=[]
for ea in idautils.Functions():
 f=ida_funcs.get_func(ea)
 try:
  cf=ida_hexrays.decompile(ea)
  text=str(cf) if cf else None
 except Exception as ex: text=None
 functions.append({'ea':ea,'name':ida_funcs.get_func_name(ea),'end':f.end_ea,'flags':f.flags,'code':text})
segments=[]
for ea in idautils.Segments():
 s=ida_segment.getseg(ea)
 segments.append({'name':ida_segment.get_segm_name(s),'start':s.start_ea,'end':s.end_ea,'bytes':(ida_bytes.get_bytes(s.start_ea,s.end_ea-s.start_ea) or b'').hex()})
names=[]
for ea,name in idautils.Names():
 if not ida_funcs.get_func(ea):
  ti=ida_typeinf.tinfo_t()
  ida_nalt.get_tinfo(ti,ea)
  names.append({'ea':ea,'name':name,'type':str(ti),'size':ida_bytes.get_item_size(ea)})
import base64,zlib
export_blob=base64.b64encode(zlib.compress(json.dumps({'input':ida_nalt.get_input_file_path(),'sha256':ida_nalt.retrieve_input_file_sha256().hex(),'functions':functions,'segments':segments,'names':names}).encode())).decode()
print(len(export_blob))
'''
p={'jsonrpc':'2.0','id':1,'method':'tools/call','params':{'name':'py_eval','arguments':{'code':code}}}
r=urllib.request.Request('http://127.0.0.1:13337/mcp',json.dumps(p).encode(),{'Content-Type':'application/json','Accept':'application/json, text/event-stream'})
result=json.load(urllib.request.urlopen(r,timeout=120))['result']
result=result.get('structuredContent') or json.loads(result['content'][0]['text'])
if result.get('stderr'): print(result['stderr'])
length=int(result['stdout'].strip())
chunks=[]
for offset in range(0,length,4000):
 p['params']['arguments']['code']=f'print(export_blob[{offset}:{offset+4000}])'
 r=urllib.request.Request('http://127.0.0.1:13337/mcp',json.dumps(p).encode(),{'Content-Type':'application/json','Accept':'application/json, text/event-stream'})
 result=json.load(urllib.request.urlopen(r,timeout=30))['result']
 result=result.get('structuredContent') or json.loads(result['content'][0]['text'])
 chunks.append(result['stdout'].strip())
data=json.loads(zlib.decompress(base64.b64decode(''.join(chunks))))
(root/'dev/ida_export.json').write_text(json.dumps(data,indent=2),encoding='utf-8')
print('Functions:',len(data['functions']),'Decompiled:',sum(bool(f['code']) for f in data['functions']))
print('Segments:',[(s['name'],hex(s['start']),s['end']-s['start']) for s in data['segments']])
