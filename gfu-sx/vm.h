/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GFUSX_VM_H_
#define GFUSX_VM_H_

#include "prologue.h"
#include "chip.h"
#include "fu6109.h"
#include "memory.h"
#include "platform.h"

#define GFUSX_CYCLE_BIAS 2

struct gfusx {
    gfusx_platform_t* platform;
    gfusx_memory_t memory;
    gfusx_fu6109_t cpu;
    gfusx_chip_t chip;
    bool halt : 1;
    bool verbose : 1;
    bool trace : 1;
    bool headless : 1;
};

bool gfusx_vm_init(gfusx_t* vm);
void gfusx_vm_deinit(gfusx_t* vm);

void gfusx_schedule_interrupt(gfusx_t* vm, gfu_uword interrupt, gfu_uword cycle_count);

#endif /* GFUSX_VM_H_ */
