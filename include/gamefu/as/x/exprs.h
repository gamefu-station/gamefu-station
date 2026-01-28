/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef EXPR
#  define EXPR(Id)
#endif

EXPR(REG)
EXPR(IMM)
EXPR(IMM_UPPER)
EXPR(IMM_LOWER)
EXPR(ADDR)
EXPR(ADDR_UPPER)
EXPR(ADDR_LOWER)
EXPR(BYTE_STRING)

#undef EXPR
