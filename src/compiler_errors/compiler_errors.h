#ifndef PL0_ERRORS_H
#define PL0_ERRORS_H

#include "lexical_analyzer/lexical_analyzer.h"


typedef enum {
    ERR_LEXICAL_INVALID_SYMBOL = 1,
    ERR_LEXICAL_INVALID_NUMBER,
    ERR_LEXICAL_INVALID_IDENTIFIER,
    ERR_NO_FINAL_PERIOD,
    ERR_COMMENT_NOT_CLOSED,
    ERR_UNEXPECTED_EOF,
    ERR_LEXICAL_MISSING_SEMICOLON,
    ERR_LEXICAL_MISSING_EQUAL_SYMBOL,
    ERR_LEXICAL_MISSING_END_SYMBOL,
    ERR_LEXICAL_MISSING_THEN_SYMBOL,
    ERR_LEXICAL_MISSING_DO_SYMBOL,
    ERR_LEXICAL_MISSING_RIGHT_PARENTHESIS,
    ERR_LEXICAL_MISSING_LEFT_PARENTHESIS,
    ERR_LEXICAL_MISSING_RELATIONAL_OPERATOR,
} PL0_ERRORS;

// ERRORS TABLE WITH MESSAGES
typedef struct error {
    int code;
    char* message;
} PL0_Error;

void throw_error(int error_code, int line, int* error_count);
void panic_mode(Compiler_state* state);
int is_sync_token(token_type token_type);

#endif