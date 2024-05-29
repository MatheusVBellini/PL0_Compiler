#ifndef PL0_SYNTACTIC_ANALYZER_H
#define PL0_SYNTACTIC_ANALYZER_H

#include "lexical_analyzer/lexical_analyzer.h"

void PL0_syntactic_analyzer(Compiler_state* state);

void PROC_programa(Compiler_state* state);
void PROC_bloco(Compiler_state* state);
void PROC_constante(Compiler_state* state);
void PROC_declaracao(Compiler_state* state);
void PROC_comando(Compiler_state* state);

#endif