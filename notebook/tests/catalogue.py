"""Resolve the actual printed selection, not all files in content/."""
from pathlib import Path
import re
ROOT=Path(__file__).resolve().parents[1]

def clean_tex(text):
    return '\n'.join(re.split(r'(?<!\\)%',line,1)[0] for line in text.splitlines())

def active_headers(root=ROOT):
    main=clean_tex((root/'content/kactl.tex').read_text())
    chapters=re.findall(r'\\kactlchapter\{([^}]+)\}',main)
    result=[]
    for chapter in chapters:
        text=clean_tex((root/f'content/{chapter}/chapter.tex').read_text())
        for name in re.findall(r'\\kactlimport(?:\[[^\]]*\])?\{([^}]+\.h)\}',text):
            p=root/'content'/chapter/name
            assert p.is_file(),p
            result.append(p)
    assert len(result)==len(set(result)), 'Duplicate active header'
    return result

def closure(paths):
    result=set()
    def visit(p):
        p=p.resolve()
        if p in result:return
        assert p.exists(),p
        result.add(p)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(),re.M):
            visit(p.parent/name)
    for p in paths:visit(p)
    return result
