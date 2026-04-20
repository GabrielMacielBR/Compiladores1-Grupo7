# Sprint 4 — Analisador Sintático

**Período:** 06/04 a 12/04  
**Status:** Em andamento  

## Objetivo
Implementar o analisador sintático.

## Atividades / Issues
- [x] Implementar parser  
- [x] Integrar com léxico  
- [x] Testes sintáticos  

## Resultados
- Estrutura inicial do parser implementada com regras de programa/comando.
- Regras sintáticas básicas implementadas para:
	- declaração de variáveis (`int` e `float`)
	- atribuição
	- expressões aritméticas com `+`, `-`, `*`, `/` e parênteses
- Integração inicial com o analisador léxico concluída (scanner e parser compilando no fluxo do `make`).
- Tratamento sintático inicial de erros implementado com recuperação até `;`.

### Testes sintáticos básicos
Entradas utilizadas:

```text
int x;
float y;
x = 10 + 5;
20;
```

Saídas observadas:

```text
Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.
INFO: Declaração de variável detectada.
INFO: Declaração de variável float detectada.
SUCESSO: Atribuição realizada. Resultado da expressão: 15
Resultado: 20
```

## Dificuldades
- Sincronização inicial de tokens entre scanner e parser.
- Ajuste da estrutura das regras para reduzir ambiguidades nas expressões.
- Depuração de erros sintáticos durante integração.

## Observações
Sprint concluída com a base do sintático funcionando.
Implementações de comparações (`==`, `!=`, `<`, `<=`, `>`, `>=`) e operadores lógicos (`&&`, `||`, `!`) foram movidas para a Sprint 5.