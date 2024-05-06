#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyzer/lexical_analyzer.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // declarations
    FILE *input;
    if (argc != 2) {
        fprintf(stderr, "\033[1;31mError:\033[0m Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }

    // open files
    input = fopen(argv[1], "r");

    // I/O error check
    if (!input) {
        perror("Error opening files");
        exit(1);
    }

    // run lexical analyzer
    PL0_lexical_analyzer(input);

    fclose(input);

    return 0;
}
