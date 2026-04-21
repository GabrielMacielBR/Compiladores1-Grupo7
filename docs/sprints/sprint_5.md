# Sprint 5

**Período:** 13/04 a 19/04  
**Status:** ✅ Concluída  

## Objetivo
Implementação do analisador sintático para suporte a: atribuição, declaração, condicional, laços e funções.

## Atividades / Issues
- [x] Corrigir erros de integração do parser com o analisar léxico  
- [x] Validar gramática

## Resultados
- Integração dos analisadores léxico e sintático para: atribuição, declaração e laços.
- Regras sintáticas básicas implementadas para:
	- declaração de variáveis (`int` e `float`)
	- atribuição
- Integração inicial com o analisador léxico concluída (scanner e parser compilando no fluxo do `make`).
- Tratamento sintático inicial de erros implementado com recuperação até `;`.

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
int x;
float y;
x = 10 + 5;
20;

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
Digite expressoes terminadas com ';'. Pressione Ctrl+D para encerrar.
INFO: Declaração de variável detectada.
INFO: Declaração de variável float detectada.
SUCESSO: Atribuição realizada. Resultado da expressão: 15
Resultado: 20

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
- Depuração de erros sintáticos durante integração.
- Definição da melhor granularidade entre regras de expressão (estrutura mais didática vs. compacta).

## Observações
Dependente da conclusão da Sprint 4.
Tratamento de erros permanece parcialmente implementado: existe recuperação básica até `;`, mas os cenários de erro ainda não estão cobertos por completo.