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
- Suporte completo aos laços `while`, `for` e `do...while` com duas formas de corpo:
	- bloco com chaves (`{ ... }`)
	- instrução única sem chaves
- `for` validado com componentes opcionais:
	- init opcional
	- condição opcional
	- step opcional
	- forma `for (;;)` aceita

### Testes executados
Entrada utilizada no parser:

```text
1 < 2 || 3 < 4 && 0;
1 < 2 && 3 < 4 || 0;
1 < 2 || 3 > 4 && 5 < 6;
(1 < 2 || 3 > 4) && 0;
!(1 < 2) || 0;

int i = 0;
while (i < 2) i++;

int j;
for (j = 0; j < 2; j++) { j++; }

int k = 0;
do k++; while (k < 2);
```

Saídas observadas:

```text
Resultado: 1
Resultado: 1
Resultado: 1
Resultado: 0
Resultado: 0

INFO: Declaração de variável com inicialização detectada. Valor: 0
INFO: incremento posfixado detectado
INFO: Laço WHILE detectado

INFO: Declaração de variável detectada.
SUCESSO: Atribuição realizada. Resultado da expressão: 0
INFO: incremento posfixado detectado
INFO: incremento posfixado detectado
INFO: Bloco de codigo detectado
INFO: Laço FOR detectado

INFO: Declaração de variável com inicialização detectada. Valor: 0
INFO: incremento posfixado detectado
INFO: Laço DO...WHILE detectado
```

Conclusão dos testes:
- A precedência e associatividade dos operadores lógicos e comparativos estão consistentes com o esperado.
- Os três laços (`while`, `for`, `do...while`) foram reconhecidos corretamente, com corpo em bloco e/ou instrução única.

### Complemento — Testes extensivos de laços

Casos válidos executados e aprovados:
- `while` com bloco e com instrução única
- `for` com bloco e com instrução única
- `do...while` com bloco e com instrução única
- variações de `for`: sem init, sem condição, sem step e `for(;;)`
- `for` com init por declaração (`for (int i = 0; ... )`)
- step com `i++`, `++i` e `i--`
- laços aninhados em combinações mistas

Casos inválidos executados (com rejeição esperada):
- falta de `;` em declaração
- `for` sem `)`
- `do...while` sem `;` final
- corpo inválido em laço (`while (cond) ;`)

Comportamentos observados:
- recuperação sintática básica até `;` funcionando nos cenários cobertos
- checagem semântica de divisão por zero ativa (`[ERRO SEMANTICO] Divisao por zero!`)

## Dificuldades
- Ajuste de ambiguidade inicial nas regras de expressão.
- Definição da melhor granularidade entre regras de expressão (estrutura mais didática vs. compacta).

## Observações
Dependente da conclusão da Sprint 4.
Tratamento de erros permanece parcialmente implementado: existe recuperação básica até `;`, mas os cenários de erro ainda não estão cobertos por completo.