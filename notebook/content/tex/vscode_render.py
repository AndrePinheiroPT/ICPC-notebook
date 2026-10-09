#!/usr/bin/env python3
"""Presentation only: exact VS Code tokens and indentation guides for listings."""
import json
import hashlib
import os
from pathlib import Path
import re
import subprocess

HERE = Path(__file__).resolve().parent
OPEN, CLOSE = '(*@', '@*)'


def listing(source, language):
    if language == 'raw':
        return ('language=raw,numbers=none,frame=none,xleftmargin=0pt,'
                'xrightmargin=0pt', source)
    if language not in ('C++', 'bash'):
        return ('language=' + language, source)
    if OPEN in source or CLOSE in source:
        raise ValueError('Code contains the reserved presentation escape delimiter')
    request = json.dumps({'language': language, 'code': source})
    # Ship a source-keyed cache, so the supplied notebook builds without Node.
    # Editing code regenerates that entry with Node and the vendored lexer.
    key = hashlib.sha256(request.encode()).hexdigest()
    cache = HERE / 'vscode-theme' / 'cache' / (key + '.json')
    if cache.exists():
        data = json.loads(cache.read_text())
    else:
        result = subprocess.run(
            [os.environ.get('NODE', 'node'), str(HERE / 'vscode_tokens.cjs')],
            input=request, text=True, capture_output=True, check=True)
        data = json.loads(result.stdout)
        cache.parent.mkdir(exist_ok=True)
        cache.write_text(json.dumps(data, separators=(',', ':')))
    lines = source.split('\n')
    assert [line['text'] for line in data['lines']] == lines
    expanded = [line.expandtabs(2) for line in lines]
    indents = [len(s) - len(s.lstrip(' ')) for s in expanded]
    # KACTL uses tabs displayed at 2 spaces; space-only files use 2 or 4.
    nonzero = [n for s, n in zip(lines, indents) if s.strip() and n]
    step = 2 if '\t' in source or any(n % 4 == 2 for n in nonzero) else 4
    previous = [0] * len(lines)
    following = [0] * len(lines)
    last = 0
    for i, s in enumerate(lines):
        if s.strip(): last = indents[i]
        previous[i] = last
    last = 0
    for i in reversed(range(len(lines))):
        if lines[i].strip(): last = indents[i]
        following[i] = last
    output = []
    for i, line in enumerate(data['lines']):
        text = line['text']
        indent = indents[i] if text.strip() else min(previous[i], following[i])
        prefix = len(text) - len(text.lstrip(' \t'))
        # Keep source indentation before escapes so wrapped lines retain it.
        guides = ''.join(r'\VSGuide{%d}' % (n - indents[i])
                         for n in range(step, indent, step))
        parts = [text[:prefix], OPEN + r'\VSLine{%d}{%s}' % (indents[i], guides) + CLOSE]
        end = 0
        for span in line['spans']:
            assert span['start'] == end and span['end'] >= end
            color = span['color'].lstrip('#')[:6].upper()
            assert re.fullmatch('[0-9A-F]{6}', color)
            if span['end'] > prefix:
                parts.append(OPEN + r'\VS{%s}' % color + CLOSE)
                parts.append(text[max(prefix, span['start']):span['end']])
            end = span['end']
        assert end == len(text)
        output.append(''.join(parts))
    decorated = '\n'.join(output)
    assert re.sub(re.escape(OPEN) + r'.*?' + re.escape(CLOSE), '', decorated) == source
    return ('language=raw,vscode=true,escapeinside={(*@}{@*)}', decorated)
