#include "lexical_analyzer.h"

#include <stdlib.h>
#include <string.h>

#include "hash_table/hash_table.h"
#include "utils/utils.h"
#include "compiler_errors/compiler_errors.h"

#define COMMENT_CLOSED -1

// Used to relate the symbol name with its ID
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

/**
 * @brief Get the ID of a symbol based on its name
 * @param symbol Symbol name
 * @return Symbol ID
 */
int get_symbol_id(char* symbol) {
    int symbols_count = sizeof(symbols) / sizeof(Symbol_entry);
    for (int i = 0; i < symbols_count; i++) {
        if (strcmp(symbols[i].name, symbol) == 0) {
            return symbols[i].type;
        }
    }

    return -1;
}

// List of symbol names
// Used only for printing the tokens, we don't need to use this list for the lexical analysis
Symbol_entry symbol_names[] = {
    {"<ERRO_LEXICO>", symbol_error},
    {"simbolo_atribuicao", symbol_atrib},
    {"simbolo_mais", symbol_op_plus},
    {"simbolo_menos", symbol_op_minus},
    {"simbolo_vezes", symbol_op_times},
    {"simbolo_divisao", symbol_op_slash},
    {"simbolo_igual", symbol_rel_eq},
    {"simbolo_diferente", symbol_rel_neq},
    {"simbolo_menor", symbol_rel_lt},
    {"simbolo_maior", symbol_rel_gt},
    {"simbolo_menor_igual", symbol_rel_le},
    {"simbolo_maior_igual", symbol_rel_ge},
    {"simbolo_parentesis_esq", symbol_lparen},
    {"simbolo_parentesis_dir", symbol_rparen},
    {"simbolo_virgula", symbol_comma},
    {"simbolo_ponto_virgula", symbol_semicolon},
    {"simbolo_ponto", symbol_period},
    {"simbolo_chaves_esq", symbol_lkey},
    {"simbolo_chaves_dir", symbol_rkey},
    {"ident", symbol_identifier},
    {"numero", symbol_number}};

/**
 * @brief Get the name of a symbol based on its ID
 * @param symbol_id Symbol ID
 * @return Symbol name
 */
char* get_symbol_name(int symbol_id) {
    int symbols_count = sizeof(symbol_names) / sizeof(Symbol_entry);
    for (int i = 0; i < symbols_count; i++) {
        if ((int)symbol_names[i].type == symbol_id) {
            return symbol_names[i].name;
        }
    }

    return NULL;
}

/**
 * @brief Create a new token
 * @param type Token type
 * @param value Token value
 * @return Token*
 */
Token* new_token(token_type type, char* value) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
    strcpy(token->value, value);

    return token;
}

/**
 * @brief Add a token to the compiler state
 * @param s Compiler state
 * @param token The token to be added
 */
void add_token_to_state(Compiler_state* s, Token* token) {
    s->token = token;
}

/**
 * @brief Get the next token from the input file and store it in the state
 * @param s Compiler state
 */
void get_next_token(Compiler_state* s) {
    static char line[PL0_MAX_TOKEN_SIZE];
    static int line_index = 0;
    static bool end_of_line = true;
    static int comment_opened_in_line = COMMENT_CLOSED;

    char c;

    while (true) {
        if (end_of_line) {
            if (fgets(line, PL0_MAX_TOKEN_SIZE, s->input)) {
                line_index = 0;
                end_of_line = false;
                s->current_line++;
            } else {
                // If there are no more lines, add a NULL token to the state
                add_token_to_state(s, NULL);
                break;
            }
        }

        while ((c = line[line_index]) != '\0') {
            // Skip comments
            if (comment_opened_in_line != COMMENT_CLOSED) {
                if (c == '}') {
                    comment_opened_in_line = COMMENT_CLOSED;
                }
                line_index++;
                continue;
            }

            // Check if a comment starts
            if (c == '{') {
                comment_opened_in_line = s->current_line;
                line_index++;
                continue;
            }

            // Skip spaces
            if (is_space(c)) {
                line_index++;
                continue;
            }

            // Handle numbers, identifiers, symbols and errors
            if (is_alphanumeric(c)) {
                char word[PL0_MAX_TOKEN_SIZE];
                int word_len = 0;

                // Read the whole word, '_' is also considered a valid character
                while (is_alphanumeric(c) || c == '_') {
                    word[word_len++] = c;
                    line_index++;
                    if (line[line_index] == '\0') break;  // Check to avoid overflow if it's the end of the string
                    c = line[line_index];
                }
                word[word_len] = '\0';

                if (kwtable_query(s->kwtable, word)) {
                    // Check if the word is a keyword
                    add_token_to_state(s, new_token(symbol_keyword, word));
                } else if (is_letter(word[0])) {
                    // Check if the word is an identifier
                    add_token_to_state(s, new_token(symbol_identifier, word));
                } else if (is_digit(word[0])) {
                    // Check if the word is a number
                    add_token_to_state(s, new_token(symbol_number, word));
                } else {
                    // If the word is not a keyword, identifier or number, it's an error
                    add_token_to_state(s, new_token(symbol_error, word));
                }

                continue;
            }

            // Handle compound symbols and error reporting
            if (c == ':' && line[line_index + 1] == '=') {
                add_token_to_state(s, new_token(symbol_atrib, ":="));
                line_index += 2;  // Skip the two characters
                continue;
            }

            // Check if the character is a symbol
            if (is_special_symbol(c)) {
                char symbol[3] = {c, '\0', '\0'};

                // Check if the symbol is a two-character symbol and if the second character is the expected one
                if (is_possible_double_char_symbol(c) && is_second_expected_char(c, line[line_index + 1])) {
                    symbol[1] = line[line_index + 1];
                    line_index++;
                }
                add_token_to_state(s, new_token(get_symbol_id(symbol), symbol));
                line_index++;
                continue;
            }

            // If the character is not a valid one, it's an error
            char err[2] = {c, '\0'};
            add_token_to_state(s, new_token(symbol_error, err));
            line_index++;
        }

        // If we've processed the whole line, set the end_of_line flag
        end_of_line = true;
    }

    // If we reach this point, it means we've reached the end of the file
    // Check if there are comments that were not closed
    if (comment_opened_in_line != COMMENT_CLOSED) {
        throw_error(ERR_COMMENT_NOT_CLOSED, comment_opened_in_line);
    }
}

void print_token(Token* token) {
    if (token == NULL) {
        return;
    }

    if (token->type == symbol_keyword) {
        // keyword, print the value in uppercase
        char* upper = str_to_upper(token->value);
        printf("%s, %s\n", token->value, upper);
        free(upper);
        return;
    }

    char* name = get_symbol_name(token->type);
    if (name == NULL) {
        printf("%s, NO_NAME (id = %d)\n", token->value, token->type);
        return;
    }

    printf("%s, %s\n", token->value, name);
}

/**
 * @brief Get the next token from the input file and store it in the state
 * @param s Compiler state
 */
// void get_next_token(Compiler_state* s) {
//     static int i = 0;

//     // Check if there are more tokens to return
//     if (i < s->token_list->count) {
//         return &s->token_list->tokens[i++];
//     }

//     // If there are no more tokens, try to read next line
//     char line[PL0_MAX_TOKEN_SIZE];
//     if (fgets(line, PL0_MAX_TOKEN_SIZE, s->input)) {
//         transform_line_to_tokens(line, s->token_list, s->kwtable);
//         s->current_line++;
//         return get_next_token(s);
//     }

//     // If there are no more lines, return NULL
//     return NULL;
// }
