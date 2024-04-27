#include "pl0-lex-analyzer.h"

#include <stdlib.h>
#include <string.h>

Symbol_entry reserved_words[] = {
    {"const", symbol_const},
    {"var", symbol_var},
    {"procedure", symbol_procedure},
    {"call", symbol_call},
    {"begin", symbol_begin},
    {"end", symbol_end},
    {"if", symbol_if},
    {"then", symbol_then},
    {"while", symbol_while},
    {"do", symbol_do},
};

Symbol_entry symbols[] = {
    {"+", symbol_op_plus},
    {"-", symbol_op_minus},
    {"*", symbol_op_times},
    {"/", symbol_op_slash},
    {"=", symbol_rel_eq},
    {"<>", symbol_rel_neq},
    {"<", symbol_rel_lt},
    {">", symbol_rel_gt},
    {"<=", symbol_rel_le},
    {">=", symbol_rel_ge},
    {":=", symbol_atrib},
    {"(", symbol_lparen},
    {")", symbol_rparen},
    {",", symbol_comma},
    {";", symbol_semicolon},
    {".", symbol_period},
    {"{", symbol_lkey},
    {"}", symbol_rkey},
};

int get_reserved_word_id(char* word) {
    int reserved_words_count = sizeof(reserved_words) / sizeof(Symbol_entry);
    for (int i = 0; i < reserved_words_count; i++) {
        if (strcmp(reserved_words[i].name, word) == 0) {
            return reserved_words[i].type;
        }
    }

    return -1;
}

int get_symbol_id(char* symbol) {
    int symbols_count = sizeof(symbols) / sizeof(Symbol_entry);
    for (int i = 0; i < symbols_count; i++) {
        if (strcmp(symbols[i].name, symbol) == 0) {
            return symbols[i].type;
        }
    }

    return -1;
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_alphanumeric(char c) {
    return is_digit(c) || is_letter(c);
}

void add_token(Token_list* tl, Token token) {
    tl->tokens[tl->count] = token;
    tl->count++;
}

bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void transform_line_to_tokens(char* line, Token_list* tl) {
    bool is_comment = false;  // flag to check if the current line is a comment

    char c;
    int i = 0;
    while (is_comment || c != '\0') {
        char word[PL0_MAX_TOKEN_SIZE];
        int word_len = 0;

        if (is_comment) {
            // comentario
            return;
        }

        if (is_alphanumeric(c)) {
            // palavra ou numero
        }

        if (is_space(c)) {
            // começa outra palavra
            c = line[i++];
        }

        c = line[i++];
    }
}

Token new_token(token_type type, char* value) {
    Token token;
    token.type = type;
    strcpy(token.value, value);

    return token;
}

void PL0_lexical_analyzer(FILE* file) {
    Token_list token_list;

    token_list.tokens = (Token*)malloc(sizeof(Token) * 1024);

    char line[PL0_MAX_TOKEN_SIZE];
    while (fgets(line, PL0_MAX_TOKEN_SIZE, file)) {
        printf("[->] %s", line);
        transform_line_to_tokens(line, &token_list);
    }

    for (int i = 0; i < token_list.count; i++) {
        Token token = token_list.tokens[i];
        printf("Token: %s\n", token.value);
    }
}
