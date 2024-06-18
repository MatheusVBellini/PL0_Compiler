#include "compiler_errors.h"

#include "lexical_analyzer/lexical_analyzer.h"

#include <stdio.h>

const token_type sync_tokens[] = {symbol_semicolon, symbol_period};
const int num_sync_tokens = sizeof(sync_tokens) / sizeof(sync_tokens[0]);

PL0_Error errors_table[] = {
    {ERR_NO_FINAL_PERIOD, "Expected a period at the end of the program"},
    {ERR_COMMENT_NOT_CLOSED, "Comment not closed"},
    {ERR_UNEXPECTED_EOF, "Unexpected end of file"},
    {ERR_LEXICAL_INVALID_SYMBOL, "Invalid symbol"},
    {ERR_LEXICAL_INVALID_NUMBER, "Invalid number"},
    {ERR_LEXICAL_INVALID_IDENTIFIER, "Invalid identifier"},
    {ERR_LEXICAL_MISSING_SEMICOLON, "Missing semicolon \" ; \""},
    {ERR_LEXICAL_MISSING_EQUAL_SYMBOL, "Missing equal \" = \" symbol"},
    {ERR_LEXICAL_MISSING_END_SYMBOL, "Missing \" END \" symbol"},
    {ERR_LEXICAL_MISSING_THEN_SYMBOL, "Missing \" THEN \" symbol"},
    {ERR_LEXICAL_MISSING_DO_SYMBOL, "Missing \" DO \" symbol"},
    {ERR_LEXICAL_MISSING_RIGHT_PARENTHESIS, "Missing right parenthesis \" ) \""},
    {ERR_LEXICAL_MISSING_LEFT_PARENTHESIS, "Missing left parenthesis \" ( \""},
    {ERR_LEXICAL_MISSING_RELATIONAL_OPERATOR, "Missing relational operator \" =, <>, <, >, <=, >= \""},
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

void panic_mode(Compiler_state* state) {
    if (!state->token) {
        return; // Não há mais tokens para analisar
    }

    // Continuar obtendo o próximo token até que encontremos um que possa sincronizar a análise
    while (!is_sync_token(state->token->type)) {
        printf("Descartando token: %d na linha %d\n", state->token->type, state->current_line);
        get_next_token(state);
    }
    printf("Recuperação completa. Retomando análise no token: %d na linha %d\n", state->token->type, state->current_line);
}

int is_sync_token(token_type type) {
    for (int i = 0; i < num_sync_tokens; i++) {
        if (type == sync_tokens[i]) {
            return 1;
        }
    }
    return 0;
}