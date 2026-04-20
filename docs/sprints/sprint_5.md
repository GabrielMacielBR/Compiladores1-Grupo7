# Sprint 5 — Finalização do Sintático

**Período:** 13/04 a 19/04  
**Status:** ⬜ Não iniciada  

## Objetivo
Finalizar o analisador sintático.

## Atividades / Issues
- [X] Corrigir erros do parser  
- [ ] Implementar tratamento de erros  
- [X] Validar gramática  

## Resultados
- Parser integrado com o analisador léxico e compilando com sucesso.
- Estrutura de expressões organizada por níveis de precedência:
	- `expr`
	- `expr_or`
	- `expr_and`
	- `expr_comp`
	- `expr_arit`
	- `expr_unaria`
	- `expr_primaria`
- Operadores comparativos implementados e validados:
	- `==`, `!=`, `<`, `<=`, `>`, `>=`
- Operadores lógicos implementados e validados:
	- `&&`, `||`, `!`
- Precedência validada em execução:
	- comparação com precedência maior que `&&`
	- `&&` com precedência maior que `||`
	- `!` no nível unário

### Testes executados
Entrada utilizada no parser:

```text
1 < 2 || 3 < 4 && 0;
1 < 2 && 3 < 4 || 0;
1 < 2 || 3 > 4 && 5 < 6;
(1 < 2 || 3 > 4) && 0;
!(1 < 2) || 0;
```

Saídas observadas:

```text
Resultado: 1
Resultado: 1
Resultado: 1
Resultado: 0
Resultado: 0
```

Conclusão dos testes:
- A precedência e associatividade dos operadores lógicos e comparativos estão consistentes com o esperado.

## Dificuldades
- Ajuste de ambiguidade inicial nas regras de expressão.
- Definição da melhor granularidade entre regras de expressão (estrutura mais didática vs. compacta).

## Observações
Dependente da conclusão da Sprint 4.
Tratamento de erros permanece parcialmente implementado: existe recuperação básica até `;`, mas os cenários de erro ainda não estão cobertos por completo.