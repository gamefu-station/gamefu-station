#include <gamefu/prologue.h>
#include <gamefu/memory.h>

#include <errno.h>
#include <stdlib.h>

void gfu_arena_init(gfu_arena_t* arena, gfu_uword_t capacity) {
    if (arena == nullptr) return;

    char* memory = calloc((size_t)capacity, sizeof *memory);
    gfu_assert(memory != nullptr, "Failed to allocate arena backing memory.");

    *arena = (gfu_arena_t) {
        .memory = memory,
        .capacity = capacity,
        .alignment = 16,
    };
}

void gfu_arena_deinit(gfu_arena_t* arena) {
    if (arena == nullptr) return;
    free(arena->memory);
    *arena = (gfu_arena_t) {0};
}

void* gfu_arena_alloc(gfu_arena_t* arena, gfu_uword_t size) {
    gfu_assert(arena != nullptr, "Can't allocate into a null arena.");

    const gfu_uword_t align = arena->alignment;
    const gfu_uword_t padding = (align - (size % align)) % align;
    size += padding;

    gfu_assertf(arena->allocated + size <= arena->capacity, "Arena overflow: %u (currently allocated) + %u (requested) > %u (max capacity).", arena->allocated, size, arena->capacity);

    char* result = arena->memory + arena->allocated;
    arena->allocated += size;
    return result;
}

static gfu_thread_local char gfu_temp[GFU_TEMP_SIZE] = {0};
static gfu_thread_local gfu_uword_t gfu_temp_allocated = 0;

gfu_uword_t gfu_temp_mark(VOIDPROTO) {
    return gfu_temp_allocated;
}

void gfu_temp_rewind(gfu_uword_t mark) {
    if (mark >= gfu_temp_allocated) return;
    memset(gfu_temp + mark, 0, (size_t)(gfu_temp_allocated - mark));
    gfu_temp_allocated = mark;
}

void gfu_temp_clear(VOIDPROTO) {
    gfu_temp_allocated = 0;
}

void* gfu_temp_alloc(gfu_uword_t size) {
    const gfu_uword_t align = 4 * sizeof(gfu_uword_t);
    const gfu_uword_t padding = (align - (size % align)) % align;
    size += padding;

    gfu_assertf(gfu_temp_allocated + size <= GFU_TEMP_SIZE, "Temporary buffer overflow: %u (currently allocated) + %u (requested) > %u (max capacity).", gfu_temp_allocated, size, GFU_TEMP_SIZE);

    char* result = gfu_temp + gfu_temp_allocated;
    gfu_temp_allocated += size;
    return result;
}
