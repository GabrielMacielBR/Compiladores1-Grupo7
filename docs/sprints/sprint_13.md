# Sprint 13

**Período:** 08/06 a 14/06  
**Status:** ✅ Concluída  

## Objetivo
Definir e implementar a geração de código intermediário utilizando Three-Address Code (TAC).

## Atividades
- [x] Criar a estrutura de representação TAC.
- [x] Implementar geração de temporários e labels.
- [x] Gerar TAC para atribuições e operações aritméticas.
- [x] Gerar TAC para estruturas condicionais.
- [x] Gerar TAC para estruturas de repetição.
- [x] Gerar TAC para definição e chamada de funções.

## Resultados
- Foi criada a estrutura `TAC`, com campos para operador, argumentos, resultado e encadeamento das instruções.
- Foram implementadas funções para criação, inserção, impressão e liberação da lista de instruções TAC.
- Foram implementadas as funções `newTemp` e `newLabel`.
- Foram definidas as estruturas de TAC para atribuições, operações aritméticas, condicionais, repetições e funções.
- Chamadas de função passaram a gerar instruções relacionadas a parâmetros e chamada.
- Definições de função passaram a gerar marcadores de início e fim de função no TAC.

### Testes executados

## Dificuldades

## Observações
- A sprint concentrou a definição e implementação das estruturas de código intermediário do projeto.
