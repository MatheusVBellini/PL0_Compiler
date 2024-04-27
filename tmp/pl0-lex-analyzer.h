#ifndef PL0_LEXICAL_ANALYSER_H
#define PL0_LEXICAL_ANALYSER_H

#include <stdio.h>
#include <stdbool.h>

#define PL0_MAX_TOKEN_SIZE 64
#define PL0_MAX_TOKEN_NUMBER 1024

typedef enum {
    symbol_var = 1,         // VAR
    symbol_const = 2,       // CONST
    symbol_procedure = 3,   // PROCEDURE
    symbol_call = 4,        // CALL
    symbol_begin = 5,       // BEGIN
    symbol_end = 6,         // END
    symbol_if = 7,          // IF
    symbol_then = 8,        // THEN
    symbol_while = 9,       // WHILE
    symbol_do = 10,         // DO
    symbol_atrib = 11,      // :=
    symbol_op_plus = 12,    // +
    symbol_op_minus = 13,   // -
    symbol_op_times = 14,   // *
    symbol_op_slash = 15,   // /
    symbol_rel_eq = 15,     // =
    symbol_rel_neq = 16,    // <>
    symbol_rel_lt = 17,     // <
    symbol_rel_gt = 18,     // >
    symbol_rel_le = 19,     // <=
    symbol_rel_ge = 20,     // >=
    symbol_lparen = 21,     // (
    symbol_rparen = 22,     // )
    symbol_comma = 23,      // ,
    symbol_semicolon = 24,  // ;
    symbol_period = 25,     // .
    symbol_lkey = 26,       // {
    symbol_rkey = 27,       // }
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
    int value;
} Symbol_entry;

int get_reserved_word_id(char *word);
bool is_digit(char c);
bool is_letter(char c);
bool is_alphanumeric(char c);

void PL0_lexical_analyzer(FILE *file);

#endif
