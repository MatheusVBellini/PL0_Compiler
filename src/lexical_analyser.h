#ifndef LEXICAL_ANALYSER_H
#define LEXICAL_ANALYSER_H

#include <stdio.h>

typedef struct {
  char *token;
  char *classe;
} Token;

/**
 * @brief read the next token in a file stream
 * @param input Input file being read
 * @return Token struct
 */
Token read_token(FILE *input);

#endif
