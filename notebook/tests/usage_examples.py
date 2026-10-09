#!/usr/bin/env python3
"""Compile and execute the literal Usage blocks in the 40 DS/string/various headers.

Fixtures supply only the inputs referenced but not declared by the printed snippets.
The C++ snippets themselves are extracted afresh, never manually copied. Assertions
check the claimed results. Usage: python3 tests/usage_examples.py [--sanitize]
"""
from pathlib import Path
import argparse
import os
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent
# key -> (fixture before literal Usage, postconditions after literal Usage)
CASES = {
    'data-structures/ArrayBasics.h': ('', 'CHECK(x==5); CHECK(p==vec<ll>({0,4,2,9}));'),
    'data-structures/UnionFind.h': ('', 'CHECK(yes); CHECK(uf.size(0)==2);'),
    'data-structures/UnionFindRollback.h': (
        'int n=4,a=0,b=1;', 'CHECK(uf.time()==t); CHECK(uf.find(a)!=uf.find(b));'),
    'data-structures/DynamicConnectivity.h': (
        'DynamicConnectivity dc(3,3); dc.add(0,2,0,1); dc.add(1,3,1,2);\n'
        'vec<int> ans(3),u{0,0,0},v{1,2,2};', 'CHECK(ans==vec<int>({1,1,0}));'),
    'data-structures/Fenwick.h': ('', 'CHECK(i==1); CHECK(f.query(0,3)==7);'),
    'data-structures/Fenwick2D.h': (
        'vec<pair<ll,ll>> points{{1,2},{3,4}}; ll x=1,y=2,x1=0,y1=0,x2=2,y2=3;',
        'CHECK(sum==7); CHECK(f.query(0LL,0LL,5LL,5LL)==7);'),
    'data-structures/SegmentTree.h': ('', 'CHECK(x==7);'),
    'data-structures/LazySegTree.h': ('', 'CHECK(total==17); CHECK(st.queryMax(0,3)==7);'),
    'data-structures/SparseLazySegTree.h': ('', 'CHECK(best==3); CHECK(st.query(30,40)==0);'),
    'data-structures/RMQ.h': ('', 'CHECK(x==1);'),
    'data-structures/SqrtDecomposition.h': ('', 'CHECK(k==2); CHECK(d.countLE(0,3,0)==1);'),
    'data-structures/PrefixSum2D.h': ('', 'CHECK(x==6);'),
    'data-structures/Difference2D.h': ('', 'CHECK(a==vec<vec<ll>>({{0,5,5},{0,5,5}}));'),
    'data-structures/OrderedSet.h': ('', 'CHECK(it!=s.end()); CHECK(*it==3);'),
    'data-structures/HashMap.h': ('', 'CHECK(freq[-7]==1); CHECK(freq.size()==1);'),
    'data-structures/Treap.h': ('', 'CHECK(a==vec<ll>({7,3}));'),
    'strings/KMP.h': ('', 'CHECK(at==vec<int>({0,2}));'),
    'strings/ZFunction.h': ('', 'CHECK(z==vec<int>({0,3,2,1}));'),
    'strings/DoubleHash.h': ('', 'CHECK(same);'),
    'strings/AlphabetTrie.h': ('', 'CHECK(copies==2); CHECK(t.countPrefix("ca")==2);'),
    'strings/Aho.h': (
        'string text="ababa"; ostringstream log; auto old=cout.rdbuf(log.rdbuf());',
        'cout.rdbuf(old); CHECK(total==2); CHECK(log.str()=="0 ");'),
    'strings/Suffixes.h': (
        'string s="banana";',
        'CHECK(a.sa==vec<int>({5,3,1,0,4,2}));\n'
        'CHECK(rmq.query(min(a.rank[1],a.rank[3]),max(a.rank[1],a.rank[3]))==3);'),
    'strings/Palindromes.h': (
        'string s="abba";', 'CHECK(odd==vec<int>({1,1,1,1})); CHECK(even==vec<int>({0,0,2,0}));'),
    'strings/LeastRotation.h': ('string s="baba";', 'CHECK(start==1);'),
    'various/CoordinateCompression.h': ('', 'CHECK(values==vec<int>({10,50})); CHECK(rank==vec<int>({1,0,1}));'),
    'various/TwoPointers.h': ('', 'CHECK(l==0 && r==3);'),
    'various/MonotonicStack.h': ('', 'CHECK(left==vec<int>({-1,-1,1,1}));'),
    'various/MonotonicDeque.h': ('', 'CHECK(at==vec<int>({1,3,3}));'),
    'various/Subsequence.h': ('', 'CHECK(sz(ids)==3); CHECK(ids==vec<int>({1,2,4}) || ids==vec<int>({1,3,4}));'),
    'various/ClassicDP.h': ('', 'CHECK(linear==6 && circular==11); CHECK(best==9);'),
    'various/SubsetBits.h': ('', 'CHECK(possible[10]); CHECK(possible.count()==4);'),
    'various/SOS.h': ('', 'CHECK(a==vec<ll>({1,3,4,10}));'),
    'various/DigitDP.h': ('', 'CHECK(count==7);'),
    'various/DivideAndConquerDP.h': (
        'int n=3; vec<ll> prefix{0,1,3,6};',
        'CHECK(cur==vec<ll>({INFLL,1,9,36}));'),
    'various/XorBasis.h': ('', 'CHECK(yes); CHECK(best==6); CHECK(b.rank==2);'),
    'various/CoverIntervals.h': ('', 'CHECK(ids.has_value()); CHECK(*ids==vec<int>({0,1}));'),
    'various/Histogram.h': ('', 'CHECK(area==8 && l==2 && r==4);'),
    'various/FunctionalCycle.h': ('vec<int> next{1,2,3,1};', 'CHECK(mu==1 && len==3);'),
    'various/LineHull.h': ('', 'CHECK(best==9);'),
    'various/GregorianDays.h': ('', 'CHECK(days==2);'),
}


