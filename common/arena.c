#include "../common/common.h"

void arena_init(arena* a, gfu_uword capacity) {
    if (a == nullptr) return;

    char* memory = calloc((size_t)capacity, sizeof *memory);
    gfu_assert(memory != nullptr, "Failed to allocate arena backing memory.");

    *a = (arena) {
        .memory = memory,
        .capacity = capacity,
        .alignment = 16,
    };
}

void arena_deinit(arena* a) {
    if (a == nullptr) return;
    free(a->memory);
    *a = (arena) {0};
}

void* arena_alloc(arena* a, gfu_uword size) {
    gfu_assert(a != nullptr, "Can't allocate into a null arena.");

    const gfu_uword align = a->alignment;
    const gfu_uword padding = (align - (size % align)) % align;
    size += padding;

    gfu_assertf(a->allocated + size <= a->capacity, "Arena overflow: %u (currently allocated) + %u (requested) > %u (max capacity).", a->allocated, size, a->capacity);

    char* result = a->memory + a->allocated;
    a->allocated += size;
    return result;
}
