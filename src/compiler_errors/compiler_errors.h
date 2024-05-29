#ifndef PL0_ERRORS_H
#define PL0_ERRORS_H

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

#endif