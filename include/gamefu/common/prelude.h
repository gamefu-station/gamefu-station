/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef __GAMEFU_COMMON__PRELUDE_H__
#define __GAMEFU_COMMON__PRELUDE_H__


/* -- Host Operating System Identification ---------------------------------- */


#undef __GAMEFU_HOSTOS_WINDOWS__
#define __GAMEFU_HOSTOS_WINDOWS__  (0)

#undef __GAMEFU_HOSTOS_LINUX__
#define __GAMEFU_HOSTOS_LINUX__  (0)


#if defined(_WINDOWS) || defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#  undef __GAMEFU_HOSTOS_WINDOWS__
#  define __GAMEFU_HOSTOS_WINDOWS__  (1)
#endif

#if defined(__linux__) || defined(__linux) || defined(linux)
#  undef __GAMEFU_HOSTOS_LINUX__
#  define __GAMEFU_HOSTOS_LINUX__  (1)
#endif


/* -- Target Platform Macro Configurations ---------------------------------- */


#if __GAMEFU_HOSTOS_WINDOWS__

#  if !defined(_CRT_SECURE_NO_WARNINGS)
#    define _CRT_SECURE_NO_WARNINGS
#  endif /* _CRT_SECURE_NO_WARNINGS */

#  if !defined(_CRT_NONSTDC_NO_DEPRECATE)
#    define _CRT_NONSTDC_NO_DEPRECATE  1
#  endif /* _CRT_NONSTDC_NO_DEPRECATE */

#  if !define(WIN32_LEAN_AND_MEAN)
#    define WIN32_LEAN_AND_MEAN
#  endif /* WIN32_LEAN_AND_MEAN */

#endif /* __GAMEFU_HOSTOS_WINDOWS__ */


#if __GAMEFU_HOSTOS_LINUX__

#  if !defined(_BSD_SOURCE)
#    define _BSD_SOURCE
#  endif /* _BSD_SOURCE */

#  if !defined(_SVID_SOURCE)
#    define _SVID_SOURCE
#  endif /* _SVID_SOURCE */

#  if !defined(_DEFAULT_SOURCE)
#    define _DEFAULT_SOURCE
#  endif /* _DEFAULT_SOURCE */

#  if !defined(_GNU_SOURCE)
#    define _GNU_SOURCE
#  endif /* _GNU_SOURCE */

#  if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE - 0 < 200809L)
#    undef _POSIX_C_SOURCE
#  endif /* _POSIX_C_SOURCE */
#  if !defined(_POSIX_C_SOURCE)
#    define _POSIX_C_SOURCE  200809L
#  endif /* _POSIX_C_SOURCE */

#endif /* __GAMEFU_HOSTOS_LINUX__ */


/* -- Compiler and Language Standard Identification ------------------------- */


#if defined(__cplusplus)
#  define __GAMEFU_STDCXX__  (1)
#  define __GAMEFU_STDCXX_VERSION__  __cplusplus
#  define __GAMEFU_C_HEADER_PROLOGUE__  extern "C" {
#  define __GAMEFU_C_HEADER_EPILOGUE__  }
#else
#  define __GAMEFU_STDCXX__  (0)
#  define __GAMEFU_STDCXX_VERSION__  (0)
#  define __GAMEFU_C_HEADER_PROLOGUE__
#  define __GAMEFU_C_HEADER_EPILOGUE__
#endif


#if defined(__STDC_VERSION__)
#  define __GAMEFU_STDC__  (1)
#  define __GAMEFU_STDC_VERSION__  __STDC_VERSION__
#else
#  define __GAMEFU_STDC__  (0)
#  define __GAMEFU_STDC_VERSION__  (0)
#endif


#define __GAMEFU_HOSTCC_CLANG__  (0)
#define __GAMEFU_HOSTCC_GCC__    (0)
#define __GAMEFU_HOSTCC_MSVC__   (0)

