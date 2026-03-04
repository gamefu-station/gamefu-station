/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#ifndef __GAMEFU__TOKEN_H__
#define __GAMEFU__TOKEN_H__


typedef enum {
    FU_TK_INVALID = 0,
    FU_TK_EOF = 1,

    __GAMEFU_TK_SKIP_ASCII__ = 127,

    FU_ITK_KW_MATCH,
    FU_ITK_KW_EMITS,
    FU_ITK_KW_REG,
    FU_ITK_KW_IMM,
    FU_ITK_KW_ADDR,
    FU_ITK_KW_CP0,
    FU_ITK_KW_BYTES,
    FU_ITK_KW_STR,
    FU_ITK_KW_UPPER,
    FU_ITK_KW_LOWER,
    FU_ITK_KW_OFFS,
    FU_ITK_VAR,
    FU_ITK_OPCODE,
    FU_ITK_FUNCTION,
    FU_ITK_CP0_FUNCTION,
    FU_ITK_MNEMONIC,
    FU_ITK_REGISTER,
    FU_ITK_IMMEDIATE,

    FU_ATK_MNEMONIC,
} FUTokenKind;


#endif /* __GAMEFU__TOKEN_H__ */
