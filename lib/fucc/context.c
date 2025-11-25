#include <gamefu/prologue.h>

#include <fucc/context.h>
#include <fucc/diagnostic.h>
#include <fucc/parser.h>

bool fucc_context_init(fucc_context_t* c, int argc) {
    bool result = true;

    *c = (fucc_context_t){0};
    gfu_arena_init(&c->string_arena, 64 * 1024);
    c->inputs = calloc((size_t)argc - 1, sizeof *c->inputs);

    gfu_arena_init(&c->decl_type_arena, sizeof(fucc_decl_type_t) * 16 * 1024);
    c->decl_types = malloc((size_t)(c->decl_type_capacity = 1024) * sizeof(fucc_decl_type_t));

defer:;
    return result;
}

void fucc_context_deinit(fucc_context_t* c) {
    fucc_diag_flush(c);
    free(c->decl_types);
    gfu_arena_deinit(&c->decl_type_arena);
    free(c->inputs);
    gfu_arena_deinit(&c->string_arena);
    *c = (fucc_context_t){0};
}

const char* fucc_intern_string(fucc_context_t* c, const char* str, size_t length) {
    for (const char* sub = c->string_arena.memory, *end = c->string_arena.memory + c->string_arena.allocated; sub < end; ) {
        size_t sub_length = strlen(sub);
        if (length == sub_length && 0 == strncmp(sub, str, length)) {
            return sub;
        }
        sub += sub_length + 1;
    }

    char *new_str = gfu_arena_alloc(&c->string_arena, (gfu_uword_t)length + 1);
    memcpy(new_str, str, length);
    return new_str;
}
