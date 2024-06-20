#include "compiler_errors.h"

#include <stdio.h>
#include <string.h>

#include "lexical_analyzer/lexical_analyzer.h"

const token_type sync_tokens[] = {symbol_semicolon, symbol_period, symbol_identifier, symbol_keyword};
const int num_sync_tokens = sizeof(sync_tokens) / sizeof(sync_tokens[0]);

PL0_Error errors_table[] = {
    {ERR_NO_FINAL_PERIOD, "Expected a period at the end of the program"},
    {ERR_COMMENT_NOT_CLOSED, "Comment not closed"},
    {ERR_UNEXPECTED_EOF, "Unexpected end of file"},
    {ERR_LEXICAL_INVALID_SYMBOL, "Invalid symbol"},
    {ERR_LEXICAL_INVALID_NUMBER, "Invalid number"},
    {ERR_LEXICAL_INVALID_IDENTIFIER, "Invalid identifier"},
    {ERR_LEXICAL_MISSING_SEMICOLON, "Missing semicolon `;` before this token"},
    {ERR_LEXICAL_MISSING_EQUAL_SYMBOL, "Missing equal `=` symbol before this token"},
    {ERR_LEXICAL_MISSING_END_SYMBOL, "Missing `END` symbol before this token"},
    {ERR_LEXICAL_MISSING_THEN_SYMBOL, "Missing `THEN` symbol before this token"},
    {ERR_LEXICAL_MISSING_DO_SYMBOL, "Missing `DO` symbol before this token"},
    {ERR_LEXICAL_MISSING_RIGHT_PARENTHESIS, "Missing right parenthesis `)` before this token"},
    {ERR_LEXICAL_MISSING_LEFT_PARENTHESIS, "Missing left parenthesis `(` before this token"},
    {ERR_LEXICAL_MISSING_RELATIONAL_OPERATOR, "Missing relational operator `=`, `<>`, `<`, `>`, `<=`, `>=` before this token"},
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

void throw_error(int error_code, Compiler_state* s) {
    Input_info* inp = s->input_info;
    s->error_count++;

    // print without line number
    if (inp->current_line <= 0 || inp->line == NULL) {
        if (s->flags->test_mode) {
            printf("error: %s\n", get_error_message(error_code));
            return;
        }

        printf("\033[1;31merror:\033[0m %s\n", get_error_message(error_code));
        return;
    }

    if (s->flags->test_mode) {
        printf("%d:%d error: %s\n", inp->current_line, inp->line_pos, get_error_message(error_code));
        return;
    }

    size_t token_len = 0;
    if (s->token != NULL) {
        token_len = strlen(s->token->value);
    }

    // print with line number and the line content
    printf("\033[1m%s:%d:%d:\033[1;31m error:\033[0m %s\n", inp->file_name, inp->current_line, inp->line_pos, get_error_message(error_code));

    // Print the line with the token highlighted in red
    printf("  %02d |\t", inp->current_line);
    for (size_t i = 0; i < inp->line_pos - token_len; i++) {
        printf("%c", inp->line[i]);
    }

    if (s->token != NULL) {
        printf("\033[1;31m%s\033[0m", s->token->value);
    }
    printf("%s", inp->line + inp->line_pos);

    printf("     |\t");

    if (s->token == NULL) {
        token_len++;  // It just works
    }

    // Print the caret pointing to the error position (line_pos - token_len)
    for (size_t i = 0; i < inp->line_pos - token_len; i++) {
        if (inp->line[i] == '\t') {
            printf("\t");  // handle tabs properly
        } else {
            printf(" ");
        }
    }

    if (s->token == NULL) {
        token_len--;  // It just works
    }

    // Print the caret
    printf("\033[1;31m^\033[0m");

    // Print the ~
    for (size_t i = 1; i < token_len; i++) {
        printf("\033[1;31m~\033[0m");
    }

    printf("\n");
}

void panic_mode(Compiler_state* state) {
    if (!state->token) {
        return;  // No token to synchronize
    }

    // Keep reading tokens until a sync token is found
    while (!is_sync_token(state->token->type)) {
        get_next_token(state);
    }
}

int is_sync_token(token_type type) {
    for (int i = 0; i < num_sync_tokens; i++) {
        if (type == sync_tokens[i]) {
            return 1;
        }
    }
    return 0;
}