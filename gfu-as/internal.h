/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GAMEFU_AS_INTERNAL_H_
#define GAMEFU_AS_INTERNAL_H_

#include <gamefu/common.h>
#include <gamefu/bfd/object.h>
#include <gamefu/as.h>

#define VERSION "0.2.0"

typedef enum gfuas_token_kind {
    GFUAS_TK_INVALID = 0x00FFFFFF,
    GFUAS_TK_EOF = 0,

    _GFUAS_TK_MULTIBYTE_BEGIN = 127,

#define TK(Id) GFUAS_TK_##Id,
#include <gamefu/as/x/tokens.h>
} gfuas_token_kind;

typedef struct gfuas_token {
    gfuas_token_kind kind;
    gfu_uword location;
    union {
        gfuas_directive directive;
        gfuas_mnemonic mnemonic;
        gfu_gpr gpr;
        gfu_c0r c0r;
        gfu_uword immediate;
        const char* label;
        struct {
            const char* data;
            gfu_uword length;
        } byte_string;
    } as;
} gfuas_token;

typedef struct gfuas_state {
    const char* program;
    bool show_help : 1;
    bool show_version : 1;
    bool verbose : 1;
    source source;
    const char* output;

    gfu_arena arena;
    gfuas_builder asmb;
    gfuobj_builder objb;

    gfu_uword lex_position;
    gfu_word lex_character;
    gfu_word lex_stride;

    gfuas_token token, next_token;

    const char* entry_name;
    bool is_entry_set;
} gfuas_state;

bool gfuas_options_parse(int argc, char** argv, gfuas_state* state);

gfuas_token gfuas_read_token(gfuas_state* state);

gfuobj_raw* gfuas_assemble_internal(gfuas_state* state);
gfuobj_raw* gfuas_assemble_ir_internal(gfuas_state* state);

static inline bool gfuas_lexer_at_end(gfuas_state* state) {
    return state->lex_position >= (gfu_uword) state->source.length;
}

static inline void gfuas_next_character(gfuas_state* state) {
    state->lex_position += state->lex_stride;
    if (gfuas_lexer_at_end(state)) {
        state->lex_character = 0;
        return;
    }

    state->lex_character = state->source.text[state->lex_position];
    state->lex_stride = 1;
}

#endif /* GAMEFU_AS_INTERNAL_H_ */
