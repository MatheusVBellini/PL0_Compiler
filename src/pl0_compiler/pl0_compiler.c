#include "pl0_compiler.h"

#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyzer/lexical_analyzer.h"
#include "syntactic_analyzer/syntactic_analyzer.h"
#include "utils/utils.h"

/**
 * @brief Run the lexical analyzer on the given file
 * @param file File pointer
 */
void PL0_compiler(FILE* file, int argc, char* argv[]) {
    // init keyword table
    KWTable* kwtable = kwtable_init();

    // init state
    Compiler_state state;
    state.input = file;
    state.kwtable = kwtable;
    state.input_info = malloc(sizeof(Input_info));
    state.input_info->line = malloc(PL0_MAX_TOKEN_SIZE);
    state.input_info->file_name = argv[1];
    state.error_count = 0;
    set_flags(&state, argc, argv);

    PL0_syntactic_analyzer(&state);

    if (state.error_count == 0) {
        if (state.flags->test_mode) {
            printf("Compilation successful\n");
        } else {
            printf("\033[1;32mCompilation successful\033[0m\n");
        }
    } else {
        if (state.flags->test_mode) {
            printf("\nCompilation failed, %d errors\n", state.error_count);
        } else {
            printf("\n\033[1;31mCompilation failed, %d errors\033[0m\n", state.error_count);
        }
    }

    // free memory
    free(state.input_info->line);
    free(state.input_info);
    free(state.flags);
    kwtable_free(kwtable);
}