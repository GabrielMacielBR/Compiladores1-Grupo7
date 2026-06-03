/* TESTE: chamada com número errado de argumentos (deve gerar erro semântico) */
int sum(int a, int b) { return a + b; }
int t;
/* chamada com 3 argumentos em vez de 2 */
t = sum(1, 2, 3);
