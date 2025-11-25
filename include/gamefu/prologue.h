#if defined(__cplusplus)
#  error "No no no, silly silly. C++ is not a real programming language."
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#  error "Unsupported version of C. Expecting standard-compliant C99 or greater."
#endif

#ifndef GAMEFU_PROLOGUE_H_
#define GAMEFU_PROLOGUE_H_

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

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ < 202311L
#  define nullptr NULL
#  define VOIDPROTO void
#else
#  define VOIDPROTO
#endif

#define GFU_STR_(X) #X
#define GFU_STR(X) GFU_STR_(X)

#ifdef GFU_NDEBUG
#  define gfu_assert(Cond, Message) do { } while (0)
#  define gfu_assertf(Cond, Message, ...) do { } while (0)
#  define gfu_static_assert(Cond, Message)
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

#  if __STDC_VERSION__ < 202311L && !defined(static_assert)
#    define gfu_static_assert(Cond, Message) extern int (*_gfu_static_assert(VOIDPROTO))[!!sizeof(struct { int _error_if_negative[(Cond) ? 2 : -1]; })]
#  else
#    define gfu_static_assert(Cond, Message) static_assert(Cond, Message)
#  endif
#endif /* GFU_NDEBUG */

#if __STDC_VERSION__ >= 202300L
#  define gfu_thread_local thread_local
#elif __STDC_VERSION__ >= 201100L
#  define gfu_thread_local _Thread_local
#elif defined(__GNUC__) || defined(__clang__)
#  define gfu_thread_local __thread
#else
#  define gfu_thread_local
#endif

#define gfu_return_defer(Result) do { result = (Result); goto defer; } while (0)

#define gfu_cast(T, V) ((T)(V))
#define gfu_sext(I, O, V) ((gfu_u##O##_t)(gfu_##O##_t)(gfu_##I##_t)(V))

#define GFU_SWAP16(V) (V)
#define GFU_SWAP32(V) (V)

typedef int8_t gfu_byte_t;
typedef uint8_t gfu_ubyte_t;
typedef int16_t gfu_half_t;
typedef uint16_t gfu_uhalf_t;
typedef int32_t gfu_word_t;
typedef uint32_t gfu_uword_t;
typedef int64_t gfu_long_t;
typedef uint64_t gfu_ulong_t;
typedef bool gfu_bool_t;

#define gfu_true true
#define gfu_false false

#endif /* GAMEFU_PROLOGUE_H_ */
