#!/usr/bin/env python3
"""Compile selected headers alone and together with the actual Contest template."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import argparse,hashlib,json,os,re,subprocess,time
from catalogue import ROOT,active_headers,closure

ap=argparse.ArgumentParser()
ap.add_argument('--jobs',type=int,default=2)
args=ap.parse_args()
headers=active_headers()
deps=closure(headers)
template=(ROOT/'content/contest/template.cpp').read_text()
preamble,sep,_=template.partition('int main() {')
assert sep
assert 'using vi' not in preamble
for p in deps:
    code=p.read_text().split('*/',1)[-1]
    assert not re.search(r'\b(?:vi|vvi)\b',code),p
    assert not re.search(r'^\s*#define\s+f\b',code,re.M),p
out=ROOT/'validation/compile'
out.mkdir(parents=True,exist_ok=True)
build=ROOT/'build/compile'
build.mkdir(parents=True,exist_ok=True)
def compile_one(name,source):
    cpp=build/(name+'.cpp');cpp.write_text(source)
    command=[os.environ.get('CXX','g++'),'-std=gnu++17','-Wall','-Wextra','-Werror',
             '-I',str(ROOT),'-fsyntax-only',str(cpp)]
    start=time.monotonic()
    p=subprocess.run(command,capture_output=True,text=True)
    (out/(name+'.log')).write_text(p.stdout+p.stderr)
    return {'name':name,'exit_code':p.returncode,'seconds':round(time.monotonic()-start,2)}
jobs=[]
for p in headers:
    rel=p.relative_to(ROOT)
    name=str(rel).replace('/','_').replace('.','_')
    jobs.append((name,preamble+'\n#include "'+str(rel)+'"\nint main() {}\n'))
with ThreadPoolExecutor(max_workers=args.jobs) as pool:
    results=list(pool.map(lambda x:compile_one(*x),jobs))
all_includes=''.join('#include "'+str(p.relative_to(ROOT))+'"\n' for p in headers)
results.append(compile_one('all-headers',preamble+all_includes+'\nint main() { vec<int> a{1,2}; DEBUG(a); }\n'))
results.append(compile_one('all-headers-local','#define LOCAL\n'+preamble+all_includes+'\nint main() { vec<int> a{1,2}; DEBUG(a); }\n'))
data={'active_headers':[str(p.relative_to(ROOT)) for p in headers],
      'dependency_closure':[str(p.relative_to(ROOT)) for p in sorted(deps)],
      'results':results,'all_passed':all(x['exit_code']==0 for x in results),
      'source_sha256':{str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(deps)}}
(out/'summary.json').write_text(json.dumps(data,indent=2)+'\n')
print('Active headers:',len(headers),'dependency closure:',len(deps))
print('Compile checks:',sum(x['exit_code']==0 for x in results),'/',len(results))
for x in results:
    if x['exit_code']:print('FAIL',x['name'])
raise SystemExit(not data['all_passed'])
