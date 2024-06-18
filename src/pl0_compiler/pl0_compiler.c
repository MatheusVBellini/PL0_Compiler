#include "pl0_compiler.h"

#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyzer/lexical_analyzer.h"
#include "syntactic_analyzer/syntactic_analyzer.h"

/**
 * @brief Run the lexical analyzer on the given file
 * @param file File pointer
 */
void PL0_compiler(FILE* file, char* file_name) {
    // init keyword table
    KWTable* kwtable = kwtable_init();

    // init state
    Compiler_state state;
    state.input = file;
    state.kwtable = kwtable;
    state.input_info = malloc(sizeof(Input_info));
    state.input_info->line = malloc(PL0_MAX_TOKEN_SIZE);
    state.input_info->file_name = file_name;
    state.error_count = 0;

    PL0_syntactic_analyzer(&state);

    if (state.error_count == 0) {
        printf("\033[1;32mCompilation successful\033[0m\n");
    } else {
        printf("\n\033[1;31mCompilation failed, %d errors\033[0m\n", state.error_count);
    }

    // free memory
    free(state.input_info->line);
    free(state.input_info);
    kwtable_free(kwtable);
}