/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/as.h>

void
gfuas_builder_init(gfuas_builder* b) {
    gfu_arena_init(&b->arena, 1024 * 1024);
}

void
gfuas_builder_deinit(gfuas_builder* b) {
    gfu_arena_deinit(&b->arena);
}

void gfuas_builder_position_at_start(gfuas_builder* b) {
    if (b == nullptr) return;
    b->current = nullptr;
}

void
gfuas_builder_position_at_end(gfuas_builder* b) {
    if (b == nullptr) return;
    b->current = b->tail;
}

void
gfuas_builder_position_before(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    b->current = stmt->prev;
}

void
gfuas_builder_position_after(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    b->current = stmt;
}

void
gfuas_builder_insert(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    if (b->head == nullptr) {
        gfu_assertn(b->current == nullptr);
        gfu_assertn(b->tail == nullptr);
        b->head = b->tail = stmt;
    } else if (b->current == nullptr) {
        gfu_assertn(b->head != nullptr);
        stmt->next = b->head;
        b->head = stmt;
    } else {
        gfu_assertn(b->current != nullptr);
        b->current->prev->next = stmt;
        stmt->prev = b->current->prev;
        b->current->prev = stmt;
        stmt->next = b->current;
        b->current = stmt;
    }
}

void
gfuas_set_location(
    gfuas_stmt* stmt, gfu_source source, gfu_uword location
) {
    if (stmt == nullptr) return;
    stmt->source = source;
    stmt->location = location;
}

gfuas_stmt*
gfuas_build_directive(
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

gfuas_stmt*
gfuas_build_label(
    gfuas_builder* b,
    const char* label,
    bool is_local
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .label = label,
        .is_label_local = is_local,
        .byte_size = 0,
    };
    gfuas_builder_insert(b, stmt);
    gfu_da_push(&b->labels, stmt);
    return stmt;
}

static void
gfuas_ensure_instruction_byte_size(gfuas_stmt* stmt) {
    switch (stmt->mnemonic) {
        default: break;
        case GFUAS_MNEM_BYTES: {
            if (stmt->operand_count != 1) break;
            const gfuas_expr op1 = stmt->operands[0];
            if (op1.kind == GFUAS_EXPR_BYTE_STRING) {
                stmt->byte_size = op1.as.byte_string.length;
            }
        } break;
    }
}

gfuas_stmt*
gfuas_build_instruction0(
    gfuas_builder* b,
    gfuas_mnemonic mnemonic
) {
    if (b == nullptr) return nullptr;
    gfuas_stmt* stmt = gfu_arena_alloc(&b->arena, sizeof *stmt);
    *stmt = (gfuas_stmt) {
        .mnemonic = mnemonic,
        .byte_size = sizeof(gfu_inst),
    };
    gfuas_ensure_instruction_byte_size(stmt);
    gfuas_builder_insert(b, stmt);
    return stmt;
}

gfuas_stmt*
gfuas_build_instruction1(
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
        .byte_size = sizeof(gfu_inst),
    };
    gfuas_ensure_instruction_byte_size(stmt);
    gfuas_builder_insert(b, stmt);
    return stmt;
}

gfuas_stmt*
gfuas_build_instruction2(
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
        .byte_size = sizeof(gfu_inst),
    };
    gfuas_ensure_instruction_byte_size(stmt);
    gfuas_builder_insert(b, stmt);
    return stmt;
}

gfuas_stmt*
gfuas_build_instruction3(
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
        .byte_size = sizeof(gfu_inst),
    };
    gfuas_ensure_instruction_byte_size(stmt);
    gfuas_builder_insert(b, stmt);
    return stmt;
}

gfuas_stmt*
gfuas_build_instruction(
    gfuas_builder* b,
    gfuas_mnemonic mnemonic,
    gfuas_expr* ops,
    int count
) {
    if (b == nullptr) return nullptr;
    gfu_assertf(count >= 0 && count <= 3, "Instruction argument count out of range: %d is not in the range [0, 3].", count);
    switch (count) {
        default: unreachable; return nullptr;
        case 0: return gfuas_build_instruction0(b, mnemonic);
        case 1: return gfuas_build_instruction1(b, mnemonic, ops[0]);
        case 2: return gfuas_build_instruction2(b, mnemonic, ops[0], ops[1]);
        case 3: return gfuas_build_instruction3(b, mnemonic, ops[0], ops[1], ops[2]);
    }
}
