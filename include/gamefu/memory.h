#ifndef GAMEFU_MEMORY_H_
#define GAMEFU_MEMORY_H_

#include "../common/common.h"

typedef struct gfu_arena {
    char* memory;
    gfu_uword_t capacity, allocated;
    gfu_uword_t alignment;
} gfu_arena_t;

void gfu_arena_init(gfu_arena_t* arena, gfu_uword_t capacity);
void gfu_arena_deinit(gfu_arena_t* arena);
void* gfu_arena_alloc(gfu_arena_t* arena, gfu_uword_t size);

#endif /* GAMEFU_MEMORY_H_ */
