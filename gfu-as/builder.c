/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/as.h>

GAMEFU_API void gfuas_builder_position_at_start(gfuas_builder* b) {
    if (b == nullptr) return;
    b->current = nullptr;
}

GAMEFU_API void gfuas_builder_position_at_end(gfuas_builder* b) {
    if (b == nullptr) return;
    b->current = b->tail;
}

GAMEFU_API void gfuas_builder_position_before(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    b->current = stmt->prev;
}

GAMEFU_API void gfuas_builder_position_after(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    b->current = stmt;
}

GAMEFU_API void gfuas_builder_insert(gfuas_builder* b, gfuas_stmt* stmt) {
    if (b == nullptr || stmt == nullptr) return;
    if (b->head == nullptr) {
        assertn(b->current == nullptr);
        assertn(b->tail == nullptr);
        b->head = b->tail = stmt;
    } else if (b->current == nullptr) {
        assertn(b->head != nullptr);
        stmt->next = b->head;
        b->head = stmt;
    } else {
        assertn(b->current != nullptr);
        b->current->prev->next = stmt;
        stmt->prev = b->current->prev;
        b->current->prev = stmt;
        stmt->next = b->current;
        b->current = stmt;
    }
}

GAMEFU_API void gfuas_set_location(gfuas_stmt* stmt, source source, gfu_uword location) {
    if (stmt == nullptr) return;
    stmt->source = source;
    stmt->location = location;
}

GAMEFU_API gfuas_stmt* gfuas_build_directive(
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

GAMEFU_API gfuas_stmt* gfuas_build_label(
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

GAMEFU_API gfuas_stmt* gfuas_build_instruction0(
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

GAMEFU_API gfuas_stmt* gfuas_build_instruction1(
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

GAMEFU_API gfuas_stmt* gfuas_build_instruction2(
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

GAMEFU_API gfuas_stmt* gfuas_build_instruction3(
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

GAMEFU_API gfuas_stmt* gfuas_build_instruction(
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
