#ifndef GAMEFU_ASM_H_
#define GAMEFU_ASM_H_

#include "../common/common.h"
#include "../gfu-opcodes/instruction.h"
#include "../gfu-opcodes/register.h"
#include "../gfu-opcodes/syscall.h"

#define FUASM_DIRECTIVES(X) \
    X(ADDRESS_SPACE, "address_space") \
    X(ENTRY, "entry") \
    X(TEXT, "text") \
    X(DATA, "data")

typedef enum fuasm_mnemonic {
    FUASM_MNEM_INVALID,
#define MNEM(Id, Name) FUASM_MNEM_##Id,
#include "mnemonics.h"
} fuasm_mnemonic, fuasm_mnemonic_t;

typedef enum fuasm_directive {
    FUASM_DIR_INVALID,
#define X(Id, Image) FUASM_DIR_##Id,
    FUASM_DIRECTIVES(X)
#undef X
} fuasm_directive, fuasm_directive_t;

typedef union fuasm_inst {
    gfu_uword_t raw;
    struct {
        gfu_uword_t function : 6;
        gfu_uword_t shift : 5;
        gfu_uword_t r : 5;
        gfu_uword_t l : 5;
        gfu_uword_t d : 5;
        gfu_uword_t opcode : 6;
    } reg;
    struct {
        gfu_uword_t value : 16;
        gfu_uword_t l : 5;
        gfu_uword_t d : 5;
        gfu_uword_t opcode : 6;
    } imm;
    struct {
        gfu_uword_t value : 26;
        gfu_uword_t opcode : 6;
    } addr;
} fuasm_inst, fuasm_inst_t;

static_assert(sizeof(fuasm_inst_t) == sizeof(gfu_uword_t), "Ensure that the union of bitfields does not change the size of the instruction type.");

#define FUASM_EXPR_KINDS(X) \
    X(REG) \
    X(IMM) \
    X(IMM_UPPER) \
    X(IMM_LOWER) \
    X(ADDR) \
    X(ADDR_UPPER) \
    X(ADDR_LOWER) \
    X(BYTE_STRING)

typedef enum fuasm_expr_kind {
    FUASM_EXPR_INVALID,
#define X(Id) FUASM_EXPR_##Id,
    FUASM_EXPR_KINDS(X)
#undef X
} fuasm_expr_kind, fuasm_expr_kind_t;

#define FUASM_ADDR_KINDS(X) \
    X(LABEL)

typedef enum fuasm_addr_kind {
    FUASM_ADDR_INVALID,
#define X(Id) FUASM_ADDR_##Id,
    FUASM_ADDR_KINDS(X)
#undef X
} fuasm_addr_kind, fuasm_addr_kind_t;

typedef struct fuasm_addr {
    fuasm_addr_kind_t kind;
    source source;
    gfu_uword_t location;
    union {
        const char* label;
    } as;
} fuasm_addr, fuasm_addr_t;

typedef struct fuasm_expr {
    fuasm_expr_kind_t kind;
    source source;
    gfu_uword_t location;
    bool is_base;
    union {
        gfu_gpr_t _register;
        gfu_uword_t immediate;
        fuasm_addr_t address;
        struct {
            const char* data;
            gfu_uword_t length;
        } byte_string;
    } as;
} fuasm_expr, fuasm_expr_t;

typedef struct fuasm_stmt {
    struct fuasm_stmt* next;
    source source;
    gfu_uword_t location;
    fuasm_directive_t directive;
    const char* label;
    bool is_label_local;
    fuasm_mnemonic_t mnemonic;
    gfu_uword_t operand_count;
    fuasm_expr_t operands[3];
    gfu_uword_t pattern_index;
} fuasm_stmt, fuasm_stmt_t;

char* fuasm_assemble(source source, gfu_uword_t* rom_size);
char* fuasm_assemble_ir(fuasm_stmt_t* ir, gfu_uword_t* rom_size);

#endif /* GAMEFU_ASM_H_ */