#if defined(__clang__)
#  undef __GAMEFU_HOSTCC_CLANG__
#  define __GAMEFU_HOSTCC_CLANG__  (1)
#elif defined(_MSC_VER)
#  undef __GAMEFU_HOSTCC_MSVC__
#  define __GAMEFU_HOSTCC_MSVC__  (1)
/*
NOTE: Many compilers define GCC macros such as __GNUC__ to indicate GNU
extension compatibility.
To prevent false positives it is therefore necessary to check for
__GNUC__ after all other compilers which could define it.
*/
#elif defined(__GNUC__)
#  undef __GAMEFU_HOSTCC_GCC__
#  define __GAMEFU_HOSTCC_GCC__  (1)
#endif


#define __GAMEFU_C89__  (0)
#define __GAMEFU_C90__  (0)
#define __GAMEFU_C94__  (0)
#define __GAMEFU_C99__  (0)
#define __GAMEFU_C11__  (0)
#define __GAMEFU_C17__  (0)
#define __GAMEFU_C23__  (0)
#define __GAMEFU_C2Y__  (0)

#define __GAMEFU_STDC_VERSION_C89__  198900L
#define __GAMEFU_STDC_VERSION_C90__  199000L
#define __GAMEFU_STDC_VERSION_C94__  199409L
#define __GAMEFU_STDC_VERSION_C99__  199901L
#define __GAMEFU_STDC_VERSION_C11__  201100L
#define __GAMEFU_STDC_VERSION_C17__  201700L
/*
NOTE: GCC, even years after C23 was standardized, may still define
__STDC_VERSION__ as 202000L.
*/
#define __GAMEFU_STDC_VERSION_C23__  202000L
/*
NOTE: At time of writing, C2y is still unreleased and in development. 202400L
is a reasonable minimum expectation.
*/
#define __GAMEFU_STDC_VERSION_C2Y__  202400L

#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C89__)
#  undef __GAMEFU_C89__
#  define __GAMEFU_C89__  (1)
#endif
#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C90__)
#  undef __GAMEFU_C90__
#  define __GAMEFU_C90__  (1)
#endif
#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C94__)
#  undef __GAMEFU_C94__
#  define __GAMEFU_C94__  (1)
#endif
#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C99__)
#  undef __GAMEFU_C99__
#  define __GAMEFU_C99__  (1)
#endif
#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C11__)
#  undef __GAMEFU_C11__
#  define __GAMEFU_C11__  (1)
#endif
#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C17__)
#  undef __GAMEFU_C17__
#  define __GAMEFU_C17__  (1)
#endif
#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C23__)
#  undef __GAMEFU_C23__
#  define __GAMEFU_C23__  (1)
#endif
#if (__GAMEFU_STDC_VERSION__ - 0 >= __GAMEFU_STDC_VERSION_C2Y__)
#  undef __GAMEFU_C2Y__
#  define __GAMEFU_C2Y__  (1)
#endif


#define __GAMEFU_CXX98__  (0)
#define __GAMEFU_CXX03__  (0)
#define __GAMEFU_CXX11__  (0)
#define __GAMEFU_CXX14__  (0)
#define __GAMEFU_CXX17__  (0)
#define __GAMEFU_CXX23__  (0)

#define __GAMEFU_STDCXX_VERSION_CXX98__  199711L
#define __GAMEFU_STDCXX_VERSION_CXX03__  199711L
#define __GAMEFU_STDCXX_VERSION_CXX11__  201103L
#define __GAMEFU_STDCXX_VERSION_CXX14__  201103L
#define __GAMEFU_STDCXX_VERSION_CXX17__  201103L
#define __GAMEFU_STDCXX_VERSION_CXX23__  201103L

