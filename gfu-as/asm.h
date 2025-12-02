#ifndef GAMEFU_ASM_H_
#define GAMEFU_ASM_H_

#include "../common/common.h"

#include "../gfu-opcodes/instruction.h"
#include "../gfu-opcodes/register.h"
#include "../gfu-opcodes/syscall.h"

#define GFUAS_DIRECTIVES(X) \
    X(ADDRESS_SPACE, "address_space") \
    X(ENTRY, "entry") \
    X(TEXT, "text") \
    X(DATA, "data")

typedef enum gfuas_mnemonic {
    GFUAS_MNEM_INVALID,
#define MNEM(Id, Name) GFUAS_MNEM_##Id,
#include "x/mnemonics.h"
} gfuas_mnemonic;

typedef enum gfuas_directive {
    GFUAS_DIR_INVALID,
#define X(Id, Image) GFUAS_DIR_##Id,
    GFUAS_DIRECTIVES(X)
#undef X
} gfuas_directive;

#define GFUAS_EXPR_KINDS(X) \
    X(REG) \
    X(IMM) \
    X(IMM_UPPER) \
    X(IMM_LOWER) \
    X(ADDR) \
    X(ADDR_UPPER) \
    X(ADDR_LOWER) \
    X(BYTE_STRING)

typedef enum gfuas_expr_kind {
    GFUAS_EXPR_INVALID,
#define X(Id) GFUAS_EXPR_##Id,
    GFUAS_EXPR_KINDS(X)
#undef X
} gfuas_expr_kind;

#define GFUAS_ADDR_KINDS(X) \
    X(LABEL)

typedef enum gfuas_addr_kind {
    GFUAS_ADDR_INVALID,
#define X(Id) GFUAS_ADDR_##Id,
    GFUAS_ADDR_KINDS(X)
#undef X
} gfuas_addr_kind;

typedef struct gfuas_addr {
    gfuas_addr_kind kind;
    source source;
    gfu_uword location;
    union {
        const char* label;
    } as;
} gfuas_addr;

typedef struct gfuas_expr {
    gfuas_expr_kind kind;
    source source;
    gfu_uword location;
    bool is_base;
    union {
        gfu_gpr _register;
        gfu_uword immediate;
        gfuas_addr address;
        struct {
            const char* data;
            gfu_uword length;
        } byte_string;
    } as;
} gfuas_expr;

typedef struct gfuas_stmt {
    struct gfuas_stmt* next;
    source source;
    gfu_uword location;
    gfuas_directive directive;
    const char* label;
    bool is_label_local;
    gfuas_mnemonic mnemonic;
    gfu_uword operand_count;
    gfuas_expr operands[3];
    gfu_uword pattern_index;
} gfuas_stmt;

char* gfuas_assemble(source source, gfu_uword* rom_size);
char* gfuas_assemble_ir(gfuas_stmt* ir, gfu_uword* rom_size);

#endif /* GAMEFU_ASM_H_ */
