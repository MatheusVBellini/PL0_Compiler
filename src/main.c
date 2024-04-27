#include <stdio.h>
#include <stdlib.h>

#include "pl0-lex-analyzer.h"

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

    PL0_lexical_analyzer(input);

    // freeing memory
    fclose(input);
    fclose(output);

    return 0;
}
