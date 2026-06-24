def ehPar(n):
    if n % 2 == 0:
        return 1
    else:
        return 0

def somaImpares(limite):
    soma = 0
    i = 1
    while i <= limite:
        if ehPar(i):
            i += 1
            continue
        soma = soma + i
        i += 1
    return soma

def main():
    resultado = somaImpares(10)
    x = 1
    y = 0
    valido = (resultado > 0) and not (x == y)
    return 0