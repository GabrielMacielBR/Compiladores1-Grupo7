---
icon: lucide/arrow-big-up-dash
---

# Otimizações de TAC

Este documento resume, de forma breve, os pontos de melhoria identificados no projeto, as técnicas de otimização aplicadas sobre o TAC e os ganhos de desempenho observados nos testes realizados.

## 1. Objetivo

Aplicar otimizações simples e seguras sobre o TAC gerado pelo compilador para reduzir instruções redundantes, diminuir temporários e, quando possível, reduzir o trabalho das fases posteriores.

## 2. Pontos de melhoria identificados

- Geração de muitos temporários intermediários (tN) para expressões aritméticas.
- Avaliações de expressões constantes (ex.: 2 + 3) gerando instruções desnecessárias.
- Atribuições a temporários que não são usados posteriormente.
- Funções com muitos operadores aritméticos dentro de loops, gerando TAC repetitivo.

Observação: a análise inicial foi feita com `gprof` (instrumentation) e medidas por wall-clock com `result.py` / execuções repetidas. Para hotspots mais precisos recomenda‑se profiling por amostragem (`perf`) com stacks DWARF e flamegraphs.

## 3. Técnicas aplicadas

Implementadas em `src/ast.c` (função `optimizeTAC`) e expostas em `src/ast.h`:

- Constant Folding
  - Detecta operações binárias com operandos constantes inteiras e as substitui por uma atribuição direta do valor constante.
  - Cobertura: operações aritméticas básicas (+, -, *, /) e comparadores (==, !=, <, >, <=, >=).

- Constant Propagation
  - Propaga apenas definições do tipo `tN = CONST` para usos posteriores de `tN`.
  - Limitação: aplica-se somente a temporários (nomes que começam com `t`) e constantes inteiras.

- Dead Code Elimination 
  - Remove atribuições a temporários que não têm usos posteriores (passo de contagem de uso e reconstrução da lista TAC).

Integração: a fase roda apenas se a variável de ambiente `OPTIMIZE_TAC=1` estiver definida antes da execução do parser.

## 4. Contrato curto (inputs / outputs / garantia)

- Entrada: lista de instruções TAC gerada por `generateTAC(NodeAST *root)`.
- Saída: nova lista TAC otimizada (pode ter menos instruções e menos temporários). O output preserva a semântica observável do programa (valores de variáveis do usuário e chamadas de função não são removidas).
- Modos de erro: otimizações são conservadoras; se encontrar cenários não tratados, o código evita transformar instruções para não alterar semântica.

## 5. Ganhos de desempenho observados

Teste representativo usado: `tests/prof_input.c` (workload sintético com funções e muitos loops).

- Medição: 200 execuções do parser sobre `tests/prof_input.c` (comando realizado na raiz):

  - Sem otimização: total (wall clock) ≈ 0.64 s
  - Com `OPTIMIZE_TAC=1`: total (wall clock) ≈ 0.50 s

- Resultado: redução observada de ≈ 0.14 s no total para 200 execuções (aprox. 21.9% de melhora no tempo).


## 6. Como reproduzir (comandos)

Compilar:

```bash
cd src
make
cd ..
```

Rodar sem otimização:

```bash
./src/parser tests/prof_input.c < tests/prof_input.c
```

Rodar com otimização:

```bash
OPTIMIZE_TAC=1 ./src/parser tests/prof_input.c < tests/prof_input.c
```

Benchmark (exemplo de 200 repetições):

```bash
/usr/bin/time -f 'real %e' bash -c "for i in {1..200}; do ./src/parser tests/prof_input.c < tests/prof_input.c >/dev/null; done"
OPTIMIZE_TAC=1 /usr/bin/time -f 'real %e' bash -c "for i in {1..200}; do ./src/parser tests/prof_input.c < tests/prof_input.c >/dev/null; done"
```

Gerar gprof :

```bash
# recompile com -pg
cd src
gcc -c -g -O2 -pg parser.tab.c -o parser.o
gcc -c -g -O2 -pg lex.yy.c -o lex.yy.o
gcc -c -g -O2 -pg ast.c -o ast.o
gcc -c -g -O2 -pg table.c -o table.o
gcc -g -O2 -pg -o parser parser.o lex.yy.o ast.o table.o
cd ..
./src/parser tests/prof_input.c < tests/prof_input.c
gprof ./src/parser gmon.out > gprof.txt
```

Para profiling por amostragem e flamegraph :

```bash
perf record -F 99 -g -- ./src/parser tests/prof_input.c < tests/prof_input.c
perf script -i perf.data | ./FlameGraph/stackcollapse-perf.pl > out.folded
./FlameGraph/flamegraph.pl out.folded > flamegraph.svg
```

## 7. Limitações

- Limitações atuais:
  - Somente inteiro constants são propagadas.
  - Propagação limitada a temporários simples (`tN`).
  - Single-pass propagation; poderia ser multi-pass até fixpoint.
  - Nenhuma análise de escopo avançada (kills/overwrites) para propagação mais agressiva.

