/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#ifndef __GAMEFU__TYPES_H__
#define __GAMEFU__TYPES_H__


#include <gamefu/host_features.h>

#include <stdint.h>


/* A signed 8-bit integer; a single byte. */
typedef int8_t gfu_byte;
/* An unsigned 8-bit integer; a single byte. */
typedef uint8_t gfu_ubyte;

/* A signed 16-bit integer; a half word. */
typedef int16_t gfu_half;
/* An unsigned 16-bit integer; a half word. */
typedef uint16_t gfu_uhalf;

/* A signed 32-bit integer; a word. */
typedef int32_t gfu_word;
/* An unsigned 32-bit integer; a word. */
typedef uint32_t gfu_uword;

/* A signed 64-bit integer; a double word. */
typedef int64_t gfu_double;
/* An unsigned 64-bit integer; a double word. */
typedef uint64_t gfu_udouble;


/* The minimum value of a signed byte (-128). */
#define GFU_BYTE_MIN  INT8_MIN
/* The maximum value of a signed byte (127). */
#define GFU_BYTE_MAX  INT8_MAX
/* The maximum value of an unsigned byte (255). */
#define GFU_UBYTE_MAX  UINT8_MAX

/* The minimum value of a signed half word (-32,768). */
#define GFU_HALF_MIN  INT16_MIN
/* The maximum value of a signed half word (32,767). */
#define GFU_HALF_MAX  INT16_MAX
/* The maximum value of an unsigned half word (65,535). */
#define GFU_UHALF_MAX  UINT16_MAX

/* The minimum value of a signed word (-2,147,483,648). */
#define GFU_WORD_MIN  INT32_MIN
/* The maximum value of a signed word (2,147,483,647). */
#define GFU_WORD_MAX  INT32_MAX
/* The maximum value of an unsigned word (4,294,967,295). */
#define GFU_UWORD_MAX  UINT32_MAX

/* The minimum value of a signed double word (-9,223,372,036,854,775,808). */
#define GFU_DOUBLE_MIN  INT64_MIN
/* The maximum value of a signed double word (9,223,372,036,854,775,807). */
#define GFU_DOUBLE_MAX  INT64_MAX
/* The maximum value of an unsigned double word (18,446,744,073,709,551,615). */
#define GFU_UDOUBLE_MAX  UINT64_MAX


#define gfu_max(X, Y) (((X) > (Y)) ? (X) : (Y))
#define gfu_min(X, Y) (((X) < (Y)) ? (X) : (Y))


static inline gfu_byte
gfu_minb(gfu_byte a, gfu_byte b) { return a < b ? a : b; }

static inline gfu_byte
gfu_maxb(gfu_byte a, gfu_byte b) { return a > b ? a : b; }

static inline gfu_ubyte
gfu_minub(gfu_ubyte a, gfu_ubyte b) { return a < b ? a : b; }

static inline gfu_ubyte
gfu_maxub(gfu_ubyte a, gfu_ubyte b) { return a > b ? a : b; }


static inline gfu_half
gfu_minh(gfu_half a, gfu_half b) { return a < b ? a : b; }

static inline gfu_half
gfu_maxh(gfu_half a, gfu_half b) { return a > b ? a : b; }

static inline gfu_uhalf
gfu_minuh(gfu_uhalf a, gfu_uhalf b) { return a < b ? a : b; }

static inline gfu_uhalf
gfu_maxuh(gfu_uhalf a, gfu_uhalf b) { return a > b ? a : b; }


static inline gfu_word
gfu_minw(gfu_word a, gfu_word b) { return a < b ? a : b; }

static inline gfu_word
gfu_maxw(gfu_word a, gfu_word b) { return a > b ? a : b; }

static inline gfu_uword
gfu_minuw(gfu_uword a, gfu_uword b) { return a < b ? a : b; }

static inline gfu_uword
gfu_maxuw(gfu_uword a, gfu_uword b) { return a > b ? a : b; }


static inline gfu_double
gfu_mind(gfu_double a, gfu_double b) { return a < b ? a : b; }

static inline gfu_double
gfu_maxd(gfu_double a, gfu_double b) { return a > b ? a : b; }

static inline gfu_udouble
gfu_minud(gfu_udouble a, gfu_udouble b) { return a < b ? a : b; }

static inline gfu_udouble
gfu_maxud(gfu_udouble a, gfu_udouble b) { return a > b ? a : b; }


