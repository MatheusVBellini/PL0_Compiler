#ifndef PL0_SYNTACTIC_ANALYZER_H
#define PL0_SYNTACTIC_ANALYZER_H

#include "lexical_analyzer/lexical_analyzer.h"

void PL0_syntactic_analyzer(Compiler_state* state);

void PROC_programa(Compiler_state* state);
void PROC_bloco(Compiler_state* state);
void PROC_declaracao(Compiler_state* state);
void PROC_constante(Compiler_state* state);
void PROC_mais_const(Compiler_state* state);
void PROC_variavel(Compiler_state* state);
void PROC_mais_var(Compiler_state* state);
void PROC_procedimento(Compiler_state* state);
void PROC_comando(Compiler_state* state);
void PROC_mais_cmd(Compiler_state* state);
void PROC_expressao(Compiler_state* state);
void PROC_operador_unario(Compiler_state* state);
void PROC_termo(Compiler_state* state);
void PROC_mais_termos(Compiler_state* state);
void PROC_fator(Compiler_state* state);
void PROC_condicao(Compiler_state* state);
void PROC_relacional(Compiler_state* state);

#endif