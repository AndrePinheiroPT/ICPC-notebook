#!/usr/bin/env python3
"""Structural PDF checks; final visual review is still required."""
from pathlib import Path
import argparse,hashlib,json,re,subprocess,xml.etree.ElementTree as ET
ap=argparse.ArgumentParser()
ap.add_argument('pdf',nargs='?',default='kactl.pdf')
ap.add_argument('--pages',type=int,default=26)
a=ap.parse_args();p=Path(a.pdf).resolve()
info=subprocess.check_output(['pdfinfo',str(p)],text=True)
pages=int(re.search(r'^Pages:\s+(\d+)',info,re.M).group(1))
assert pages==a.pages,(pages,a.pages)
xml=subprocess.check_output(['pdftotext','-bbox-layout',str(p),'-']).decode()
# Poppler can emit XML-illegal control codes for TeX math glyphs.
xml=re.sub(r'[\x00-\x08\x0b\x0c\x0e-\x1f]','',xml)
root=ET.fromstring(xml);ns={'h':'http://www.w3.org/1999/xhtml'}
texts=[];out=[]
for i,page in enumerate(root.findall('.//h:page',ns)):
    w,h=float(page.attrib['width']),float(page.attrib['height'])
    words=page.findall('.//h:word',ns)
    assert words, f'Blank page {i+1}'
    for word in words:
        x1,y1,x2,y2=(float(word.attrib[k]) for k in ['xMin','yMin','xMax','yMax'])
        assert x1>=-0.5 and y1>=-0.5 and x2<=w+0.5 and y2<=h+0.5,(i+1,word.text,word.attrib)
    texts.append(' '.join(x.text or '' for x in words))
    out.append({'physical_page':i+1,'words':len(words),'width_pt':w,'height_pt':h})
assert 'Better Call Solve' in texts[0], 'Missing team cover'
assert all('UMinho' in t for t in texts[1:]), 'Missing page header'
assert not any(token in ' '.join(texts) for token in ['Could not read source','Missing command:','Unknown command:'])
assert 'Guia de complexidade' in texts[-1]
result={'physical_pages':pages,'content_pages':pages-1,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),
        'pages':out,'page_bounds_passed':True,'visual_review':'separate manual review'}
folder=Path(__file__).resolve().parents[1]/'validation';folder.mkdir(exist_ok=True)
(folder/'pdf.json').write_text(json.dumps(result,indent=2)+'\n')
print(f'PASS: {pages} pages = cover + {pages-1} content; all text inside page bounds')
