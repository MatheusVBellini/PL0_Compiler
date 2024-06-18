#include "syntactic_analyzer.h"

#include <stdio.h>

#include "compiler_errors/compiler_errors.h"
#include "lexical_analyzer/lexical_analyzer.h"
#include "utils/utils.h"

void PL0_syntactic_analyzer(Compiler_state* state) {
    get_next_token(state);
    PROC_programa(state);
}

// <programa> ::= <bloco>.
void PROC_programa(Compiler_state* state) {
    PROC_bloco(state);

    if (is_equal_token_types(state->token, symbol_period)) {
        get_next_token(state);
    } else {
        throw_error(ERR_NO_FINAL_PERIOD, state->current_line);
        panic_mode(state);
    }
}

// <bloco> ::= <declaracao><comando>
void PROC_bloco(Compiler_state* state) {
    PROC_declaracao(state);
    PROC_comando(state);
}

// <declaracao> ::= <constante><variavel><procedimento>
void PROC_declaracao(Compiler_state* state) {
    PROC_constante(state);
    PROC_variavel(state);
    PROC_procedimento(state);
}

// <constante> ::= CONST ident = numero <mais_const>; | ε
void PROC_constante(Compiler_state* state) {
    if (is_equal_keywords(state->token, "CONST")) {
        get_next_token(state);
        if (is_equal_token_types(state->token, symbol_identifier)) {
            get_next_token(state);
            if (is_equal_token_types(state->token, symbol_rel_eq)) {
                get_next_token(state);
                if (is_equal_token_types(state->token, symbol_number)) {
                    get_next_token(state);
                    PROC_mais_const(state);
                    if (is_equal_token_types(state->token, symbol_semicolon)) {
                        get_next_token(state);
                    } else {
                        throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
                        panic_mode(state);
                    }
                } else {
                    throw_error(ERR_LEXICAL_INVALID_NUMBER, state->current_line);
                    panic_mode(state);
                }
            } else {
                throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
                panic_mode(state);
            }
        } else {
            throw_error(ERR_LEXICAL_INVALID_IDENTIFIER, state->current_line);
            panic_mode(state);
        }
        return;
    }
    return;  // ε
}

// <mais_const> ::= , ident = numero <mais_const> | ε
void PROC_mais_const(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_comma)) {
        get_next_token(state);
        if (is_equal_token_types(state->token, symbol_identifier)) {
            get_next_token(state);
            if (is_equal_token_types(state->token, symbol_rel_eq)) {
                get_next_token(state);
                if (is_equal_token_types(state->token, symbol_number)) {
                    get_next_token(state);
                    PROC_mais_const(state);
                } else {
                    throw_error(ERR_LEXICAL_INVALID_NUMBER, state->current_line);
                    panic_mode(state);
                }
            } else {
                throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
                panic_mode(state);
            }
        } else {
            throw_error(ERR_LEXICAL_INVALID_IDENTIFIER, state->current_line);
            panic_mode(state);
        }
    }
    return;  // ε
}

// <variavel> ::= VAR ident <mais_var>; | ε
void PROC_variavel(Compiler_state* state) {
    if (is_equal_keywords(state->token, "VAR")) {
        get_next_token(state);
        if (is_equal_token_types(state->token, symbol_identifier)) {
            get_next_token(state);
            PROC_mais_var(state);
            if (is_equal_token_types(state->token, symbol_semicolon)) {
                get_next_token(state);
            } else {
                throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
                panic_mode(state);
            }
        } else {
            throw_error(ERR_LEXICAL_INVALID_IDENTIFIER, state->current_line);
            panic_mode(state);
        }
        return;
    }
    return;  // ε
}

// <mais_var> ::= , ident <mais_var> | ε
void PROC_mais_var(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_comma)) {
        get_next_token(state);
        if (is_equal_token_types(state->token, symbol_identifier)) {
            get_next_token(state);
            PROC_mais_var(state);
        } else {
            throw_error(ERR_LEXICAL_INVALID_IDENTIFIER, state->current_line);
            panic_mode(state);
        }
    }
    return;  // ε
}

// <procedimento> ::= PROCEDURE ident ; <bloco> ; <procedimento> | ε
void PROC_procedimento(Compiler_state* state) {
    if (is_equal_keywords(state->token, "PROCEDURE")) {
        get_next_token(state);
        if (is_equal_token_types(state->token, symbol_identifier)) {
            get_next_token(state);
            if (is_equal_token_types(state->token, symbol_semicolon)) {
                get_next_token(state);
                PROC_bloco(state);
                if (is_equal_token_types(state->token, symbol_semicolon)) {
                    get_next_token(state);
                    PROC_procedimento(state);
                } else {
                    throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
                    panic_mode(state);
                }
            } else {
                throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
                panic_mode(state);
            }
        } else {
            throw_error(ERR_LEXICAL_INVALID_IDENTIFIER, state->current_line);
            panic_mode(state);
        }
        return;
    }
    return;  // ε
}