#if (__GAMEFU_STDCXX_VERSION__ - 0 >= __GAMEFU_STDCXX_VERSION_CXX98__)
#  undef __GAMEFU_CXX98__
#  define __GAMEFU_CXX98__  (1)
#endif
#if (__GAMEFU_STDCXX_VERSION__ - 0 >= __GAMEFU_STDCXX_VERSION_CXX03__)
#  undef __GAMEFU_CXX03__
#  define __GAMEFU_CXX03__  (1)
#endif
#if (__GAMEFU_STDCXX_VERSION__ - 0 >= __GAMEFU_STDCXX_VERSION_CXX11__)
#  undef __GAMEFU_CXX11__
#  define __GAMEFU_CXX11__  (1)
#endif
#if (__GAMEFU_STDCXX_VERSION__ - 0 >= __GAMEFU_STDCXX_VERSION_CXX14__)
#  undef __GAMEFU_CXX14__
#  define __GAMEFU_CXX14__  (1)
#endif
#if (__GAMEFU_STDCXX_VERSION__ - 0 >= __GAMEFU_STDCXX_VERSION_CXX17__)
#  undef __GAMEFU_CXX17__
#  define __GAMEFU_CXX17__  (1)
#endif
#if (__GAMEFU_STDCXX_VERSION__ - 0 >= __GAMEFU_STDCXX_VERSION_CXX23__)
#  undef __GAMEFU_CXX23__
#  define __GAMEFU_CXX23__  (1)
#endif


#if __GAMEFU_C23__ || __GAMEFU_STDCXX__
#  define __GAMEFU_VOIDPROTO__
#else
#  define __GAMEFU_VOIDPROTO__  void
#endif


/* -- Language Agnostic Declaration Attributes ------------------------------ */


#if __GAMEFU_STDCXX__
#  define __GAMEFU_EXTERN__  extern "C"
#else
#  define __GAMEFU_EXTERN__  extern
#endif


#if __GAMEFU_HOSTOS_WINDOWS__
/*
NOTE: CMake defines {libname}_EXPORTS for shared libraries by default.
While CMake is not the default build system provided by this project,
it doesn't hurt to use GAMEFU_EXPORTS rather than a bespoke, more
explicit macro.
We'd otherwise be checking at least two potential configuration macros
or requiring extra work of anyone writing their own CMake build script
for GAMEFU. */
#  if defined(GAMEFU_EXPORTS)
#    define __GAMEFU_API__  __GAMEFU_EXTERN__ __declspec(dllexport)
#  else
#    define __GAMEFU_API__  __GAMEFU_EXTERN__ __declspec(dllimport)
#  endif
#else
#  define __GAMEFU_API__  __GAMEFU_EXTERN__
#endif


#if __GAMEFU_C23__ || __GAMEFU_CXX__
#  define __GAMEFU_HAS_NORETURN__  (1)
#  define __GAMEFU_NORETURN__  [[noreturn]]
#elif __GAMEFU_C11__
#  define __GAMEFU_HAS_NORETURN__  (1)
#  define __GAMEFU_NORETURN__  _Noreturn
#elif __GAMEFU_HOSTCC_CLANG__ || __GAMEFU_HOSTCC_GCC__
#  define __GAMEFU_HAS_NORETURN__  (1)
#  define __GAMEFU_NORETURN__  __attribute__((noreturn))
#else
#  define __GAMEFU_HAS_NORETURN__  (0)
#  define __GAMEFU_NORETURN__
#endif


#if __GAMEFU_C23__ || __GAMEFU_CXX__
#  define __GAMEFU_NODISCARD__  [[nodiscard]]
#  define __GAMEFU_NODISCARD_REASON__(Why)  [[nodiscard("" Why "")]]
#elif __GAMEFU_HOSTCC_CLANG__ || __GAMEFU_HOSTCC_GCC__
#  define __GAMEFU_NODISCARD__  __attribute__((warn_unused_result))
#  define __GAMEFU_NODISCARD_REASON__(Why)  __attribute__((warn_unused_result))
#else
#  define __GAMEFU_NODISCARD__
#  define __GAMEFU_NODISCARD_REASON__(Why)
#endif


