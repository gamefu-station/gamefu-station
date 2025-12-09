#if defined(__cplusplus)
#  include "No no no, silly silly. C++ is not a real programming language."
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#  include "Unsupported version of C. Expecting standard-compliant C99 or greater."
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 202000L
#  include "Unsupported version of C. C23 deprecates some earlier C standard features and we don't want to ifdef them."
#endif

#ifndef NDEBUG
#  define NDEBUG
#endif

#ifndef GAMEFU_PRELUDE_H_
#define GAMEFU_PRELUDE_H_

/*
 * C Compiler Identification
 */

#define GFU_COMPILER_CLANG 0
#define GFU_COMPILER_GCC 0

#if defined(__clang__)
#  undef GFU_COMPILER_CLANG
#  define GFU_COMPILER_CLANG 1
/// Many compilers define this for compatibility, so we'll have to explicitly check all of those when supported later.
#elif defined(__GNUC__)
#  undef GFU_COMPILER_GCC
#  define GFU_COMPILER_GCC 1
#else
// #  include "Unknown C compiler. Add explicit support for this compiler at the top of this C file."
#  warning "Unknown C compiler. Edit this header to add explicit support for it and silence this warning."
#endif

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

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAMEFU_API extern

#endif /* GAMEFU_PRELUDE_H_ */
