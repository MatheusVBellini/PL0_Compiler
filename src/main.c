#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyzer/lexical_analyzer.h"

#define BUFFER_SIZE 1024

int main() {
    // declarations
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    // I/O error check
    if (!input || !output) {
        perror("Error opening files");
        exit(1);
    }

    // run lexical analyzer
    PL0_lexical_analyzer(input);

    fclose(input);
    fclose(output);

    return 0;
}
