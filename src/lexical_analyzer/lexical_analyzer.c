#include "lexical_analyzer.h"

#include <stdlib.h>
#include <string.h>

#include "hash_table/hash_table.h"

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

bool is_special_symbol(char c) {
    // Check if c is a single character symbol (e.g., '+', '-', '*', '/')
    return strchr("+-*/=<>,;.", c) != NULL;
}

bool is_possible_double_char_symbol(char c) {
    // Check if c can be the first char of a two-character symbol (e.g., ':', '<', '>')
    return strchr(":<=>", c) != NULL;
}

char get_second_char(char c) {
    // Get the expected second character for a two-character symbol based on the first char
    switch (c) {
        case ':':
            return '=';
        case '<':
            return '>';  // Assuming '<>' or '<='
        case '>':
            return '=';  // Assuming '>='
        default:
            return '\0';
    }
}

void add_token(Token_list* tl, Token token) {
    tl->tokens[tl->count] = token;
    tl->count++;
}

bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void transform_line_to_tokens(char* line, Token_list* tl, KWTable* kwtable) {
    bool is_comment = false;  // Flag to check if the current line is a comment
    int i = 0;
    char c;

    while (line[i] != '\0') {
        c = line[i];

        if (is_comment) {
            if (c == '}') {
                is_comment = false;
            }
            i++;
            continue;
        }

        if (c == '{') {
            is_comment = true;
            i++;
            continue;
        }

        if (is_alphanumeric(c) && !is_comment) {
            char word[PL0_MAX_TOKEN_SIZE];
            int word_len = 0;

            while (is_alphanumeric(c)) {
                word[word_len++] = c;
                i++;
                if (line[i] == '\0') break;  // Check to avoid overflow if it's the end of the string
                c = line[i];
            }

            word[word_len] = '\0';

            if (kwtable_query(kwtable, word)) {
                // Check if the word is a keyword
                add_token(tl, new_token(symbol_keyword, word));
            } else if (is_letter(word[0])) {
                // Check if the word is an identifier
                add_token(tl, new_token(symbol_identifier, word));
            } else if (is_digit(word[0])) {
                // Check if the word is a number
                add_token(tl, new_token(symbol_number, word));
            } else {
                // If the word is not a keyword, identifier or number, it's an error
                add_token(tl, new_token(symbol_error, word));
            }

            continue;
        }

        if (is_space(c)) {
            i++;
            continue;
        }

        // Handle compound symbols and error reporting
        if (!is_comment) {
            if (c == ':' && line[i + 1] == '=') {
                add_token(tl, new_token(symbol_atrib, ":="));
                i += 2;  // Skip both characters
            } else if (is_special_symbol(c)) {
                char symbol[3] = {c, '\0', '\0'};
                if (is_possible_double_char_symbol(c) && line[i + 1] == get_second_char(c)) {
                    symbol[1] = line[i + 1];
                    i++;  // Advance an extra character
                }
                add_token(tl, new_token(get_symbol_id(symbol), symbol));
                i++;
            } else {
                char err[2] = {c, '\0'};
                add_token(tl, new_token(symbol_error, err));
                i++;
            }
            continue;
        }

        i++;  // Move to the next character
    }
}

Token new_token(token_type type, char* value) {
    Token token;
    token.type = type;
    strcpy(token.value, value);

    return token;
}

Symbol_entry symbol_names[] = {
    {"<LEXICAL_ERROR>", symbol_error},
    {"symbol_atrib", symbol_atrib},
    {"symbol_op_plus", symbol_op_plus},
    {"symbol_op_minus", symbol_op_minus},
    {"symbol_op_times", symbol_op_times},
    {"symbol_op_slash", symbol_op_slash},
    {"symbol_rel_eq", symbol_rel_eq},
    {"symbol_rel_neq", symbol_rel_neq},
    {"symbol_rel_lt", symbol_rel_lt},
    {"symbol_rel_gt", symbol_rel_gt},
    {"symbol_rel_le", symbol_rel_le},
    {"symbol_rel_ge", symbol_rel_ge},
    {"symbol_lparen", symbol_lparen},
    {"symbol_rparen", symbol_rparen},
    {"symbol_comma", symbol_comma},
    {"symbol_semicolon", symbol_semicolon},
    {"symbol_period", symbol_period},
    {"symbol_lkey", symbol_lkey},
    {"symbol_rkey", symbol_rkey},
    {"identifier", symbol_identifier},
    {"number", symbol_number}};

char* get_symbol_name(int symbol_id) {
    int symbols_count = sizeof(symbol_names) / sizeof(Symbol_entry);
    for (int i = 0; i < symbols_count; i++) {
        if ((int)symbol_names[i].type == symbol_id) {
            return symbol_names[i].name;
        }
    }

    return NULL;
}

void print_tokens(Token_list* tl) {
    printf("\n===================\n");
    printf("%d tokens were found:\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        Token token = tl->tokens[i];

        if (token.type == symbol_keyword) {
            // keyword, print the own value
            printf("%s, %s\n", token.value, token.value);
            continue;
        }

        char* name = get_symbol_name(token.type);
        if (name == NULL) {
            printf("%s, NO_NAME (id = %d)\n", token.value, token.type);
            continue;
        }

        printf("%s, %s\n", token.value, name);
    }
}

void PL0_lexical_analyzer(FILE* file) {
    Token_list token_list;

    token_list.tokens = (Token*)malloc(sizeof(Token) * 1024);
    token_list.count = 0;

    // init keyword table
    KWTable* kwtable = kwtable_init();

    char line[PL0_MAX_TOKEN_SIZE];
    while (fgets(line, PL0_MAX_TOKEN_SIZE, file)) {
        printf("[->] %s", line);
        transform_line_to_tokens(line, &token_list, kwtable);
    }

    print_tokens(&token_list);
}
