# Sprint 11

**Período:** 25/05 a 31/05  
**Status:** Em andamento

## Objetivo
Implementar a geração da Abstract Syntax Tree (AST) e da Tabela de Símbolos para as estruturas já suportadas pelo analisador sintático. Além de iniciar o estudo sobre a etapa de otimização de código.

## Atividades / Issues
- [x] Implementar Abstract Syntax Tree e Tabela de Símbolos.
- [x] Estudo sobre Projeto de compiladores e parsers na ESW 

## Resultados
- Consolidação da Tabela de Símbolos com hash, controle de escopos, busca de identificadores e tratamento de conflitos de declaração.
- Evolução da análise semântica para detectar símbolos não declarados e já declarados, com melhorias nas mensagens de erro usando linha e coluna.
- Integração da inferência de tipos à AST e adaptação da estrutura para suportar laços e outras construções já reconhecidas pelo parser.
- Refatorações no parser e na AST para reduzir conflitos de integração e manter a base do compilador pronta para as próximas etapas.
- Ajustes de documentação relacionados ao scoping e à integração entre laços e condicionais.



### Testes executados
- Não há registro formal de uma bateria de testes automatizada nesta sprint.
- A validação ocorreu principalmente por integração incremental das mudanças no parser, AST e Tabela de Símbolos.


## Dificuldades
- Resolução de conflitos durante a integração entre parser, AST e Tabela de Símbolos.
- Ajustes finos para manter a validação de tipos consistente com as novas verificações semânticas.
- Organização das mensagens de erro para exibir linha e coluna corretamente.

## Observações
- A sprint 11 concentrou a base estrutural do compilador para a etapa semântica, deixando o projeto mais preparado para evoluções futuras como otimização de código.
