#include "utils.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "compiler_errors/compiler_errors.h"
#include "lexical_analyzer/lexical_analyzer.h"

/**
 * @brief Check if a character is a digit
 * @param c Character
 * @return true if c is a digit, false otherwise
 */
bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

/**
 * @brief Check if a character is a letter
 * @param c Character
 * @return true if c is a letter, false otherwise
 */
bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/**
 * @brief Check if a character is alphanumeric
 * @param c Character
 * @return true if c is alphanumeric, false otherwise
 */
bool is_alphanumeric(char c) {
    return is_digit(c) || is_letter(c);
}

/**
 * @brief Check if a character is a special symbol
 * @param c Character
 * @return true if c is a special symbol, false otherwise
 */
bool is_special_symbol(char c) {
    // Check if c is a single character symbol (e.g., '+', '-', '*', '/')
    return strchr("+-*/=<>,;.", c) != NULL;
}

/**
 * @brief Check if a character can be the first char of a two-character symbol
 * @param c Character
 * @return true if c can be the first char of a two-character symbol, false otherwise
 */
bool is_possible_double_char_symbol(char c) {
    // Check if c can be the first char of a two-character symbol (e.g., ':', '<', '>')
    return strchr(":<=>", c) != NULL;
}

/**
 * @brief Get the second character of a two-character symbol
 * @param c First character of the symbol
 * @return Second character of the symbol
 * @return true if c can be the first char of a two-character symbol, false otherwise
 */
bool is_second_expected_char(char c, char next_c) {
    // Get the expected second character for a two-character symbol based on the first char
    switch (c) {
        case ':':
            return next_c == '=';
        case '<':
            return next_c == '>' || next_c == '=';
        case '>':
            return next_c == '=';
        default:
            return false;
    }
}

/**
 * @brief Check if a character is a space
 * @param c Character
 * @return true if c is a space, false otherwise
 */
bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

char* str_to_upper(const char* str) {
    char* upper_str = (char*)malloc(strlen(str) + 1);
    for (int i = 0; str[i]; i++) {
        upper_str[i] = toupper(str[i]);
    }
    upper_str[strlen(str)] = '\0';
    return upper_str;
}

bool is_equal_token_types(Token* t, int type) {
    if (t == NULL) {
        throw_error(ERR_UNEXPECTED_EOF, 0);
        return false;
    };

    return t->type == type;
}

bool is_equal_keywords(Token* t, const char* value) {
    if (t == NULL) {
        throw_error(ERR_UNEXPECTED_EOF, 0);
        return false;
    };

    if (t->type == symbol_keyword) {
        char* upper = str_to_upper(t->value);
        bool result = strcmp(upper, value) == 0;
        free(upper);
        return result;
    }

    return false;
}