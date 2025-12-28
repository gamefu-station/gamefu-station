#include <gamefu/common.h>

void gfu_arena_init(gfu_arena* a, gfu_uword default_capacity_per_chunk) {
    if (a == nullptr) return;

    char* memory = calloc((size_t)default_capacity_per_chunk, sizeof *memory);
    assert(memory != nullptr, "Failed to allocate arena backing memory.");

    gfu_chunk* start = calloc(1, sizeof *start);
    assert(start != nullptr, "Failed to allocate arena chunk.");

    start->memory = memory;
    start->capacity = default_capacity_per_chunk;

    *a = (gfu_arena) {
        .start = start,
        .alignment = 16,
        .default_capacity_per_chunk = default_capacity_per_chunk,
    };
}

void gfu_arena_deinit(gfu_arena* a) {
    if (a == nullptr) return;

    gfu_chunk* c = a->start;
    while (c != nullptr) {
        gfu_chunk* next = c->next;
        free(c->memory);
        free(c);
        c = next;
    }

    *a = (gfu_arena) {0};
}

void* gfu_arena_alloc(gfu_arena* a, gfu_uword size) {
    assert(a != nullptr, "Can't allocate into a null arena.");

    const gfu_uword align = a->alignment;
    const gfu_uword padding = (align - (size % align)) % align;
    size += padding;

    for (gfu_chunk* c = a->start; c != nullptr; c = c->next) {
        gfu_uword available = c->capacity - c->allocated;
        if (size > available) {
            continue;
        }
        char* result = c->memory + c->allocated;
        c->allocated += size;
        return result;
    }

    gfu_uword capacity = gfu_max(a->default_capacity_per_chunk, size);
    char* memory = calloc((size_t)capacity, sizeof *memory);
    assert(memory != nullptr, "Failed to allocate arena backing memory.");

    gfu_chunk* chunk = calloc(1, sizeof *chunk);
    assert(chunk != nullptr, "Failed to allocate arena chunk.");

    chunk->memory = memory;
    chunk->allocated = size;
    chunk->capacity = capacity;

    chunk->next = a->start;
    a->start = chunk;
    return chunk->memory;
}
