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
    Token_list token_list;

    token_list.tokens = (Token*)malloc(sizeof(Token) * 1024);
    token_list.count = 0;

    // init keyword table
    KWTable* kwtable = kwtable_init();

    // init state
    Compiler_state state;
    state.input = file;
    state.token_list = &token_list;
    state.kwtable = kwtable;
    state.current_line = 1;

    while (get_next_token(&state) != NULL) {
        // vamos tirar esse while e o print_tokens depois
    }

    print_tokens(&token_list);
}