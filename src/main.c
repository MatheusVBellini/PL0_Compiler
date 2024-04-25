#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  // declarations
  FILE *input = fopen("meu_programa.txt", "r");
  FILE *output = fopen("saida.txt", "w");
  char str[BUFFER_SIZE];

  // I/O error check
  if (!input || !output) {
    perror("Error opening files");
    exit(1);
  }

  // freeing memory
  fclose(input);
  fclose(output);

  return 0;
}
