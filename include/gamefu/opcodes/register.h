/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GAMEFU_OPCODES_REGISTER_H_
#define GAMEFU_OPCODES_REGISTER_H_

#include <gamefu/common.h>

typedef enum gfu_gpr {
#define GPR(Id, Image) GFU_GPR_##Id,
#include "x/registers.h"
    GFU_GPR_INVALID = 0xFF,
} gfu_gpr;

typedef enum gfu_c0r {
#define C0R(Id, Image) GFU_C0R_##Id,
#include "x/registers.h"
    GFU_C0R_INVALID = 0xFF,
} gfu_c0r;

gfu_gpr gfu_gpr_from_string(const char* s, size_t length);
gfu_c0r gfu_c0r_from_string(const char* s, size_t length);

#endif /* GAMEFU_OPCODES_REGISTER_H_ */
