// arquivo gerado para profiling: múltiplas funções e laços
int add(int a, int b)
{
    int i;
    int s = 0;
    for (i = 0; i < 10; i = i + 1)
    {
        s = s + (a + b + i);
    }
    return s;
}

int main_var;

// repete muitos blocos para aumentar carga

int dummy()
{
    int x;
    x = 0;
    while (x < 50)
    {
        x = x + 1;
        add(x, x + 1);
    }
    return x;
}

// gerar muitos invocations
int _start_all()
{
    int k;
    k = 0;
    for (k = 0; k < 200; k = k + 1)
    {
        add(k, k + 2);
        dummy();
    }
    return k;
}

// série de expressões soltas
int _exprs()
{
    int i;
    i = 0;
    while (i < 1000)
    {
        int a;
        a = i + 2;
        a = a * (i + 3);
        i = i + 1;
    }
    return i;
}

// ponto de entrada sintético para o parser (arquivo apenas com declarações)
int x;
x = _start_all();
