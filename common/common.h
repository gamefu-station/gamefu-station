#if defined(__cplusplus)
#  include "No no no, silly silly. C++ is not a real programming language."
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#  include "Unsupported version of C. Expecting standard-compliant C99 or greater."
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 202000L
#  include "Unsupported version of C. C23 deprecates some earlier C standard features and we don't want to ifdef them."
#endif

#ifndef GAMEFU_COMMON_COMMON_H_
#define GAMEFU_COMMON_COMMON_H_

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#  endif
#  undef _CRT_NONSTDC_NO_DEPRECATE
#  define _CRT_NONSTDC_NO_DEPRECATE 1
#  include <io.h>
#  define isatty _isatty
#  define fileno _fileno
#else
#  define _DEFAULT_SOURCE
#  ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200112L
#  endif
#endif

#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nullptr NULL

#define GFU_STR_(X) #X
#define GFU_STR(X) GFU_STR_(X)

#ifdef GFU_NDEBUG
#  define gfu_assert(Cond, Message) do { } while (0)
#  define gfu_assertf(Cond, Message, ...) do { } while (0)
#  define static_assert(Cond, Message)
#else /* !GFU_NDEBUG */
#  include <assert.h>

#  define gfu_assert(Cond, Message) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_STR(__LINE__) ": Assertion '" #Cond "' failed:\n    " Message "\n"); \
            abort(); \
        } \
    } while (0)

#  define gfu_assertf(Cond, Message, ...) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_STR(__LINE__) ": Assertion '" #Cond "' failed:\n    " Message "\n", __VA_ARGS__); \
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

#define fu_assert(Cond) \
    do { \
        if (!(Cond)) diag_issue(DIAG_FATAL, NOSOURCE, "%s:%d: Assertion failed: " #Cond, __FILE__, __LINE__); \
    } while (0)

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

typedef struct gfu_arena {
    char* memory;
    gfu_uword capacity, allocated;
    gfu_uword alignment;
} gfu_arena;

void gfu_arena_init(gfu_arena* arena, gfu_uword capacity);
void gfu_arena_deinit(gfu_arena* arena);
void* gfu_arena_alloc(gfu_arena* arena, gfu_uword size);

#endif /* GAMEFU_COMMON_COMMON_H_ */
