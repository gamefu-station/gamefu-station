#ifndef GAMEFU_ISEL_H_
#define GAMEFU_ISEL_H_

#include "../common/common.h"
#include <gamefu/system/instruction.h>
#include <gamefu/system/register.h>

#include "gamefu.h"
#include "asm.h"

#include <stdbool.h>

typedef enum isel_type {
    ISEL_TY_INVALID,
    ISEL_TY_REG,
    ISEL_TY_IMM,
    ISEL_TY_ADDR,
    ISEL_TY_STR,
} isel_type, isel_type_t;

typedef struct isel_operand {
    isel_type_t type;
    bool is_constant;
    bool is_base;
    union {
        const char* var;
        gfu_uword_t immediate;
    } as;
} isel_operand, isel_operand_t;

typedef struct isel_match {
    fuasm_mnemonic_t mnemonic;
    gfu_uword_t operand_count;
    isel_operand_t operands[3];
} isel_match, isel_match_t;

typedef enum isel_argument_kind {
    ISEL_ARG_INVALID,
    ISEL_ARG_REGISTER,
    ISEL_ARG_VAR,
    ISEL_ARG_VAR_UPPER,
    ISEL_ARG_VAR_LOWER,
    ISEL_ARG_VAR_ADDR,
    ISEL_ARG_VAR_OFFS,
    ISEL_ARG_VAR_NEGATE,
    ISEL_ARG_IMM,
    ISEL_ARG_IMM_UPPER,
    ISEL_ARG_IMM_LOWER,
    ISEL_ARG_IMM_ADDR,
    ISEL_ARG_IMM_OFFS,
} isel_argument_kind, isel_argument_kind_t;

typedef struct isel_argument {
    isel_argument_kind_t kind;
    union {
        gfu_gpr_t _register;
        gfu_uword_t var_index;
        gfu_uword_t immediate;
    } as;
} isel_argument, isel_argument_t;

typedef enum isel_emit_kind {
    ISEL_EMIT_INVALID,
    ISEL_EMIT_REG,
    ISEL_EMIT_IMM,
    ISEL_EMIT_ADDR,
    ISEL_EMIT_COP0,
    ISEL_EMIT_BYTES,
} isel_emit_kind, isel_emit_kind_t;

typedef struct isel_emit {
    isel_emit_kind_t kind;
    union {
        gfu_opcode_t opcode;
        gfu_opfn_t function;
        gfu_c0fn_t cop0_function;
    } op;
    gfu_uword_t argument_count;
    isel_argument_t arguments[4];
} isel_emit, isel_emit_t;

typedef struct isel_pattern {
    gfu_uword_t var_count;
    gfu_uword_t match_index, match_count;
    gfu_uword_t emit_index, emit_count;
} isel_pattern, isel_pattern_t;

#endif /* GAMEFU_ISEL_H_ */
