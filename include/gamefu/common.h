/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef __GAMEFU_COMMON_H__
#define __GAMEFU_COMMON_H__


#include <gamefu/common/prelude.h>


__GAMEFU_C_HEADER_PROLOGUE__


#ifdef GFU_NDEBUG
#  define assert(Cond, Message) do { } while (0)
#  define assertf(Cond, Message, ...) do { } while (0)
#  define static_assert(Cond, Message)
#else /* !GFU_NDEBUG */
#  define assertn(Cond) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_PPSTR(__LINE__) \
                ": Assertion '" #Cond "' failed.\n"); \
            abort(); \
        } \
    } while (0)
#  define assert(Cond, Message) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_PPSTR(__LINE__) \
                ": Assertion '" #Cond "' failed:\n    " Message "\n"); \
            abort(); \
        } \
    } while (0)
#  define assertf(Cond, Message, ...) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_PPSTR(__LINE__) \
                ": Assertion '" #Cond "' failed:\n    " Message "\n", \
                __VA_ARGS__); \
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

#if __GAMEFU_HOSTCC_CLANG__ || __GAMEFU_HOSTCC_GCC__
#  define unreachable __builtin_unreachable()
#else
#  define unreachable do { assertn(false); } while (0)
#endif

#include <limits.h>

typedef int8_t gfu_byte;
typedef uint8_t gfu_ubyte;
typedef int16_t gfu_half;
typedef uint16_t gfu_uhalf;
typedef int32_t gfu_word;
typedef uint32_t gfu_uword;
typedef int64_t gfu_double;
typedef uint64_t gfu_udouble;

#define GFU_BYTE_MIN INT8_MIN
#define GFU_BYTE_MAX INT8_MAX
#define GFU_UBYTE_MAX UINT8_MAX

#define GFU_HALF_MIN INT16_MIN
#define GFU_HALF_MAX INT16_MAX
#define GFU_UHALF_MAX UINT16_MAX

#define GFU_WORD_MIN INT32_MIN
#define GFU_WORD_MAX INT32_MAX
#define GFU_UWORD_MAX UINT32_MAX

#define GFU_DOUBLE_MIN INT64_MIN
#define GFU_DOUBLE_MAX INT64_MAX
#define GFU_UDOUBLE_MAX UINT64_MAX

#define cast(T, V) ((T)(V))
#define sext(I, O, V) ((gfu_u##O)(gfu_##O)(gfu_##I)(V))

#define SWAP16(V) (V)
#define SWAP32(V) (V)

#define gfu_max(X, Y) (((X) > (Y)) ? (X) : (Y))
#define gfu_min(X, Y) (((X) < (Y)) ? (X) : (Y))

static inline gfu_byte gfu_minb(gfu_byte a, gfu_byte b) { return a < b ? a : b; }
static inline gfu_byte gfu_maxb(gfu_byte a, gfu_byte b) { return a > b ? a : b; }
static inline gfu_ubyte gfu_minub(gfu_ubyte a, gfu_ubyte b) { return a < b ? a : b; }
static inline gfu_ubyte gfu_maxub(gfu_ubyte a, gfu_ubyte b) { return a > b ? a : b; }

static inline gfu_half gfu_minh(gfu_half a, gfu_half b) { return a < b ? a : b; }
static inline gfu_half gfu_maxh(gfu_half a, gfu_half b) { return a > b ? a : b; }
static inline gfu_uhalf gfu_minuh(gfu_uhalf a, gfu_uhalf b) { return a < b ? a : b; }
static inline gfu_uhalf gfu_maxuh(gfu_uhalf a, gfu_uhalf b) { return a > b ? a : b; }

static inline gfu_word gfu_minw(gfu_word a, gfu_word b) { return a < b ? a : b; }
static inline gfu_word gfu_maxw(gfu_word a, gfu_word b) { return a > b ? a : b; }
static inline gfu_uword gfu_minuw(gfu_uword a, gfu_uword b) { return a < b ? a : b; }
static inline gfu_uword gfu_maxuw(gfu_uword a, gfu_uword b) { return a > b ? a : b; }

