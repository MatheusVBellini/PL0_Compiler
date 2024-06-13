#ifndef PL0_ERRORS_H
#define PL0_ERRORS_H

#include "lexical_analyzer/lexical_analyzer.h"

const int sync_tokens[] = {symbol_semicolon, symbol_period};
const int num_sync_tokens = sizeof(sync_tokens) / sizeof(sync_tokens[0]);

typedef enum {
    ERR_LEXICAL_INVALID_SYMBOL = 1,
    ERR_LEXICAL_INVALID_NUMBER,
    ERR_LEXICAL_INVALID_IDENTIFIER,
    ERR_NO_FINAL_PERIOD,
    ERR_COMMENT_NOT_CLOSED,
    ERR_UNEXPECTED_EOF,
} PL0_ERRORS;

// ERRORS TABLE WITH MESSAGES
typedef struct error {
    int code;
    char* message;
} PL0_Error;

void throw_error(int error_code, int line);
void panic_mode(Compiler_state* state);
int is_sync_token(int token_type);

#endif