#ifndef PL0_UTILS_H
#define PL0_UTILS_H

#include <stdbool.h>

#include "lexical_analyzer/lexical_analyzer.h"

bool is_digit(char c);
bool is_letter(char c);
bool is_alphanumeric(char c);
bool is_special_symbol(char c);
bool is_possible_double_char_symbol(char c);
bool is_second_expected_char(char c, char next_c);
bool is_space(char c);
char* str_to_upper(const char* str);
bool is_equal_token_types(Token* t, int type);
bool is_equal_keywords(Token* t, const char* value);

#endif