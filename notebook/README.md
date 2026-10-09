# Better Call Solve — MIUP Team Notebook

A referência da equipa **André Pinheiro, Ângelo Fernandes e Gonçalo Faria**.
Resultado da comparação dos notebooks de Gonçalo (H revisto final), André e Jet.

- `kactl.pdf`: **26 páginas A4 em paisagem: capa + 25 de conteúdo**, três colunas, tema de código a cores. Código e contratos a 7 pt; texto geral a 7,5 pt. É a escala compacta habitual do KACTL, escolhida para preservar cobertura.
- `content/`: **117 headers canónicos**, setup e referências matemáticas. Uma implementação principal por técnica; variantes apenas quando oferecem uma funcionalidade distinta.
- `REPORT.md`: resumo da seleção, alterações e validação.
- `reports/selection.md` / `selection.json`: correspondência de **todos os imports ativos** dos três inputs para a versão final ou uma exclusão justificada.
- `tests/`: baterias atuais e reproduzíveis. `validation/`: resultados efetivamente obtidos nesta entrega.
- `reserve/originals/`: os três ZIPs de entrada intactos. As implementações em reserva **não** seguem necessariamente a interface da equipa e **não** são validadas pelas baterias atuais.

## Usar o código

Copiar o template do capítulo Contest e depois o header pretendido. Os nomes de headers imediatamente acima de um bloco são as suas dependências: copiar também esses headers. Nas fontes, os `#include` relativos resolvem essa composição automaticamente.

Convenções: índices 0-based; intervalos de arrays `[l,r)`; prefixos `n+1`; `jmp[k][v]`; pai da raiz `-1`; `vec<T>` para vector, `rep(i,a,b)`, `pb`, `all`, `sz`. Geometria, limites de valores e representação interna das estruturas têm os contratos indicados no respetivo header. Não confundir pais `-1`, distâncias BFS `-1`, infinitos dos caminhos ponderados e `nullopt`.

Os headers pressupõem o preâmbulo do template e GNU C++17. `__int128`, PBDS e `<bits/stdc++.h>` são extensões GNU, disponíveis no ambiente habitual de competição. Cada implementação explica input/output, pré-condições, complexidade e os métodos que é preciso chamar. Para algoritmos recursivos que subsistem (p. ex. augmenting DFS de flow/matching), respeitar as limitações de stack documentadas. Assertivas verificam contratos, não substituem a sua leitura.

## Construir o PDF

Dependências: Python 3, GNU Make, GCC (o preprocessor usado nos hashes), e uma distribuição TeX com `pdflatex`, `extsizes`, Latin Modern e os pacotes importados por `content/tex/kactlpkg.sty` — normalmente `texlive-latex-extra`, `texlive-fonts-recommended` e `lmodern`.

```sh
make kactl
make verify-pdf
```

A construção usa `-shell-escape` para chamar o preprocessor local. O cache do tema está incluído: o conteúdo entregue reconstrói-se sem Node. Para alterar código e regenerar o realce, instalar Node.js; o lexer e as licenças já estão em `content/tex/vscode-theme/`. Se necessário: `NODE=/caminho/node make kactl`. Os ficheiros são gerados em `build/`; `make clean` remove apenas essa pasta.

`make verify-pdf` usa Poppler (`pdfinfo`, `pdftotext`) e confirma 26 páginas e limites da página. A revisão visual continua necessária após editar: renderizar com `pdftoppm -r 150 -png kactl.pdf page`.

## Testar

Dependências: Python 3, g++ com GNU C++17, sanitizers e headers Boost (Multiprecision/Rational para oráculos independentes de geometria; os headers canónicos não dependem de Boost; a nota opcional sobre cpp_int depende).

```sh
make test-compiles
make test
make test-usage
```

Para Boost instalado fora do caminho normal:

```sh
BOOST_INCLUDEDIR=/caminho/include make test
```

`make test` compila e executa todas as baterias em release e com ASan/UBSan + `_GLIBCXX_DEBUG`. Não se usa `-DNDEBUG`, pois os testes dependem de assertivas. Os testes têm seeds e limites fixos; usam enumeração, brute force, identidades e implementações independentes como oráculos. Os runners KACTL adaptados estão identificados no código e nos relatórios; não se reivindica a execução integral da suite histórica upstream.

```sh
# Um grupo; uma configuração; tempo máximo por execução:
python3 tests/run.py --only 'graph-*' --mode debug --timeout 180
```

Resultados e logs ficam em `validation/`; executáveis temporários em `build/`. O runner desativa LeakSanitizer porque este ambiente de validação usa ptrace. ASan e UBSan permanecem ativos; não houve validação de leaks com LSan. Fora deste ambiente é possível definir explicitamente `ASAN_OPTIONS=detect_leaks=1`.

A comparação original, autoria/licenças dos headers e fontes do tema foram preservadas. Os ZIPs de reserva são snapshots de proveniência, não recomendações para misturar interfaces antigas com as atuais.