#define GFU_BYTESWAP_HALF(V)  (((V) >> 8) | ((V) << 8))
#define GFU_BYTESWAP_WORD(V)  ( \
        (((V) >> 24) & 0x000000FF) | \
        (((V) >>  8) & 0x0000FF00) | \
        (((V) <<  8) & 0x00FF0000) | \
        (((V) << 24) & 0xFF000000) \
    )

#if defined(__linux__)
#  include <endian.h>

#  define gfu_htobe_half(Host)  (htobe16(Host))
#  define gfu_htole_half(Host)  (htole16(Host))
#  define gfu_betoh_half(Host)  (be16toh(Host))
#  define gfu_letoh_half(Host)  (le16toh(Host))

#  define gfu_htobe_word(Host)  (htobe32(Host))
#  define gfu_htole_word(Host)  (htole32(Host))
#  define gfu_betoh_word(Host)  (be32toh(Host))
#  define gfu_letoh_word(Host)  (le32toh(Host))
#elif (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN) || \
        defined(__BIG_ENDIAN__) || \
        defined(__ARMEB__) || \
        defined(__THUMBEB__) || \
        defined(__AARCH64EB__) || \
        defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
#  define gfu_htobe_half(Host)  (Host)
#  define gfu_htole_half(Host)  GFU_BYTESWAP_HALF(Host)
#  define gfu_betoh_half(Host)  (Host)
#  define gfu_letoh_half(Host)  GFU_BYTESWAP_HALF(Host)

#  define gfu_htobe_word(Host)  (Host)
#  define gfu_htole_word(Host)  GFU_BYTESWAP_WORD(Host)
#  define gfu_betoh_word(Host)  (Host)
#  define gfu_letoh_word(Host)  GFU_BYTESWAP_WORD(Host)
#elif (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN) || \
        defined(__LITTLE_ENDIAN__) || \
        defined(__ARMEL__) || \
        defined(__THUMBEL__) || \
        defined(__AARCH64EL__) || \
        defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
#  define gfu_htobe_half(Host)  GFU_BYTESWAP_HALF(Host)
#  define gfu_htole_half(Host)  (Host)
#  define gfu_betoh_half(Host)  GFU_BYTESWAP_HALF(Host)
#  define gfu_letoh_half(Host)  (Host)

#  define gfu_htobe_word(Host)  GFU_BYTESWAP_WORD(Host)
#  define gfu_htole_word(Host)  (Host)
#  define gfu_betoh_word(Host)  GFU_BYTESWAP_WORD(Host)
#  define gfu_letoh_word(Host)  (Host)
#else
#  error "Unknown endiannes or no platform layer to abstract to."
#endif


/*
Provides a grepable cast operation.
`GFU_CAST(int, foo)` is equivalent to the C code `((int) (foo))`.
*/
#define GFU_CAST(Ty, Value)  ((Ty) (Value))


/*
Sign extends an integer value from the first type, signed, to the second type,
unsigned.

This macro accepts the plain name of the GameFU integer type, such as "byte" or
"word" or "double" (without quotes) as its first and second parameters.
The first parameter specifies the desired width to sign extend from, while the
second parameter specifies the desired width to sign extend to.
The third parameter is the integer expression to sign extend.

As an example, the invocation `GFU_SEXT(byte, double, 0xFF)` performs the
following steps as C cast expressions:

    1. Cast the expression `0xFF` to the type `gfu_byte`, a signed 8-bit type.
    2. Cast the result of step 1 to `gfu_double`, a signed 64-bit type.
    3. Cast the result of step 2 to `gfu_udouble`, an unsigned 64-bit type.

Step 1 ensures that only the relevant bits contribute to the sign extension,
since C otherwise performs integer promotion in many cases which may produce
undesirable results.

Step 2 converts the smaller signed type to the larger signed type, taking
advantage of C's native sign extension abilities.
The resulting value is now of the correct bit width with the correct sign.

Step 3 converts the signed value to an unsigned value.
Since C supports sign extension natively with its cast operators, it is much
easier to manually sign extend a value when the target type is signed.
As such, this macro assumes the desired outcome is an unsigned integer.
*/
#define GFU_USEXT(InTy, OutTy, Value)  ( \
        GFU_CAST( \
            gfu_u##OutTy, \
            GFU_CAST( \
                gfu_##OutTy, \
                GFU_CAST(gfu_##InTy, (Value)) \
            ) \
        ) \
    )


#endif /* __GAMEFU__TYPES_H__ */
