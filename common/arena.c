#include "../common/common.h"

void gfu_arena_init(gfu_arena* arena, gfu_uword capacity) {
    if (arena == nullptr) return;

    char* memory = calloc((size_t)capacity, sizeof *memory);
    gfu_assert(memory != nullptr, "Failed to allocate arena backing memory.");

    *arena = (gfu_arena) {
        .memory = memory,
        .capacity = capacity,
        .alignment = 16,
    };
}

void gfu_arena_deinit(gfu_arena* arena) {
    if (arena == nullptr) return;
    free(arena->memory);
    *arena = (gfu_arena) {0};
}

void* gfu_arena_alloc(gfu_arena* arena, gfu_uword size) {
    gfu_assert(arena != nullptr, "Can't allocate into a null arena.");

    const gfu_uword align = arena->alignment;
    const gfu_uword padding = (align - (size % align)) % align;
    size += padding;

    gfu_assertf(arena->allocated + size <= arena->capacity, "Arena overflow: %u (currently allocated) + %u (requested) > %u (max capacity).", arena->allocated, size, arena->capacity);

    char* result = arena->memory + arena->allocated;
    arena->allocated += size;
    return result;
}
