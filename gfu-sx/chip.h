/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GFUSX_CHIP_H_
#define GFUSX_CHIP_H_

#include "prologue.h"

typedef union gfusx_chip_status {
    gfu_uword value;
    struct {
        gfu_uword reserved0 : 24;
        gfu_uword irq : 1;
        gfu_uword dma_request_state : 1;
        gfu_uword cmd_ready : 1;
        gfu_uword chip2cpu_ready : 1;
        gfu_uword dma_block_ready : 1;
        gfu_uword dma_direction : 2;
        gfu_uword reserved2 : 1;
    } bits;
} gfusx_chip_status_t;

gfu_static_assert(sizeof(gfusx_chip_status_t) == sizeof(gfu_uword), "Ensure that CHIP status is always word-sized.");

typedef struct gfusx_chip {
    gfu_ubyte* data;
    gfu_uword size, offset;
    gfusx_chip_status_t status;
} gfusx_chip_t;

bool gfusx_chip_init(gfusx_t* vm);
void gfusx_chip_deinit(gfusx_t* vm);
bool gfusx_chip_insert_rom_data(gfusx_t* vm, gfu_ubyte* data, gfu_uword size);
bool gfusx_chip_insert_rom_file(gfusx_t* vm, const char* rom_file_path);

void gfusx_chip_dma(gfusx_t* vm, gfu_uword madr, gfu_uword bcr, gfu_uword chcr);

gfu_uword gfusx_chip_read0(gfusx_t* vm);
gfu_uword gfusx_chip_read1(gfusx_t* vm);
void gfusx_chip_write0(gfusx_t* vm, gfu_uword value);
void gfusx_chip_write1(gfusx_t* vm, gfu_uword value);

void gfusx_chip_interrupt(gfusx_t* vm);
void gfusx_chip_read_interrupt(gfusx_t* vm);
void gfusx_chip_dma_interrupt(gfusx_t* vm);

#endif /* GFUSX_CHIP_H_ */
