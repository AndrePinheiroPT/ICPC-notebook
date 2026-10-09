#!/usr/bin/env node
/* Presentation-only lexer: vendored VS Code TextMate + Light (Visual Studio).
 * stdin: {language:'C++'|'bash'|'raw',code:string,includeScopes?:boolean}
 * or {items:[...]}; stdout: JSON; offsets are Unicode code points, not bytes.
 */
'use strict';
const fs = require('fs');
const path = require('path');
const vendor = path.join(__dirname, 'vscode-theme');
const tm = require(path.join(vendor, 'vscode-textmate/release/main.js'));
const onig = require(path.join(vendor, 'vscode-oniguruma/release/main.js'));
const theme = JSON.parse(fs.readFileSync(path.join(vendor, 'light_vs.json'), 'utf8'));
const BRACKETS = ['#0431FA', '#319331', '#7B3814'];
const files = {
  'source.cpp': 'cpp.tmLanguage.json',
  'source.cpp.embedded.macro': 'cpp.embedded.macro.tmLanguage.json',
  'source.shell': 'shell-unix-bash.tmLanguage.json',
};
const languages = {'C++': 'source.cpp', cpp: 'source.cpp', bash: 'source.shell'};
const opening = '{[(', closing = '}])';

async function main() {
  const input = JSON.parse(fs.readFileSync(0, 'utf8'));
  const wasm = fs.readFileSync(path.join(vendor, 'vscode-oniguruma/release/onig.wasm'));
  await onig.loadWASM(wasm.buffer.slice(wasm.byteOffset, wasm.byteOffset + wasm.byteLength));
  const registry = new tm.Registry({
    theme: {settings: [{settings: {foreground: '#000000', background: '#FFFFFF'}}, ...theme.tokenColors]},
    onigLib: Promise.resolve({
      createOnigScanner: patterns => new onig.OnigScanner(patterns),
      createOnigString: str => new onig.OnigString(str),
    }),
    loadGrammar: async scope => {
      if (!files[scope]) return null; // Optional external assembly/regex embeddings, as in stock VS Code.
      const filename = path.join(vendor, files[scope]);
      return tm.parseRawGrammar(fs.readFileSync(filename, 'utf8'), filename);
    },
  });
  const grammarCache = new Map();
  async function tokenize(item) {
    if (!item || typeof item.code !== 'string') throw new Error('Expected code:string');
    const language = item.language || 'C++';
    if (language !== 'raw' && !languages[language]) throw new Error('Unsupported language: ' + language);
    const sourceLines = item.code.split('\n');
    if (language === 'raw') return {language, lines: sourceLines.map(text => ({
      text, spans: text ? [{start: 0, end: Array.from(text).length, color: '#000000'}] : [], brackets: [],
    }))};
    const scope = languages[language];
    if (!grammarCache.has(scope)) grammarCache.set(scope, registry.loadGrammar(scope));
    const grammar = await grammarCache.get(scope);
    if (!grammar) throw new Error('Cannot load grammar ' + scope);
    const colors = registry.getColorMap();
    const stack = [];
    let state = tm.INITIAL;
    const lines = [];
    for (const text of sourceLines) {
      const colored = grammar.tokenizeLine2(text, state);
      const scoped = item.includeScopes ? grammar.tokenizeLine(text, state) : null;
      state = colored.ruleStack;
      // TextMate indices use UTF-16; Python presentation code uses Unicode code points.
      const points = Array.from(text), charColors = [], charTypes = [], charScopes = [];
      let utf16 = 0, token = 0, scopeToken = 0;
      for (let i = 0; i < points.length; ++i) {
        while (token + 2 < colored.tokens.length && colored.tokens[token + 2] <= utf16) token += 2;
        const metadata = colored.tokens[token + 1];
        charColors.push(colors[(metadata >>> 15) & 511].toUpperCase());
        charTypes.push((metadata >>> 8) & 3);
        if (scoped) {
          while (scopeToken + 1 < scoped.tokens.length && scoped.tokens[scopeToken + 1].startIndex <= utf16) ++scopeToken;
          charScopes.push(scoped.tokens[scopeToken].scopes);
        }
        utf16 += points[i].length;
      }
      const brackets = [];
      // Stock C++ and Bash both register {}, [], (). Angles stay token-colored.
      for (let i = 0; i < points.length; ++i) {
        if (charTypes[i] !== 0) continue;
        const ch = points[i];
        let depth;
        if (opening.includes(ch)) {
          depth = stack.length;
          stack.push(ch);
        } else if (closing.includes(ch)) {
          if (stack.length && opening.indexOf(stack[stack.length - 1]) === closing.indexOf(ch)) {
            stack.pop(); depth = stack.length;
          } else {
            // VS Code's light unexpected-bracket #FF1212CC over white.
            charColors[i] = '#FF4141';
            brackets.push({start: i, end: i + 1, char: ch, depth: -1, color: charColors[i]});
            continue;
          }
        } else continue;
        charColors[i] = BRACKETS[depth % BRACKETS.length];
        brackets.push({start: i, end: i + 1, char: ch, depth, color: charColors[i]});
      }
      const spans = [];
      for (let start = 0; start < points.length;) {
        let end = start + 1;
        while (end < points.length && charColors[end] === charColors[start] &&
          (!scoped || JSON.stringify(charScopes[end]) === JSON.stringify(charScopes[start]))) ++end;
        const span = {start, end, color: charColors[start]};
        if (scoped) span.scopes = charScopes[start];
        spans.push(span); start = end;
      }
      lines.push({text, spans, brackets});
    }
    return {language, lines};
  }
  let result;
  if (Array.isArray(input.items)) {
    result = {items: []};
    for (const item of input.items) result.items.push(await tokenize(item));
  } else result = await tokenize(input);
  process.stdout.write(JSON.stringify(result));
  registry.dispose();
}
main().catch(err => {
  process.stderr.write('VS Code highlighting failed: ' + (err.stack || err) + '\n');
  process.exitCode = 1;
});