def usage(path):
    text = path.read_text().split('*/', 1)[0]
    lines = []
    active = False
    for raw in text.splitlines():
        value = re.sub(r'^\s*\* ?', '', raw)
        if value.startswith('Usage: '):
            active = True
            lines.append(value[len('Usage: '):])
        elif active:
            if re.match(r'\w+:', value):
                break
            lines.append(value)
    if not active:
        raise RuntimeError(f'No Usage block in {path}')
    return '\n'.join(lines).rstrip()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    imported = []
    for folder in ('data-structures', 'strings', 'various'):
        chapter = (ROOT / 'content' / folder / 'chapter.tex').read_text()
        for line in chapter.splitlines():
            line = line.split('%', 1)[0]
            for name in re.findall(r'\\kactlimport\{([^}]+)\}', line):
                imported.append(f'{folder}/{name}')
    if set(imported) != set(CASES):
        raise RuntimeError(f'Usage coverage drift: missing={set(imported)-set(CASES)}, '
                           f'stale={set(CASES)-set(imported)}')
    source = ['#include "tests/team/prelude.hpp"']
    source += [f'#include "content/{name}"' for name in imported]
    source += ['int checks=0;',
               '#define CHECK(x) do { ++checks; if(!(x)) { '
               'cerr << "Usage failure line " << __LINE__ << ": " << #x << endl; abort(); }} while(0)',
               'int main() {']
    for name in imported:
        before, after = CASES[name]
        source += ['{', f'// Literal Usage: {name}', before,
                   usage(ROOT / 'content' / name), after, '}']
    source += [f'cout << "literal Usage: {len(imported)} headers, " << checks << " checks passed\\n";', '}']
    with tempfile.TemporaryDirectory(prefix='team-usage-') as directory:
        cpp, exe = Path(directory) / 'usage.cpp', Path(directory) / 'usage'
        cpp.write_text('\n'.join(source)+'\n')
        flags = ['-O2']
        if args.sanitize:
            flags = ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                     '-D_GLIBCXX_ASSERTIONS']
        command = [os.environ.get('CXX','g++'), '-std=c++17', '-Wall', '-Wextra', *flags,
                   '-I', str(ROOT), str(cpp), '-o', str(exe)]
        subprocess.run(command, check=True, timeout=90)
        env = dict(os.environ)
        if args.sanitize:
            env['ASAN_OPTIONS'] = 'detect_leaks=0'  # LSan is unavailable under host ptrace.
        subprocess.run([str(exe)], check=True, timeout=30, env=env)


if __name__ == '__main__':
    main()
