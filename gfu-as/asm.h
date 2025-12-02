#ifndef GAMEFU_GFUAS_ASM_H_
#define GAMEFU_GFUAS_ASM_H_

#include "../common/common.h"

#include "../gfu-opcodes/instruction.h"
#include "../gfu-opcodes/register.h"
#include "../gfu-opcodes/syscall.h"

#include "mnemonics.h"

typedef enum gfuas_directive {
    GFUAS_DIR_INVALID,
#define DIR(Id, Image) GFUAS_DIR_##Id,
#include "x/directives.h"
} gfuas_directive;

typedef enum gfuas_expr_kind {
    GFUAS_EXPR_INVALID,
#define EXPR(Id) GFUAS_EXPR_##Id,
#include "x/exprs.h"
} gfuas_expr_kind;

typedef enum gfuas_addr_kind {
    GFUAS_ADDR_INVALID,
#define ADDR_KIND(Id) GFUAS_ADDR_##Id,
#include "x/addr_kinds.h"
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

#endif /* GAMEFU_GFUAS_ASM_H_ */
