# Sprint 10

**Período:** 18/05 a 24/05  
**Status:** Em andamento

## Objetivo
Implementar a geração da Abstract Syntax Tree (AST) e da Tabela de Símbolos para as estruturas já suportadas pelo analisador sintático. Além de iniciar o estudo sobre a etapa de otimização de código.

## Atividades / Issues
- [x] Implementar Abstract Syntax Tree e Tabela de Símbolos.
- [x] Estudo sobre otimização de código e início de Estudos sobre máquinas abstratas e ambiente de tempo de execução

## Resultados
4. **Rastreamento Avançado de Localização (Linhas e Colunas)**: Incluímos uma macro no Flex (`YY_USER_ACTION`) em `scanner.l` responsável por rastrear a linha (`yyline`) e a coluna (`yycolumn`) corrente da avaliação. Esses dados agora são armazenados na Struct dos Símbolos e reportam o exato rastreamento visual das falhas (ex: `Erro semântico [L3:C6]: incompatibilidade de tipos...`).
5. **Suporte a Escopos (Scoping) e Shadowing**: Evoluímos a Tabela de Símbolos (`pushScope` e `popScope` em `table.c`) para aceitar múltiplos escopos. Blocos aninhados, como funções e estruturas de controle, operam de forma isolada. Variáveis em sub-escopos não interferem no escopo pai (shadowing) e são adequadamente liberadas (removidas da AST e memória) ao fim do bloco (`{ ... }`).
6. **Integração de Laços e Condicionais**: Concluímos a integração das instruções de desvio/condição (`if`, `else`, operador ternário) e repetição (`for`, `while`, `do..while`) diretamente no Parser e AST. Com a ajuda do gerenciamento de escopo (como nas variáveis declaradas no prólogo de um `for`), o *scope lifecycle* funciona corretamente.

### Testes executados


## Dificuldades

## Observações
