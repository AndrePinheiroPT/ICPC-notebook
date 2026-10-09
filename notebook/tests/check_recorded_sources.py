#!/usr/bin/env python3
"""Check the delivered executable headers against the hashes recorded by tests.

Six final edits corrected only the opening documentation comment after testing.
Reconstruct their old full bytes and require the recorded SHA-256 to match;
never waive a code difference or silently refresh an old test result.
"""
from pathlib import Path
import hashlib,json
from catalogue import ROOT,active_headers
DOC_EDITS={
 'content/number-theory/ModSqrt.h':(r'(p-x)\%p.',r'(p-x)%p.'),
 'content/graph/Hierholzer.h':('With no edges returns one vertex src and no edge IDs.','With no edges returns ({src},{}).'),
 'content/graph/WeightedMatching.h':('Empty left side returns cost 0 and an empty matching.','Empty left side returns (0,{}).'),
 'content/numerical/Gauss.h':(r'a[i]=\{coefficients...,rhs\}',r'a[i]={coefficients...,rhs}'),
 'content/number-theory/Divisors.h':(r'return \{1\}.',r'return {1}.'),
 'content/number-theory/TotientMobius.h':('returns the pair (phi,mu)','returns {phi,mu}')
}
canonical={str(p.relative_to(ROOT)):p for p in active_headers()}
sha=lambda b:hashlib.sha256(b).hexdigest()
changes={};records=sorted((ROOT/'validation/team').glob('*.json'))
assert records
for record in records:
 data=json.loads(record.read_text());assert data['exit_code']==0,record
 for name,p in canonical.items():
  actual=p.read_bytes();old=data['source_sha256'][name]
  if sha(actual)==old:continue
  assert name in DOC_EDITS,(record.name,name,'unapproved source change')
  comment,separator,code=actual.decode().partition('*/')
  current,previous=DOC_EDITS[name]
  assert separator and current in comment,(record.name,name)
  reconstructed=(comment.replace(current,previous)+separator+code).encode()
  assert sha(reconstructed)==old,(record.name,name,'recorded version differs beyond documentation')
  changes[name]={'recorded_sha256':old,'final_sha256':sha(actual),'executable_part_sha256':sha(code.encode()),'change':'opening documentation comment only'}
result={'runtime_records':len(records),'canonical_headers':len(canonical),'all_passed':True,'documentation_only_adjustments':changes}
(ROOT/'validation/recorded-sources.json').write_text(json.dumps(result,indent=2)+'\n')
print(f'PASS: {len(records)} runtime records; all {len(canonical)} executable headers match tested code; {len(changes)} comment-only corrections verified')
