# Compiladores 2026.1 - Grupo 7


# Sobre
Este projeto consiste no desenvolvimento de um compilador de C para Python, implementado em C.
Faz parte da disciplina de Compiladores I, pela Faculdade de Ciência, Tecnologia e Engenharia (FCTE) da Universidade de Brasília(UnB), pelos alunos do grupo 7.

## Visão Geral
Este repositório apresenta a implementação de um **compilador de C para Python**, desenvolvido em **C** no contexto da disciplina de **Compiladores** da **Universidade de Brasília (UnB)**.

A proposta do projeto é receber programas escritos em um subconjunto da linguagem **C**, realizar sua análise estrutural e gerar código equivalente em **Python**, simulando etapas fundamentais do funcionamento de um compilador.

### Recursos Suportados

- **Linguagem de entrada**: Código C com suporte a `int`, `if`, `else`, `while`, `comparações`, `operadores aritméticos`, `operadores lógicos`, `print`, `atribuição`, `declaração de variáveis`, `array` e `return`.

## Instalação e Execução

### Pré-requisitos

Para compilar e executar o projeto, é necessário ter instalado em um ambiente Linux ou WSL:

- `gcc`
- `make`
- `bison`
- `flex`
- `libfl-dev`

No Ubuntu/WSL, instale com:

```bash
sudo apt update
sudo apt install -y gcc make bison flex libfl-dev
```

### Compilação

A partir da raiz do projeto, execute:

```bash
cd src
make
```

Esse processo gera o executável do compilador.

### Execução dos testes
Após a compilação, execute os testes com:
```bash
cd ../tests
for f in *.c; do echo "Arquivo: $f"; ../src/parser.o < "$f"; echo "---"; done
```

Esse comando percorre todos os arquivos .c da pasta tests, executa o compilador para cada teste e exibe a saída correspondente.

# Equipe

Conheça o grupo deste projeto: 

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://github.com/GabrielMacielBR">
          <img src="https://avatars.githubusercontent.com/u/119356252?v=4" width="100px;" alt="Gabriel Maciel"/><br>
          <sub><b>Gabriel Maciel</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/libruna">
          <img src="https://avatars.githubusercontent.com/u/83987201?v=4" width="100px;" alt="Bruna Lima"/><br>
          <sub><b>Bruna Lima</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/CauaNicolas">
          <img src="https://avatars.githubusercontent.com/u/79241219?v=4" width="100px;" alt="Caua Nicolas"/><br>
          <sub><b>Caua Nicolas</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/Diaxiz">
          <img src="https://avatars.githubusercontent.com/u/88682217?v=4" width="100px;" alt="Diassis"/><br>
          <sub><b>Diassis</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/IsaqueSH">
          <img src="https://avatars.githubusercontent.com/u/101431986?v=4" width="100px;" alt="Isaque Santos"/><br>
          <sub><b>Isaque Santos</b></sub>
        </a>
      </td>
    </tr>
  </table>
</div>