// <comando> ::= ident := <expressao>           |
//               CALL ident                     |
//               BEGIN <comando> <mais_cmd> END |
//               IF <condicao> THEN <comando>   |
//               WHILE <condicao> DO <comando>  |
//               ε
void PROC_comando(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_identifier)) {
        get_next_token(state);
        if (is_equal_token_types(state->token, symbol_atrib)) {
            get_next_token(state);
            PROC_expressao(state);
        }
        return;
    }

    else if (is_equal_keywords(state->token, "CALL")) {
        get_next_token(state);
        if (is_equal_token_types(state->token, symbol_identifier)) {
            get_next_token(state);
        } else {
            throw_error(ERR_LEXICAL_INVALID_IDENTIFIER, state->current_line);
            panic_mode(state);
        }
        return;
    }

    else if (is_equal_keywords(state->token, "BEGIN")) {
        get_next_token(state);
        PROC_comando(state);
        PROC_mais_cmd(state);

        if (is_equal_keywords(state->token, "END")) {
            get_next_token(state);
        } else {
            throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
            panic_mode(state);
        }
        return;
    }

    else if (is_equal_keywords(state->token, "IF")) {
        get_next_token(state);
        PROC_condicao(state);
        if (is_equal_keywords(state->token, "THEN")) {
            get_next_token(state);
            PROC_comando(state);
        } else {
            throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
            panic_mode(state);
        }
        return;
    }

    else if (is_equal_keywords(state->token, "WHILE")) {
        get_next_token(state);
        PROC_condicao(state);
        if (is_equal_keywords(state->token, "DO")) {
            get_next_token(state);
            PROC_comando(state);
        } else {
            throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
            panic_mode(state);
        }
        return;
    }

    return;  // ε
}

// <mais_cmd> ::= ; <comando> <mais_cmd> | ε
void PROC_mais_cmd(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_semicolon)) {
        get_next_token(state);
        PROC_comando(state);
        PROC_mais_cmd(state);
    }
    return;  // ε
}

// <expressao> ::= <operador_unario> <termo> <mais_termos>
void PROC_expressao(Compiler_state* state) {
    PROC_operador_unario(state);
    PROC_termo(state);
    PROC_mais_termos(state);
}

// <operador_unario> ::= - | + | ε
void PROC_operador_unario(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_op_minus) ||
        is_equal_token_types(state->token, symbol_op_plus)) {
        get_next_token(state);
    }
    return;  // ε
}

// <termo> ::= <fator> <mais_fatores>
void PROC_termo(Compiler_state* state) {
    PROC_fator(state);
    PROC_mais_termos(state);
}

// <mais_termos> ::= - <termo> <mais_termos> |
//                   + <termo> <mais_termos> |
//                   ε
void PROC_mais_termos(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_op_minus) ||
        is_equal_token_types(state->token, symbol_op_plus)) {
        get_next_token(state);
        PROC_termo(state);
        PROC_mais_termos(state);
    }
    return;  // ε
}

// <fator> ::= ident | numero | ( <expressao> )
void PROC_fator(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_identifier)) {
        get_next_token(state);
    }

    else if (is_equal_token_types(state->token, symbol_number)) {
        get_next_token(state);
    }

    else if (is_equal_token_types(state->token, symbol_lparen)) {
        get_next_token(state);
        PROC_expressao(state);
        if (is_equal_token_types(state->token, symbol_rparen)) {
            get_next_token(state);
        } else {
            throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
            panic_mode(state);
        }
    } else {
        throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
        panic_mode(state);
    }
}

// <mais_fatores> ::= * <fator> <mais_fatores> |
//                    / <fator> <mais_fatores> |
//                    ε
void PROC_mais_fatores(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_op_times) ||
        is_equal_token_types(state->token, symbol_op_slash)) {
        get_next_token(state);
        PROC_fator(state);
        PROC_mais_fatores(state);
    }
    return;  // ε
}

// <condicao> ::= ODD <expressao> |
//                <expressao> <relacao> <expressao>
void PROC_condicao(Compiler_state* state) {
    if (is_equal_keywords(state->token, "ODD")) {
        get_next_token(state);
        PROC_expressao(state);
    }

    else {
        PROC_expressao(state);
        PROC_relacional(state);
        PROC_expressao(state);
    }
}

// <relacional> ::= = | <> | < | <= | > | >=
void PROC_relacional(Compiler_state* state) {
    if (is_equal_token_types(state->token, symbol_rel_eq) ||
        is_equal_token_types(state->token, symbol_rel_neq) ||
        is_equal_token_types(state->token, symbol_rel_lt) ||
        is_equal_token_types(state->token, symbol_rel_le) ||
        is_equal_token_types(state->token, symbol_rel_gt) ||
        is_equal_token_types(state->token, symbol_rel_ge)) {
        get_next_token(state);
    } else {
        throw_error(ERR_LEXICAL_INVALID_SYMBOL, state->current_line);
        panic_mode(state);
    }
}