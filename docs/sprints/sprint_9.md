# Sprint 9

**Período:** 18/05 a 24/05  
**Status:** Em andamento

## Objetivo
Implementar a geração da Abstract Syntax Tree (AST) e da Tabela de Símbolos para as estruturas já suportadas pelo analisador sintático. Além de iniciar o estudo sobre a etapa de otimização de código.

## Atividades / Issues
- [x] Implementar Abstract Syntax Tree e Tabela de Símbolos.
- [X] Estudo sobre otimização de código e Análise Semântica

## Resultados
### Tabela de Símbolos, Análise Semântica e Rastreamento de Erros
Nesta sprint, implementamos e aprimoramos significativamente a Tabela de Símbolos e as verificações semânticas do projeto:
1. **Estrutura de Tabela Hash**: Refatoramos a tabela de símbolos (`table.c` e `table.h`) para utilizar uma Tabela Hash com a função `djb2`, garantindo eficiência na busca e inserção. Colisões são tratadas com encadeamento (*separate chaining*).
2. **Análise Semântica Fatal**: Integramos a tabela de símbolos ao `parser.y` e adicionamos validações críticas. Casos como o uso de variáveis não declaradas ou redeclaração com tipo divergente emitem mensagens claras e forçam erro na compilação via `YYABORT`, impedindo a continuação de um código inválido.
3. **Validação e Inferência de Tipos**: Adicionamos recursividade e validação para capturar incompatibilidade de atribuições (ex: não permitir a expressão `float` de ir para atribuição e/ou declaração explícita de `int`), criando garantias estáticas de tipos.

### Testes executados


## Dificuldades

## Observações
