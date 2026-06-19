# Compiladores 2026.1 - Grupo 7

## Sumário
- [Visão geral](#visão-geral)
- [Features suportadas](#features-suportadas)
- [Arquitetura e implementação](#arquitetura-e-implementação)
- [Exemplos de uso](#exemplos-de-uso)
- [Como executar o projeto](#como-executar-o-projeto)
- [Equipe](#equipe)

## Visão geral
Este repositório contém o desenvolvimento de um **compilador de C para Python**, implementado em C. O projeto foi desenvolvido como parte da disciplina Compiladores I, pela Faculdade de Ciência, Tecnologia e Engenharia (FCTE) da Universidade de Brasília (UnB), pelos alunos do Equipe 07.

A proposta do projeto é receber programas escritos em um subconjunto da linguagem **C**, realizar sua análise estrutural e gerar código equivalente em **Python**, simulando etapas fundamentais do funcionamento de um compilador: análise léxica, análise sintática, análise semântica, geração de código intermediário, otimização de código e geração de código final.

### Features suportadas
Subconjunto da linguagem de entrada C: O projeto atualmente suporta e analisa as seguintes estruturas:
- **Tipos e variáveis:** Declaração e atribuição de variáveis dos tipos `int` e `float`.
- **Operadores:** Operações aritméticas, lógicas e relacionais.
- **Controle de Fluxo:** Estruturas condicionais (`if` e `else`), laços de repetição (`for`, `while` e `do-while`) com suporte a controle de execução (`break` e `continue`).
- **Funções:** Declaração de funções, passagem de argumentos e instrução de retorno.

## Arquitetura e implementação

### Estrutura do repositório

A estrutura abaixo apresenta uma visão geral dos componentes centrais do projeto.

```text
.
├── README.md             # visão geral do projeto e instruções de uso
├── docs/                 # documentação de sprints e atas de reunião
├── src/                  # código-fonte principal 
│   ├── makefile          # script de compilação 
|   ├── ast.c             # AST e funções auxiliares para análise semântica, geração de código intermediário e código final
|   ├── parser.y          # análise sintática (Bison) e integração das demais etapas do compilador
|   ├── scanner.l         # regras léxicas (Flex)
|   ├── table.c           # tabela de símbolos
|   └── types.h           # definições de estruturas e tipos compartilhados
└── tests/                # casos de teste
```

### Decisões técnicas

- **Análise léxica**
- **Análise sintática**
- **Análise semântica**
- **Geração de código intermediário**
- **Otimização de código**
- **Geração de código final**

## Exemplos de uso

## Como executar o projeto

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

Conheça a equipe de desenvolvimento deste projeto:

<div align="center">
  <table>
    <tr>
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
        <a href="https://github.com/GabrielMacielBR">
          <img src="https://avatars.githubusercontent.com/u/119356252?v=4" width="100px;" alt="Gabriel Maciel"/><br>
          <sub><b>Gabriel Maciel</b></sub>
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