/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include "internal.h"

__GAMEFU_API__ gfuobj_raw* gfuas_source_assemble(gfu_source source) {
    gfuas_state state = {0};
    state.source = source;
    gfu_arena_init(&state.arena, 32 * 1024);
    gfuobj_raw* rom_data = gfuas_assemble_internal(&state);
    gfu_arena_deinit(&state.arena);
    return rom_data;
}

__GAMEFU_API__ gfuobj_raw* gfuas_builder_assemble(gfuas_builder* builder) {
    gfuas_state state = {0};
    state.source = (gfu_source) { .name = "<internal>", .text = "\n", .length = 1 };
    state.asmb = *builder;
    gfu_arena_init(&state.arena, 32 * 1024);
    gfuobj_raw* rom_data = gfuas_assemble_ir_internal(&state);
    gfu_arena_deinit(&state.arena);
    return rom_data;
}

gfuobj_raw* gfuas_assemble_ir_internal(gfuas_state* state) {
    return nullptr;
}

gfuobj_raw* gfuas_assemble_internal(gfuas_state* state) {
    state->lex_position = 0;
    gfuas_next_character(state);

    return gfuas_assemble_ir_internal(state);
}
