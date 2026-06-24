# Compiladores 2026.1 - Grupo 7

## Sumário
- [Visão geral](#visão-geral)
- [Features suportadas](#features-suportadas)
- [Arquitetura e implementação](#arquitetura-e-implementação)
- [Como executar o projeto](#como-executar-o-projeto)
- [Exemplos de uso](#exemplos-de-uso)
- [Equipe](#equipe)

## Visão geral
Este repositório contém o desenvolvimento de um **compilador de C para Python**, implementado em C. O projeto foi desenvolvido como parte da disciplina Compiladores I, pela Faculdade de Ciência, Tecnologia e Engenharia (FCTE) da Universidade de Brasília (UnB), pelos alunos do Equipe 07.

A proposta do projeto é receber programas escritos em um subconjunto da linguagem C, realizar sua análise estrutural e gerar código equivalente em Python, implementando as principais etapas de um compilador, incluindo análise léxica, análise sintática, análise semântica, geração de código intermediário, otimização de código e geração de código final.

Para o gerenciamento do projeto, foi adotada a metodologia ágil Scrum, com o desenvolvimento organizado em sprints semanais, totalizando 12 ciclos até a entrega da versão final. A distribuição das tarefas, o planejamento das atividades e o acompanhamento do progresso da equipe foram realizados por meio do GitHub Issues.

### Features suportadas
O projeto suporta o seguinte subconjunto da linguagem C:

- **Tipos e variáveis:** declaração e atribuição de variáveis dos tipos `int` e `float`.
- **Operadores aritméticos:** `+`, `-`, `*`, `/`, `++` e `--`.
- **Operadores relacionais:** `==`, `!=`, `<`, `>`, `<=` e `>=`.
- **Operadores lógicos:** `&&`, `||` e `!`.
- **Controle de fluxo:** estruturas condicionais (`if` e `else`) e laços de repetição (`for`, `while` e `do-while`), com suporte aos comandos `break` e `continue`.
- **Funções:** declaração de funções, passagem de parâmetros, chamadas de função e instrução de retorno (`return`).

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
|   ├── parser.y          # análise sintática e integração das demais etapas do compilador
|   ├── scanner.l         # regras léxicas
|   ├── table.c           # tabela de símbolos
|   └── types.h           # definições de estruturas e tipos compartilhados
└── tests/                # casos de teste
```

### Decisões técnicas

#### 1) Análise léxica
Implementada com o Flex, foram definidas expressões regulares para realizar a tokenização do código-fonte de entrada. O analisador reconhece palavras-chave, identificadores, constantes, operadores e símbolos da linguagem, além de ignorar espaços em branco e comentários.

#### 2) Análise sintática
Implementada com o Bison, responsável por verificar se a sequência de tokens produzida pelo analisador léxico está de acordo com a gramática definida para a linguagem. As regras sintáticas foram especificadas por meio de produções gramaticais, permitindo o reconhecimento das estruturas válidas do programa e a construção da Árvore Sintática Abstrata (AST).

#### 3) Modelagem da AST
Foi utilizada uma estrutura genérica de nós, representada pelo tipo `NodeAST`. Cada nó armazena seu tipo sintático por meio de um enumerador (`NodeType`), além de um conjunto de filhos e atributos auxiliares, como identificadores, operadores, valores literais e informações de tipo. Essa abordagem centraliza a representação das diferentes construções da linguagem em uma única estrutura de dados. As relações hierárquicas entre os elementos do programa são representadas por ponteiros para nós filhos.

#### 4) Modelagem da tabela de símbolos
Foi implementada por meio de uma tabela hash com encadeamento. Cada entrada armazena informações como nome, tipo, categoria do símbolo (variável ou função), escopo e posição da declaração no código-fonte. Para funções, também é mantida uma referência ao nó correspondente na AST, possibilitando validações posteriores.

O gerenciamento de escopos é realizado por meio de níveis numéricos, incrementados ao entrar em um novo bloco e decrementados ao sair dele. Durante a remoção de um escopo, todos os símbolos declarados naquele nível são removidos da tabela.

#### 5) Análise semântica
Implementada sobre a AST utilizando tabela de símbolos e gerenciamento de escopos. Foram desenvolvidas validações para declaração e redeclaração de variáveis, compatibilidade de tipos em expressões, atribuições e operações, validação de condicionais e estruturas de controle, verificação de chamadas de funções (declaração prévia, quantidade e compatibilidade de argumentos) e análise de retornos de funções (presença e compatibilidade de tipos).

#### 6) Geração de código intermediário
Implementada com a técnica de Three-Address Code (TAC). A partir da AST, as construções da linguagem são traduzidas para uma representação intermediária composta por instruções simples e variáveis temporárias.

#### 7) Otimização de código
Implementada sobre a representação intermediária em TAC, foram utilizadas as técnicas de Constant Folding, Constant Propagation e Dead Code Elimination. A descrição detalhada dessas otimizações e de seus respectivos resultados pode ser consultada [aqui](./docs/otimizacao.md).

#### 8) Geração de código final
Implementada a partir da AST. Embora o projeto possua uma representação intermediária em TAC, optou-se por gerar o código final diretamente da AST, uma vez que sua estrutura hierárquica preserva melhor as informações sintáticas e semânticas do programa, simplificando a tradução para a linguagem Python.

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

(Apenas em ambiente Linux/MacOS) Na pasta raiz do repositório, execute:
```bash
cd ./tests
./run_tests.sh
```
Esse comando percorre todos os arquivos .c da pasta tests, executa o compilador para cada teste e exibe a saída correspondente.

## Exemplos de uso

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