/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GAMEFU_AS_H_
#define GAMEFU_AS_H_

#include <gamefu/common.h>
#include <gamefu/bfd/object.h>

#include <gamefu/opcodes/instruction.h>
#include <gamefu/opcodes/register.h>
#include <gamefu/opcodes/syscall.h>

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

typedef struct gfuas_strings {
    GFU_DA_FIELDS(char*);
} gfuas_strings;

typedef struct gfuas_builder_label {
    const char* name;
    gfu_uword address;
    gfu_uword parent;
} gfuas_builder_label;

typedef struct gfuas_builder_labels {
    GFU_DA_FIELDS(gfuas_builder_label);
} gfuas_builder_labels;

typedef struct gfuas_builder {
    gfu_arena arena;
    gfuas_strings strings;
    gfuas_builder_labels labels;
    gfuas_stmt* head;
    gfuas_stmt* tail;
    gfuas_stmt* current;
} gfuas_builder;

GAMEFU_API gfuobj_raw* gfuas_source_assemble(source source);
GAMEFU_API gfuobj_raw* gfuas_builder_assemble(gfuas_builder* builder);

GAMEFU_API void gfuas_builder_position_at_start(gfuas_builder* b);
GAMEFU_API void gfuas_builder_position_at_end(gfuas_builder* b);
GAMEFU_API void gfuas_builder_position_before(gfuas_builder* b, gfuas_stmt* stmt);
GAMEFU_API void gfuas_builder_position_after(gfuas_builder* b, gfuas_stmt* stmt);
GAMEFU_API void gfuas_builder_insert(gfuas_builder* b, gfuas_stmt* stmt);

GAMEFU_API void gfuas_set_location(gfuas_stmt* stmt, source source, gfu_uword location);
GAMEFU_API gfuas_stmt* gfuas_build_directive(gfuas_builder* b, gfuas_directive directive);
GAMEFU_API gfuas_stmt* gfuas_build_label(gfuas_builder* b, const char* label, bool is_local);
GAMEFU_API gfuas_stmt* gfuas_build_instruction0(gfuas_builder* b, gfuas_mnemonic mnemonic);
GAMEFU_API gfuas_stmt* gfuas_build_instruction1(gfuas_builder* b, gfuas_mnemonic mnemonic, gfuas_expr op1);
GAMEFU_API gfuas_stmt* gfuas_build_instruction2(gfuas_builder* b, gfuas_mnemonic mnemonic, gfuas_expr op1, gfuas_expr op2);
GAMEFU_API gfuas_stmt* gfuas_build_instruction3(gfuas_builder* b, gfuas_mnemonic mnemonic, gfuas_expr op1, gfuas_expr op2, gfuas_expr op3);
GAMEFU_API gfuas_stmt* gfuas_build_instruction(gfuas_builder* b, gfuas_mnemonic mnemonic, gfuas_expr* ops, int count);

GAMEFU_API int gfuas_driver_main(int argc, char** argv);
GAMEFU_API int gfuas_driver_fuzz(const char* text, size_t length);

#endif /* GAMEFU_AS_H_ */
