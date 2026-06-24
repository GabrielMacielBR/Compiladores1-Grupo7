# Exemplos de Uso

Esta página apresenta alguns exemplos de programas escritos em C e o código Python gerado pelo compilador.

## Exemplo 1: Operações aritméticas

### Código de entrada (C)

```c
int main() {
    int a = 10;
    int b = 20;
    int soma = a + b;

    return soma;
}
```

### Código gerado (Python)

```python
def main():
    a = 10
    b = 20
    soma = a + b

    return soma

main()
```

## Exemplo 2: Estrutura condicional

### Código de entrada (C)

```c
int main() {
    int x = 5;

    if (x > 0) {
        x = x + 1;
    }

    return x;
}
```

### Código gerado (Python)

```python
def main():
    x = 5

    if x > 0:
        x = x + 1

    return x

main()
```