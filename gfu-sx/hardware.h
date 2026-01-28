/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GFUSX_HARDWARE_H_
#define GFUSX_HARDWARE_H_

#include "prologue.h"

gfu_ubyte gfusx_hardware_read_byte(gfusx_t* vm, gfu_uword addr);
gfu_uhalf gfusx_hardware_read_half(gfusx_t* vm, gfu_uword addr);
gfu_uword gfusx_hardware_read_word(gfusx_t* vm, gfu_uword addr);
void gfusx_hardware_write_byte(gfusx_t* vm, gfu_uword addr, gfu_ubyte value);
void gfusx_hardware_write_half(gfusx_t* vm, gfu_uword addr, gfu_uhalf value);
void gfusx_hardware_write_word(gfusx_t* vm, gfu_uword addr, gfu_uword value);

void gfusx_set_irq(gfusx_t* vm, gfu_uword irq);
void gfusx_clear_irq(gfusx_t* vm, gfu_uword irq);

void gfusx_dma_exec(gfusx_t* vm, gfu_uhalf channel, gfu_uword chcr);

#endif /* GFUSX_HARDWARE_H_ */
