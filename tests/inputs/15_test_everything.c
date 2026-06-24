int ehPar(int n) {
    if (n % 2 == 0) {
        return 1;
    } else {
        return 0;
    }
}

int somaImpares(int limite) {
    int soma = 0;
    for (int i = 1; i <= limite; i++) {
        if (ehPar(i)) {
            continue;
        }
        soma = soma + i;
    }
    return soma;
}

int main() {
    int resultado = somaImpares(10);
    int x = 1;
    int y = 0;
    int valido = (resultado > 0) && !(x == y);
    return 0;
}