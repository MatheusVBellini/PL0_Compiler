#ifndef PL0_SYNTACTIC_ANALYZER_H
#define PL0_SYNTACTIC_ANALYZER_H

#include "lexical_analyzer/lexical_analyzer.h"

void PL0_syntactic_analyzer(Compiler_state* state);

void PROC_programa(Token* t, Compiler_state* state);
void PROC_bloco(Token* t, Compiler_state* state);

#endif