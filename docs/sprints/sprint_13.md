# Sprint 13

**Período:** 08/06 a 14/06  
**Status:** ✅ Concluída  

## Objetivo
Implementar a geração de código intermediário utilizando Three-Address Code (TAC).

## Atividades
- [x] Criar a estrutura de representação TAC.
- [x] Gerar TAC para atribuições e operações aritméticas.
- [x] Gerar TAC para estruturas condicionais.
- [x] Gerar TAC para definição e chamada de funções.
- [x] Implementar geração de temporários e labels.
- [x] Avançar nas regras de análise semântica pendentes.
- [x] Implementar tratamento para indentação obrigatória do Python.

## Resultados
- Foi criada a estrutura `TAC`, com campos para operador, argumentos, resultado e encadeamento das instruções.
- Foram implementadas funções para criação, inserção, impressão e liberação da lista de instruções TAC.
- Foram implementadas as funções `newTemp` e `newLabel`.
- Foram adicionadas gerações de TAC para atribuições, operações aritméticas, condicionais e funções.
- Chamadas de função passaram a gerar instruções relacionadas a parâmetros e chamada.
- Definições de função passaram a gerar marcadores de início e fim de função no TAC.

### Testes executados

## Dificuldades

## Observações
- A sprint marcou a transição do projeto para a etapa de geração de código intermediário.
