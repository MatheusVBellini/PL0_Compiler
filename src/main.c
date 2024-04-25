#include "lexical_analyser.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  // declarations
  FILE *input = fopen("meu_programa.txt", "r");
  FILE *output = fopen("saida.txt", "w");
  char current_str[BUFFER_SIZE];
  char c;

  // I/O error check
  if (!input || !output) {
    perror("Error opening files");
    exit(1);
  }

  // read and write to file
  while ((c = fgetc(input)) != EOF) {
    fprintf(output, "%c", c);
  }

  // freeing memory
  fclose(input);
  fclose(output);

  return 0;
}
