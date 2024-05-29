#include "pl0_compiler.h"
#include "lexical_analyzer/lexical_analyzer.h"
#include "syntactic_analyzer/syntactic_analyzer.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Run the lexical analyzer on the given file
 * @param file File pointer
 */
void PL0_compiler(FILE* file) {
    // init keyword table
    KWTable* kwtable = kwtable_init();

    // init state
    Compiler_state state;
    state.input = file;
    state.kwtable = kwtable;
    state.current_line = 0;

    PL0_syntactic_analyzer(&state);
}