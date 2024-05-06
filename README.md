# PL0 Compiler

## Overview

This is a simple PL0 compiler implemented in C. PL0 is a simple programming language used for educational purposes. The compiler consists of a lexical analyzer that tokenizes input code and prints out the tokens.

## Installation

To build the compiler, make sure you have `make` installed on your system. Then, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/MatheusVBellini/lexical-analyser
    ```

2. Navigate to the project directory:

    ```bash
    cd lexical-analyser
    ```

3. Build the compiler:

    ```bash
    make
    ```

## Usage

Once the compiler is built, you can use it to tokenize PL0 code files. Here are the available commands:

- `make run`: Runs the compiler on a PL0 code file. You need to provide the path to the input file.

    ```bash
    make run
    ```

- `make test`: Runs a test suite to ensure the compiler is working correctly. The test suite includes various PL0 code files with expected token outputs.

    ```bash
    make test
    ```

## Example

Suppose you have a PL0 code file named `example.pl0` with the following content:

```pl0
VAR x;
BEGIN
  x := 5;
END.
```

You can tokenize this file using the compiler:

```bash
make run example.pl0
```

This will output the tokens:

```
VAR, VAR
x, ident
;, simbolo_ponto_virgula
BEGIN, BEGIN
x, ident
:=, simbolo_atribuicao
5, numero
;, simbolo_ponto_virgula
END, END
., simbolo_ponto
```

## Authors
- [Heitor Tanoue](https://github.com/heitortanoue)
- [Matheus Bellini](https://github.com/MatheusVBellini)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.