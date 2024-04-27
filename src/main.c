#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyser.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  // declarations
  FILE *input = fopen("input.txt", "r");
  FILE *output = fopen("output.txt", "w");
  char current_str[BUFFER_SIZE];
  char c;

  // I/O error check
  if (!input || !output) {
    perror("Error opening files");
    exit(1);
  }

  // DEBUG: test table struct
  KWTable *kwtable = kwtable_init();
  for (int i = 0; i < TABLE_SIZE; i++) {
    printf("%s\n", kwtable->table[i]->keyword);
  }

  // freeing memory
  kwtable_free(kwtable);
  fclose(input);
  fclose(output);

  return 0;
}
