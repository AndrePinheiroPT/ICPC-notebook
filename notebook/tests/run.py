#!/usr/bin/env python3
"""Compile and run team batteries, recording exact source hashes and logs."""
from pathlib import Path
import argparse,datetime,hashlib,json,os,subprocess,time

ROOT=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser()
parser.add_argument('--only',default='*',help='Glob for test filename stem, e.g. geometry-*')
parser.add_argument('--mode',choices=['release','debug','both'],default='both')
parser.add_argument('--timeout',type=int,default=180)
args=parser.parse_args()
out=ROOT/'validation/team'
out.mkdir(parents=True,exist_ok=True)
(out/'logs').mkdir(exist_ok=True)
build=ROOT/'build/tests'
build.mkdir(parents=True,exist_ok=True)
env=dict(os.environ)
env.setdefault('ASAN_OPTIONS','detect_leaks=0')
boost=env.get('BOOST_INCLUDEDIR')
extra=['-isystem',boost] if boost else []
files=sorted((ROOT/'tests/team').glob(args.only+'.cpp'))
assert files, 'No matching test programs'
modes=['release','debug'] if args.mode=='both' else [args.mode]
failed=False
for p in files:
    for mode in modes:
        name=p.stem+'-'+mode
        exe=build/name
        flags=['-std=gnu++17','-Wall','-Wextra','-I',str(ROOT)]+extra
        if mode=='release': flags+=['-O2']
        else:flags+=['-O1','-g','-DLOCAL','-D_GLIBCXX_DEBUG','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']
        command=[env.get('CXX','g++'),*flags,str(p),'-o',str(exe)]
        hashes={str(q.relative_to(ROOT)):hashlib.sha256(q.read_bytes()).hexdigest()
                for folder in ['content','tests'] for q in sorted((ROOT/folder).rglob('*'))
                if q.is_file() and q.suffix in ['.h','.hpp','.cpp','.py','.tex']}
        started=time.monotonic()
        print('RUN',name,flush=True)
        error=None
        with (out/'logs'/f'{name}.log').open('w') as log:
            try:
                code=subprocess.run(command,cwd=ROOT,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=240).returncode
                if not code:
                    code=subprocess.run([str(exe)],cwd=ROOT,env=env,stdout=log,stderr=subprocess.STDOUT,timeout=args.timeout).returncode
            except subprocess.TimeoutExpired:
                code=124;error='timeout'
        record={'name':name,'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'exit_code':code,'seconds':round(time.monotonic()-started,2),'command':command,
                'error':error,'source_sha256':hashes}
        (out/f'{name}.json').write_text(json.dumps(record,indent=2)+'\n')
        print('PASS' if code==0 else 'FAIL',name,record['seconds'],'s',flush=True)
        failed |= code!=0
raise SystemExit(int(failed))
