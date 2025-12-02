#ifndef GAMEFU_COMMON_H_
#define GAMEFU_COMMON_H_

#include "prelude.h"

#define nullptr NULL

#define PPSTR_(X) #X
#define PPSTR(X) PPSTR_(X)

#ifdef GFU_NDEBUG
#  define assert(Cond, Message) do { } while (0)
#  define assertf(Cond, Message, ...) do { } while (0)
#  define static_assert(Cond, Message)
#else /* !GFU_NDEBUG */
#  define assertn(Cond) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" PPSTR(__LINE__) ": Assertion '" #Cond "' failed.\n"); \
            abort(); \
        } \
    } while (0)
#  define assert(Cond, Message) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" PPSTR(__LINE__) ": Assertion '" #Cond "' failed:\n    " Message "\n"); \
            abort(); \
        } \
    } while (0)
#  define assertf(Cond, Message, ...) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" PPSTR(__LINE__) ": Assertion '" #Cond "' failed:\n    " Message "\n", __VA_ARGS__); \
            abort(); \
        } \
    } while (0)
#  ifdef _WIN32
#    undef static_assert
#  endif
#  if !defined(static_assert)
#    define static_assert(Cond, Message) extern int (*_static_assert(void))[!!sizeof(struct { int _error_if_negative[(Cond) ? 2 : -1]; })]
#  else
#    define static_assert(Cond, Message) static_assert(Cond, Message)
#  endif
#endif /* GFU_NDEBUG */

#define return_defer(Result) do { result = (Result); goto defer; } while (0)

#define cast(T, V) ((T)(V))
#define sext(I, O, V) ((gfu_u##O)(gfu_##O)(gfu_##I)(V))

#define SWAP16(V) (V)
#define SWAP32(V) (V)

typedef int8_t gfu_byte;
typedef uint8_t gfu_ubyte;
typedef int16_t gfu_half;
typedef uint16_t gfu_uhalf;
typedef int32_t gfu_word;
typedef uint32_t gfu_uword;
typedef int64_t gfu_long;
typedef uint64_t gfu_ulong;

typedef struct source {
    const char* name;
    const char* text;
    int32_t length;
} source;

bool load_source_from_file(const char* path, source* source);

#define NOSOURCE (struct source) {0}, 0

typedef enum diag_level {
    DIAG_IGNORED,
    DIAG_NOTE,
    DIAG_REMARK,
    DIAG_WARNING,
    DIAG_ERROR,
    DIAG_FATAL,
} diag_level;

void diag_flush(void);
void diag_pause_error_flush(void);
void diag_color_output(bool enable);
void diag_exit_on_error(bool enable);
bool diag_has_issued_error(void);
void diag_issue(diag_level level, source source, int32_t location, const char* format, ...);
void diag_issue_v(diag_level level, source source, int32_t location, const char* format, va_list v);

typedef struct arena {
    char* memory;
    gfu_uword capacity, allocated;
    gfu_uword alignment;
} arena;

void arena_init(arena* arena, gfu_uword capacity);
void arena_deinit(arena* arena);
void* arena_alloc(arena* arena, gfu_uword size);

#endif /* GAMEFU_COMMON_H_ */
