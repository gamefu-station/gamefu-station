/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GFUSX_MEMORY_H_
#define GFUSX_MEMORY_H_

#include <gamefu/common.h>

#include "prologue.h"

typedef struct gfusx_memory {
    gfu_ubyte* wram;
    gfu_ubyte* bios;
    gfu_ubyte* hard;

    gfu_ubyte** rlut;
    gfu_ubyte** wlut;
} gfusx_memory_t;

bool gfusx_memory_init(gfusx_t* vm);
void gfusx_memory_reset(gfusx_t* vm);
void gfusx_memory_deinit(gfusx_t* vm);
void gfusx_memory_set_luts(gfusx_t* vm);

gfu_ubyte gfusx_memory_read_byte(gfusx_t* vm, gfu_uword addr);
gfu_uhalf gfusx_memory_read_half(gfusx_t* vm, gfu_uword addr);
gfu_uword gfusx_memory_read_word(gfusx_t* vm, gfu_uword addr);
gfu_uword gfusx_memory_read_inst(gfusx_t* vm, gfu_uword addr);
void gfusx_memory_write_byte(gfusx_t* vm, gfu_uword addr, gfu_ubyte value);
void gfusx_memory_write_half(gfusx_t* vm, gfu_uword addr, gfu_uhalf value);
void gfusx_memory_write_word(gfusx_t* vm, gfu_uword addr, gfu_uword value);

gfu_ubyte gfusx_memory_read_hwreg_byte(gfusx_t* vm, gfu_uhalf reg);
gfu_uhalf gfusx_memory_read_hwreg_half(gfusx_t* vm, gfu_uhalf reg);
gfu_uword gfusx_memory_read_hwreg_word(gfusx_t* vm, gfu_uhalf reg);
void gfusx_memory_write_hwreg_byte(gfusx_t* vm, gfu_uhalf reg, gfu_ubyte value);
void gfusx_memory_write_hwreg_half(gfusx_t* vm, gfu_uhalf reg, gfu_uhalf value);
void gfusx_memory_write_hwreg_word(gfusx_t* vm, gfu_uhalf reg, gfu_uword value);

gfu_ubyte* gfusx_memory_get_rptr(gfusx_t* vm, gfu_uword addr);
gfu_ubyte* gfusx_memory_get_wptr(gfusx_t* vm, gfu_uword addr);

gfu_uword gfusx_memory_get_dma_madr(gfusx_t* vm, gfu_uhalf channel);
gfu_uword gfusx_memory_get_dma_bcr(gfusx_t* vm, gfu_uhalf channel);
gfu_uword gfusx_memory_get_dma_chcr(gfusx_t* vm, gfu_uhalf channel);
void gfusx_memory_set_dma_madr(gfusx_t* vm, gfu_uhalf channel, gfu_uword madr);
void gfusx_memory_set_dma_bcr(gfusx_t* vm, gfu_uhalf channel, gfu_uword bcr);
void gfusx_memory_set_dma_chcr(gfusx_t* vm, gfu_uhalf channel, gfu_uword chcr);

bool gfusx_memory_is_dma_busy(gfusx_t* vm, gfu_uhalf channel);
void gfusx_memory_set_dma_busy(gfusx_t* vm, gfu_uhalf channel);
void gfusx_memory_clear_dma_busy(gfusx_t* vm, gfu_uhalf channel);
void gfusx_memory_dma_interrupt(gfusx_t* vm, gfu_uhalf channel);

#endif /* GFUSX_MEMORY_H_ */
