#include "syntactic_analyzer.h"

#include <stdio.h>

#include "compiler_errors/compiler_errors.h"
#include "lexical_analyzer/lexical_analyzer.h"
#include "utils/utils.h"

void PL0_syntactic_analyzer(Compiler_state* state) {
    get_next_token(state);
    PROC_programa(state);
}

void PROC_programa(Compiler_state* state) {
    PROC_bloco(state);

    if (is_equal_token_types(state->token, symbol_period)) {
        get_next_token(state);
    } else {
        throw_error(ERR_NO_FINAL_PERIOD, state->current_line);
    }
}

void PROC_bloco(Compiler_state* state) {
    PROC_declaracao(state);
    PROC_comando(state);
}

void PROC_constante(Compiler_state* state) {
    if (is_equal_keywords(state->token, "CONST")) {
        get_next_token(state);

        if (is_equal_token_types(state->token, symbol_number)) {

        }

        return;
    }

    // throw_error(ERR_MISSING_KEYWORD, state->current_line);
}

void PROC_declaracao(Compiler_state* state) {
    PROC_constante(state);
    // ...
}

void PROC_comando(Compiler_state* state) {
    // ...
}