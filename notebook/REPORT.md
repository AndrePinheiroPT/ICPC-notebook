# Better Call Solve — relatório da fusão

**Entrega:** enciclopédia MIUP com **117 headers canónicos**, setup, fórmulas e referências gerais; **26 páginas A4 em paisagem: capa + 25 de conteúdo**. Mantém o tema a cores do H, três colunas, código/contratos a **7 pt** e texto geral a **7,5 pt**. Capa com a equipa e os três membros.

## Seleção e implementação

Foram mapeados os **263 imports ativos** dos três inputs: 65 headers de Gonçalo, 87 de Jet e 100 de André, mais 11 imports de setup/checklists. A união elimina interfaces duplicadas e preserva variantes com utilidade diferente. O mapa completo e as razões estão em `reports/selection.md` e `selection.json`.

| Área | Resultado principal |
|---|---|
| Convenções | GNU C++17, `vec<T>`, `rep(i,a,b)`, `pb`, 0-based, `[l,r)`, prefixos `n+1`, `jmp[k][v]`, pai da raiz `-1`. Dependências e sentinelas explícitas. |
| Grafos | Bellman–Ford conservado; Kosaraju iterativo; distâncias/pais e múltiplas origens; Dinic, min-cost flow, matching/vertex cover, Hungarian, stable matching com lados desiguais; HLD, virtual tree e centroid decomposition. |
| Estruturas | Fenwick 1D/2D, rollback/conectividade offline, RMQ; lazy densa com add/set e sum/max; lazy sparse para domínios grandes; implicit treap, OST e hash map. Tags da lazy sparse alargadas para evitar overflow em adições que se cancelam. |
| Matemática | Pesquisa inteira `[l,r)` com `optional` e midpoint seguro; matrizes por vetores; Gauss real/modular/binário; determinantes, inversa, recorrências, interpolação, NTT/FFT; CRT, primalidade/fatorização 64-bit, log/root modular, floor sums, phi/Möbius. |
| Strings e DP | Hash simples com comprimento, trie dinâmico com duplicados, Aho, suffix array/LCP, Manacher; subset sum em bitset, SOS, DP clássica, digit DP, D&C DP e XOR basis. |
| Geometria | Interface Point/P2 comum; predicados, interseções, polígonos/hulls/círculos, closest pair, união de retângulos; tangentes, área círculo-polígono, centroide e Manhattan MST. |

Cada header explica uso, input/output, condições, complexidade e métodos públicos. Exemplos extensos e páginas de estratégia foram substituídos por contratos e pequenos exemplos canónicos. As explicações são maioritariamente em inglês, com notas PT-PT.

## Exclusões explícitas

Ficam em reserva, sem fingir equivalência funcional: Link-Cut Tree, Directed MST, suffix tree, Angle/similarity transform, sieve comprimido, continued fractions/rational search, helpers de polinómios e o solver de raízes problemático, FFT para módulo arbitrário e ranking de permutações. Golden search cede lugar à ternary search, que pode avaliar mais vezes a função. O relatório detalhado identifica ainda fórmulas e wrappers de exercícios retirados.

Os três **ZIPs originais intactos** estão em `reserve/originals/`, incluindo técnicas inativas e alternativas. A reserva não é apresentada como código uniformizado ou validado. Não foram repetidos automaticamente os antigos cortes orientados ao TST.

## Validação efetivamente executada

- **119/119 verificações de compilação:** 117 headers isolados com as suas dependências, todos juntos e todos juntos com `LOCAL`; `-Wall -Wextra -Werror`.
- **20 baterias × 2 modos = 40 execuções aprovadas**, release e ASan/UBSan + STL debug. Incluem testes KACTL adaptados, enumeração/brute force, testes aleatórios reprodutíveis, limites numéricos, casos vazios e integração entre algoritmos. Todos os 117 headers estão incluídos em testes de execução; a cobertura por cenário está descrita nos relatórios por área.
- **98 exemplos de Usage** compilados/executados: 40 DS/strings/DP, 24 grafos, 34 matemática; resultados conferidos por assertivas. Inclui a correção do exemplo HLD para o máximo da segment tree predefinida.
- **PDF compilado e todas as 26 páginas revistas visualmente**, sem cortes/sobreposições detetados. Corrigidos símbolos escapados e a condição matemática da fórmula de raízes características; paginação e limites do texto verificados automaticamente.
- Reconstrução a partir da pasta limpa da entrega, incluindo o cache de cores, **sem depender de Node**. Três passagens LaTeX estabilizam também o índice.
- Os logs e hashes estão em `validation/`. `tests/check_recorded_sources.py` verifica que seis ajustes finais nos comentários não alteraram o código executável testado. A compilação final cobre os headers já com esses ajustes.

**Limites:** os testes são finitos, não uma prova formal. LeakSanitizer foi desativado por incompatibilidade com ptrace; ASan/UBSan mantiveram-se ativos. A reserva e a suite histórica upstream completa não são reivindicadas como validadas. As condições numéricas, de stack e de precisão continuam documentadas em cada implementação.

Para repetir: `make test-compiles`, `make test`, `make test-usage`; construir com `make kactl` e verificar com `make verify-pdf`. Dependências e opções em `README.md`.
