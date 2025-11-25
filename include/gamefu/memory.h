#ifndef GAMEFU_MEMORY_H_
#define GAMEFU_MEMORY_H_

#include <gamefu/prologue.h>

typedef struct gfu_arena {
    char* memory;
    gfu_uword_t capacity, allocated;
    gfu_uword_t alignment;
} gfu_arena_t;

void gfu_arena_init(gfu_arena_t* arena, gfu_uword_t capacity);
void gfu_arena_deinit(gfu_arena_t* arena);
void* gfu_arena_alloc(gfu_arena_t* arena, gfu_uword_t size);

#ifndef GFU_TEMP_SIZE
#  define GFU_TEMP_SIZE 1024 * 32
#endif

gfu_uword_t gfu_temp_mark(VOIDPROTO);
void gfu_temp_rewind(gfu_uword_t mark);
void gfu_temp_clear(VOIDPROTO);
void* gfu_temp_alloc(gfu_uword_t size);

#endif /* GAMEFU_MEMORY_H_ */
