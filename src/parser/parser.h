#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

bool is_digit(char c);
bool is_letter(char c);
bool is_alphanumeric(char c);
bool is_special_symbol(char c);
bool is_possible_double_char_symbol(char c);
bool is_second_expected_char(char c, char next_c);
bool is_space(char c);

#endif