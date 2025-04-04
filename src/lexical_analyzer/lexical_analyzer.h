#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>
#include <stdbool.h>

#define PL0_MAX_TOKEN_SIZE 64
#define PL0_MAX_TOKEN_NUMBER 1024

typedef enum {
    symbol_error = -1,     // error
    symbol_keyword = 0,     // reserved word
    symbol_atrib = 1,      // :=
    symbol_op_plus = 2,    // +
    symbol_op_minus = 3,   // -
    symbol_op_times = 4,   // *
    symbol_op_slash = 5,   // /
    symbol_rel_eq = 6,     // =
    symbol_rel_neq = 7,    // <>
    symbol_rel_lt = 8,     // <
    symbol_rel_gt = 9,     // >
    symbol_rel_le = 10,     // <=
    symbol_rel_ge = 11,     // >=
    symbol_lparen = 12,     // (
    symbol_rparen = 13,     // )
    symbol_comma = 14,      // ,
    symbol_semicolon = 15,  // ;
    symbol_period = 16,     // .
    symbol_lkey = 17,       // {
    symbol_rkey = 18,       // }
    symbol_identifier = 19, // identifier
    symbol_number = 20,     // number
} token_type;

typedef struct token {
    token_type type;
    char value[PL0_MAX_TOKEN_SIZE];
} Token;

typedef struct token_list {
    Token *tokens;
    int count;
} Token_list;

typedef struct symbol_entry {
    char name[PL0_MAX_TOKEN_SIZE];
    token_type type;
} Symbol_entry;

// int get_reserved_word_id(char *word);
bool is_digit(char c);
bool is_letter(char c);
bool is_alphanumeric(char c);
Token new_token(token_type type, char* value);

void PL0_lexical_analyzer(FILE *file);

#endif