static inline gfu_double gfu_mind(gfu_double a, gfu_double b) { return a < b ? a : b; }
static inline gfu_double gfu_maxd(gfu_double a, gfu_double b) { return a > b ? a : b; }
static inline gfu_udouble gfu_minud(gfu_udouble a, gfu_udouble b) { return a < b ? a : b; }
static inline gfu_udouble gfu_maxud(gfu_udouble a, gfu_udouble b) { return a > b ? a : b; }

#define GFU_DA_INIT_CAP 1024
#define GFU_DA_FIELDS(Type) \
    Type* items;            \
    gfu_uword count;        \
    gfu_uword capacity

#define gfu_da_reserve(DA, MinCap)                                      \
    do {                                                                \
        if ((MinCap) > (DA)->capacity) {                                \
            gfu_uword new_cap = ((DA)->capacity == 0)                   \
                ? GFU_DA_INIT_CAP                                       \
                : (DA)->capacity * 2;                                   \
            while ((MinCap) > new_cap) {                                \
                new_cap *= 2;                                           \
            }                                                           \
            (DA)->items = realloc((DA)->items, new_cap * sizeof(*(DA)->items)); \
            (DA)->capacity = new_cap;                                   \
        }                                                               \
    } while (0)

#define gfu_da_push(DA, Item)                       \
    do {                                            \
        gfu_da_reserve((DA), (DA)->capacity + 1);   \
        (DA)->items[(DA)->count++] = (Item);        \
    } while(0)

#define gfu_da_push_many(DA, Items, Count)                              \
    do {                                                                \
        gfu_da_reserve((DA), (DA)->capacity + (Count));                 \
        memcpy((DA)->items + (DA)->count, (Items), (Count) * sizeof(*(Items))); \
        (DA)->count += (Count);                                         \
    } while(0)

#define gfu_da_free(DA)                         \
    do {                                        \
        assertn((DA) != nullptr);               \
        free((DA)->items);                      \
        memset((DA), 0, sizeof *(DA));          \
    } while(0);

typedef struct gfu_source {
    const char* name;
    const char* text;
    gfu_word length;
} gfu_source;

typedef struct gfu_sources {
    GFU_DA_FIELDS(gfu_source);
} gfu_sources;

__GAMEFU_API__ bool gfu_source_load_from_file(
    const char* path, gfu_source* source
);

#define GFU_NOSOURCE (struct gfu_source) {0}, 0

typedef enum gfu_diag_level {
    DIAG_IGNORED,
    DIAG_NOTE,
    DIAG_REMARK,
    DIAG_WARNING,
    DIAG_ERROR,
    DIAG_FATAL,
} gfu_diag_level;

__GAMEFU_API__ void gfu_diag_flush(__GAMEFU_VOIDPROTO__);
__GAMEFU_API__ void gfu_diag_pause_error_flush(__GAMEFU_VOIDPROTO__);
__GAMEFU_API__ void gfu_diag_color_output(bool enable);
__GAMEFU_API__ void gfu_diag_exit_on_error(bool enable);
__GAMEFU_API__ bool gfu_diag_has_issued_error(__GAMEFU_VOIDPROTO__);
__GAMEFU_API__ void gfu_diag_issue(
    gfu_diag_level level, gfu_source source, gfu_word location,
    const char* format, ...
);
__GAMEFU_API__ void gfu_diag_issue_v(
    gfu_diag_level level, gfu_source source, gfu_word location,
    const char* format, va_list v
);

typedef struct gfu_chunk {
    char* memory;
    gfu_uword capacity, allocated;
    struct gfu_chunk* next;
} gfu_chunk;

typedef struct gfu_arena {
    gfu_chunk* start;
    gfu_uword alignment;
    gfu_uword default_capacity_per_chunk;
} gfu_arena;

__GAMEFU_API__ void gfu_arena_init(
    gfu_arena* arena, gfu_uword default_capacity_per_chunk
);
__GAMEFU_API__ void gfu_arena_deinit(gfu_arena* arena);
__GAMEFU_API__ void* gfu_arena_alloc(gfu_arena* arena, gfu_uword size);


__GAMEFU_C_HEADER_EPILOGUE__


#endif /* __GAMEFU_COMMON_H__ */
