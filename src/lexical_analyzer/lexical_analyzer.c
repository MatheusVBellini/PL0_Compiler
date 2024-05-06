#include "lexical_analyzer.h"

#include <stdlib.h>
#include <string.h>

#include "hash_table/hash_table.h"
#include "parser/parser.h"

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
 * @brief Add a token to the token list
 * @param tl Token list
 * @param token Token to be added
 */
void add_token(Token_list* tl, Token token) {
    tl->tokens[tl->count] = token;
    tl->count++;
}

/**
 * @brief Create a new token
 * @param type Token type
 * @param value Token value
 * @return Token
 */
Token new_token(token_type type, char* value) {
    Token token;
    token.type = type;
    strcpy(token.value, value);

    return token;
}

/**
 * @brief Transform a given line into tokens in the token list
 * @param line Line to be transformed
 * @param tl Token list to store the tokens
 * @param kwtable Keyword table
 */
void transform_line_to_tokens(char* line, Token_list* tl, KWTable* kwtable) {
    bool is_comment = false;  // Flag to check if the current line is a comment
    int i = 0;
    char c;

    while (line[i] != '\0') {
        c = line[i];

        // Skip comments
        if (is_comment) {
            if (c == '}') {
                is_comment = false;
            }
            i++;
            continue;
        }

        // Check if a comment starts
        if (c == '{') {
            is_comment = true;
            i++;
            continue;
        }

        // Skip spaces
        if (is_space(c)) {
            i++;
            continue;
        }

        // Handle numbers, identifiers, symbols and errors
        if (is_alphanumeric(c)) {
            char word[PL0_MAX_TOKEN_SIZE];
            int word_len = 0;

            // Read the whole word
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

        // Handle compound symbols and error reporting
        if (c == ':' && line[i + 1] == '=') {
            add_token(tl, new_token(symbol_atrib, ":="));
            i += 2;  // Skip both characters
        } else if (is_special_symbol(c)) {
            char symbol[3] = {c, '\0', '\0'};

            // Check if the symbol is a two-character symbol and if the second character is the expected one
            if (is_possible_double_char_symbol(c) && is_second_expected_char(c, line[i + 1])) {
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

        i++;  // Move to the next character
    }
}

/**
 * @brief Print the tokens in the token list
 * @param tl Token list
 */
void print_tokens(Token_list* tl) {
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

/**
 * @brief Run the lexical analyzer on the given file
 * @param file File pointer
 */
void PL0_lexical_analyzer(FILE* file) {
    Token_list token_list;

    token_list.tokens = (Token*)malloc(sizeof(Token) * 1024);
    token_list.count = 0;

    // init keyword table
    KWTable* kwtable = kwtable_init();

    char line[PL0_MAX_TOKEN_SIZE];

    // iterate over the file, line by line
    while (fgets(line, PL0_MAX_TOKEN_SIZE, file)) {
        transform_line_to_tokens(line, &token_list, kwtable);
    }

    print_tokens(&token_list);
}
