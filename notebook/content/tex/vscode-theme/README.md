# VS Code presentation assets

These files reproduce the installed VS Code **Light (Visual Studio)** token
palette using its C++/Bash TextMate grammars, `vscode-textmate`, and
`vscode-oniguruma`. They affect presentation only; algorithm files remain intact.

`../vscode_tokens.cjs` reads JSON on stdin and writes JSON to stdout. It needs
Node.js 18+; the WASM file and all JavaScript dependencies are vendored here, so
neither VS Code, npm nor network access is required to rebuild. The caller may
select a Node binary with the `NODE` environment variable. A missing dependency or invalid input is an
explicit fatal error; there is no silent approximate highlighting fallback.

The Python presentation layer includes a source-keyed `cache/` for every current
notebook snippet. Rebuilding the supplied notebook therefore needs only its
original Python/LaTeX tools. Node is needed when a snippet changes and its colors
must be regenerated, not for rebuilding an unchanged notebook. The cached text
is checked against the source before use. Source and rendered code are compared
again after removing the invisible formatting escapes.

Input is `{language:"C++"|"bash"|"raw",code:"...",includeScopes:false}` or a batch
`{items:[...]}`. Output contains the exact input lines, colored spans, and C++
bracket positions. C++ and Bash share the registered pairs `()[]{}`.
All span offsets are Unicode code point offsets (Python
string indices), not UTF-16 or byte offsets. Splitting preserves a trailing
empty line. `includeScopes:true` adds TextMate scopes to spans for debugging.

The source theme supplies black text, blue keywords/directives, green comments,
red-brown strings, and green numeric literals. No bold or italic formatting is
added to C++ tokens. Bracket pairs `()[]{}` share a nesting counter, matching
VS Code's default `independentColorPoolPerBracketType:false`; comments and string
literals are excluded. The depth colors are the VS Code Light defaults
`#0431FA`, `#319331`, `#7B3814`. Angle brackets are not bracket-colored because the
stock C++ language configuration does not register them as bracket pairs.
Indent guides use theme `#D3D3D3`; line-number default is `#237893`.

The original theme enables semantic highlighting. This static renderer applies
the exact TextMate rules without starting a project-aware C++ language server.
Cursor-dependent active-line, selected-text and active-guide states do not apply
to a printed document. Optional third-party assembly/regex grammar embeddings
are absent, matching a stock install without those extensions; ordinary C++ and
all notebook algorithms use the bundled C++ grammars.

Assets were copied from the local VS Code distribution; versions and SHA-256
hashes are in `source-manifest.json`. Microsoft assets use the MIT license in
`VSCODE-LICENSE.txt`; the TextMate and Oniguruma packages carry their licenses,
and grammar authors' notices are in `GRAMMAR-LICENSES.txt`.
