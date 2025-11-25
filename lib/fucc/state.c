#include <gamefu/prologue.h>

#include <fucc/state.h>

bool fucc_state_init(fucc_state_t* state) {
    gfu_arena_init(&state->string_arena, 64 * 1024 * 1024);
    return true;
}

void fucc_state_deinit(fucc_state_t* state) {
    gfu_arena_deinit(&state->string_arena);
}
