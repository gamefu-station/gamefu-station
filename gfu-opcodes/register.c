/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/common.h>

#include <gamefu/opcodes/register.h>

static struct {
    gfu_gpr gpr;
    const char* name;
    size_t name_length;
} gfu_gpr_names[] = {
#define GPR(Id, Name) { GFU_GPR_##Id, "" Name "", sizeof(Name) - 1 },
#include <gamefu/opcodes/x/registers.h>
    {GFU_GPR_INVALID, nullptr, 0},
};

gfu_gpr gfu_gpr_from_string(const char* s, size_t length) {
    for (int i = 0; gfu_gpr_names[i].gpr != GFU_GPR_INVALID; i++) {
        if (length != gfu_gpr_names[i].name_length) continue;
        if (0 == strncmp(s, gfu_gpr_names[i].name, length)) {
            return gfu_gpr_names[i].gpr;
        }
    }

    return GFU_GPR_INVALID;
}

static struct {
    gfu_c0r cp0r;
    const char* name;
    size_t name_length;
} gfu_c0r_names[] = {
#define C0R(Id, Name) { GFU_C0R_##Id, "" Name "", sizeof(Name) - 1 },
#include <gamefu/opcodes/x/registers.h>
    {GFU_C0R_INVALID, nullptr, 0},
};

gfu_c0r gfu_c0r_from_string(const char* s, size_t length) {
    for (int i = 0; gfu_c0r_names[i].cp0r != GFU_C0R_INVALID; i++) {
        if (length != gfu_c0r_names[i].name_length) continue;
        if (0 == strncmp(s, gfu_c0r_names[i].name, length)) {
            return gfu_c0r_names[i].cp0r;
        }
    }

    return GFU_C0R_INVALID;
}
