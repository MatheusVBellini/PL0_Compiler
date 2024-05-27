#include "syntactic_analyzer.h"
#include "lexical_analyzer/lexical_analyzer.h"
#include "compiler_errors/compiler_errors.h"

#include <stdio.h>

void PL0_syntactic_analyzer(Compiler_state* state) {
    // PROC_programa(t);
}

void PROC_programa(Token* t, Compiler_state* state) {
    PROC_bloco(t, state);
    if (t->type != symbol_period) {
        throw_error(ERR_NO_FINAL_PERIOD, state->current_line);
    }
}

void PROC_bloco(Token* t, Compiler_state* state) {
}