#if __GAMEFU_C23__ || __GAMEFU_CXX__
#  define __GAMEFU_DEPRECATED__(Why)  [[deprecated("" Why "")]]
#elif __GAMEFU_HOSTCC_CLANG__ || __GAMEFU_HOSTCC_GCC__
#  define __GAMEFU_DEPRECATED__(Why)  __attribute__((deprecated ("" Why "")))
#else
#  define __GAMEFU_DEPRECATED__(Why)
#endif


/* -- The Actual Fucking Header File ---------------------------------------- */


__GAMEFU_C_HEADER_PROLOGUE__


#if defined(NDEBUG)
#  define __GAMEFU_DEBUG__  (0)
#else
#  define __GAMEFU_DEBUG__  (1)
#endif


#if __GAMEFU_HOSTOS_WINDOWS__
#  include <io.h>
#endif


#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if __GAMEFU_HOSTOS_WINDOWS__
#  define isatty _isatty
#  define fileno _fileno
#endif


#if !__GAMEFU_C23__ && !__GAMEFU_CXX11__ && !defined(nullptr)
#  define nullptr  NULL
#endif


#if __GAMEFU_C23__
#  define unreachable  do { [[unreachable]]; } while (0)
#elif __GAMEFU_HOSTCC_CLANG__ || __GAMEFU_HOSTCC_GCC__
#  define unreachable  do { __builtin_unreachable(); } while (0)
#elif __GAMEFU_HOSTCC_MSVC__
#  define unreachable  do { __assume(false); } while (0)
#else
#  if __GAMEFU_HAS_NORETURN__
__GAMEFU_NORETURN__ inline void __gfu_unreachable__(__GAMEFU_VOIDPROTO__) { }
#    define unreachable  do { __gfu_unreachable__(); } while (0)
#  else
#    define unreachable  do { gfu_assertn(false); } while (0)
#  endif /* __GAMEFU_HAS_NORETURN__ */
#endif


#define GFU_PPSTR1(X) #X
#define GFU_PPSTR(X) GFU_PPSTR1(X)


#if __GAMEFU_DEBUG__
#  define gfu_assert(Cond, Message) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_PPSTR(__LINE__) \
                ": Assertion '" #Cond "' failed:\n    " Message "\n"); \
            abort(); \
        } \
    } while (0)

#  define gfu_assertn(Cond) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_PPSTR(__LINE__) \
                ": Assertion '" #Cond "' failed.\n"); \
            abort(); \
        } \
    } while (0)

#  define gfu_assertf(Cond, Message, ...) do { \
        if (!(Cond)) { \
            (void)fprintf(stderr, __FILE__ ":" GFU_PPSTR(__LINE__) \
                ": Assertion '" #Cond "' failed:\n    " Message "\n", \
                __VA_ARGS__); \
            abort(); \
        } \
    } while (0)

#  if __CINDER_C23__
#    define gfu_static_assert(Cond, Message) static_assert((Cond), (Message))
#  elif __CINDER_C11__
#    define gfu_static_assert(Cond, Message) _Static_assert((Cond), (Message))
#  else
#    define gfu_static_assert(Cond, Message) \
    extern int (*__gamefu_static_assert__(void))[\
        !!sizeof(struct { int _error_if_negative[(Cond) ? 2 : -1]; }) \
    ]
#  endif /* !__CINDER_C11__ */
#else
#  define gfu_assert(Cond, Message) do { } while (0)
#  define gfu_assertn(Cond) do { } while (0)
#  define gfu_assertf(Cond, Message, ...) do { } while (0)
#  define gfu_static_assert(Cond, Message)
#endif


/* TODO(echoe): Ideally we'd like to remove `return_defer`. */
#define return_defer(Result) do { result = (Result); goto defer; } while (0)


__GAMEFU_C_HEADER_EPILOGUE__


#endif /* __GAMEFU_COMMON__PRELUDE_H__ */
