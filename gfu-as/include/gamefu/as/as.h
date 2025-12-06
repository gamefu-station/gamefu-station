#ifndef GAMEFU_GFUAS_ASM_H_
#define GAMEFU_GFUAS_ASM_H_

#include <gamefu/common.h>

#include "../gfu-opcodes/instruction.h"
#include "../gfu-opcodes/register.h"
#include "../gfu-opcodes/syscall.h"

typedef enum gfuas_mnemonic {
    GFUAS_MNEM_INVALID,
#define MNEM(Id, Name) GFUAS_MNEM_##Id,
#include <gamefu/as/x/mnemonics.h>
} gfuas_mnemonic;

typedef enum gfuas_directive {
    GFUAS_DIR_INVALID,
#define DIR(Id, Image) GFUAS_DIR_##Id,
#include <gamefu/as/x/directives.h>
} gfuas_directive;

typedef enum gfuas_expr_kind {
    GFUAS_EXPR_INVALID,
#define EXPR(Id) GFUAS_EXPR_##Id,
#include <gamefu/as/x/exprs.h>
} gfuas_expr_kind;

typedef enum gfuas_addr_kind {
    GFUAS_ADDR_INVALID,
#define ADDR_KIND(Id) GFUAS_ADDR_##Id,
#include <gamefu/as/x/addr_kinds.h>
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
    struct gfuas_stmt* prev;
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

typedef struct gfuas_builder {
    gfu_arena arena;
    gfuas_stmt* head;
    gfuas_stmt* tail;
    gfuas_stmt* current;
} gfuas_builder;

char* gfuas_assemble(source source, gfu_uword* rom_size);
char* gfuas_assemble_ir(gfuas_stmt* ir, gfu_uword* rom_size);

void gfuas_builder_position_at_start(gfuas_builder* b);
void gfuas_builder_position_at_end(gfuas_builder* b);
void gfuas_builder_position_before(gfuas_builder* b, gfuas_stmt* stmt);
void gfuas_builder_position_after(gfuas_builder* b, gfuas_stmt* stmt);
void gfuas_builder_insert(gfuas_builder* b, gfuas_stmt* stmt);

static inline void gfuas_set_location(gfuas_stmt* stmt, source source, gfu_uword location) {
    if (stmt == nullptr) return;
    stmt->source = source;
    stmt->location = location;
}

static inline gfuas_stmt* gfuas_build_directive(
    gfuas_builder* b,
    gfuas_directive directive
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .directive = directive,
    };
    gfuas_builder_insert(b, stmt);
    return stmt;
}

static inline gfuas_stmt* gfuas_build_label(
    gfuas_builder* b,
    const char* label,
    bool is_local
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .label = label,
        .is_label_local = is_local,
    };
    gfuas_builder_insert(b, stmt);
    return stmt;
}

static inline gfuas_stmt* gfuas_build_instruction0(
    gfuas_builder* b,
    gfuas_mnemonic mnemonic
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .mnemonic = mnemonic,
    };
    gfuas_builder_insert(b, stmt);
    return stmt;
}

static inline gfuas_stmt* gfuas_build_instruction1(
    gfuas_builder* b,
    gfuas_mnemonic mnemonic,
    gfuas_expr op1
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .mnemonic = mnemonic,
        .operand_count = 1,
        .operands[0] = op1,
    };
    gfuas_builder_insert(b, stmt);
    return stmt;
}

static inline gfuas_stmt* gfuas_build_instruction2(
    gfuas_builder* b,
    gfuas_mnemonic mnemonic,
    gfuas_expr op1,
    gfuas_expr op2
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .mnemonic = mnemonic,
        .operand_count = 2,
        .operands[0] = op1,
        .operands[1] = op2,
    };
    gfuas_builder_insert(b, stmt);
    return stmt;
}

static inline gfuas_stmt* gfuas_build_instruction3(
    gfuas_builder* b,
    gfuas_mnemonic mnemonic,
    gfuas_expr op1,
    gfuas_expr op2,
    gfuas_expr op3
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .mnemonic = mnemonic,
        .operand_count = 3,
        .operands[0] = op1,
        .operands[1] = op2,
        .operands[2] = op3,
    };
    gfuas_builder_insert(b, stmt);
    return stmt;
}

static inline gfuas_stmt* gfuas_build_instruction(
    gfuas_builder* b,
    gfuas_mnemonic mnemonic,
    gfuas_expr* ops,
    int count
) {
    if (b == nullptr) return nullptr;
    assertf(count >= 0 && count <= 3, "Instruction argument count out of range: %d is not in the range [0, 3].", count);
    switch (count) {
        default: unreachable; return nullptr;
        case 0: return gfuas_build_instruction0(b, mnemonic);
        case 1: return gfuas_build_instruction1(b, mnemonic, ops[0]);
        case 2: return gfuas_build_instruction2(b, mnemonic, ops[0], ops[1]);
        case 3: return gfuas_build_instruction3(b, mnemonic, ops[0], ops[1], ops[2]);
    }
}

#endif /* GAMEFU_GFUAS_ASM_H_ */
