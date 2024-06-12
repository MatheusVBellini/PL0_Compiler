#include "compiler_errors.h"

#include <stdio.h>

PL0_Error errors_table[] = {
    {ERR_NO_FINAL_PERIOD, "Expected a period at the end of the program"},
    {ERR_COMMENT_NOT_CLOSED, "Comment not closed"},
    {ERR_UNEXPECTED_EOF, "Unexpected end of file"},
    {ERR_LEXICAL_INVALID_SYMBOL, "Invalid symbol"},
    {ERR_LEXICAL_INVALID_NUMBER, "Invalid number"},
    {ERR_LEXICAL_INVALID_IDENTIFIER, "Invalid identifier"}
};

// crie uma funcao que pega um codigo de erro e retorna uma mensagem de erro
char* get_error_message(int error_code) {
    for (long unsigned int i = 0; i < sizeof(errors_table) / sizeof(errors_table[0]); i++) {
        if (errors_table[i].code == error_code) {
            return errors_table[i].message;
        }
    }
    return "Unknown error";
}

void throw_error(int error_code, int line) {
    if (line <= 0) {
        // print error message in red
        printf("\033[1;31mError:\033[0m %s\n", get_error_message(error_code));
        return;
    }

    // print error message in red
    printf("\033[1;31mError in line %d:\033[0m %s\n", line, get_error_message(error_code));
}