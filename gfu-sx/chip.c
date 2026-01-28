/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include "prologue.h"
#include "chip.h"
#include "hardware.h"
#include "memory.h"
#include "vm.h"

#include <errno.h>
#include <stdio.h>

bool gfusx_chip_init(gfusx_t* vm) {
    vm->chip = (gfusx_chip_t){
        .data = vm->chip.data,
        .size = vm->chip.size,
    };
    return true;
}

void gfusx_chip_deinit(gfusx_t* vm) {
    gfusx_clear_irq(vm, 0);

    if (vm->chip.data != nullptr) {
        free(vm->chip.data);
    }

    vm->chip = (gfusx_chip_t){0};
}

bool gfusx_chip_insert_rom_data(gfusx_t* vm, gfu_ubyte* data, gfu_uword size) {
    vm->chip.data = data;
    vm->chip.size = size;

    vm->chip.status.bits.cmd_ready = 1;
    vm->chip.status.bits.dma_block_ready = 1;

    return true;
}

bool gfusx_chip_insert_rom_file(gfusx_t* vm, const char* rom_file_path) {
    bool result = true;
    char* rom_data = nullptr;

    errno = 0;
    FILE* rom_file = fopen(rom_file_path, "rb");
    if (rom_file == nullptr) {
        gfusx_logf(vm, "Failed to load ROM file '%s': %s.\n", rom_file_path, strerror(errno));
        return_defer(false);
    }

    errno = 0;
    if (fseek(rom_file, 0, SEEK_END) != 0) {
        gfusx_logf(vm, "Failed to read ROM file '%s': %s.\n", rom_file_path, strerror(errno));
        return_defer(false);
    }

    errno = 0;
    int64_t length = ftell(rom_file);
    if (length < 0) {
        gfusx_logf(vm, "Failed to read ROM file '%s': %s.\n", rom_file_path, strerror(errno));
        return_defer(false);
    }

    errno = 0;
    if (fseek(rom_file, 0, SEEK_SET) != 0) {
        gfusx_logf(vm, "Failed to read ROM file '%s': %s.\n", rom_file_path, strerror(errno));
        return_defer(false);
    }

    rom_data = calloc((size_t)length, 1);

    errno = 0;
    if ((size_t)length != fread(rom_data, (size_t)length, 1, rom_file) && ferror(rom_file)) {
        gfusx_logf(vm, "Failed to read ROM file '%s': %s.\n", rom_file_path, strerror(errno));
        return_defer(false);
    }

defer:;
    if (rom_file != nullptr) fclose(rom_file);
    if (!result) {
        if (rom_data != nullptr) free(rom_data);
        return false;
    }

    return gfusx_chip_insert_rom_data(vm, (gfu_ubyte*)rom_data, (gfu_uword)length);
}

void gfusx_chip_dma(gfusx_t* vm, gfu_uword madr, gfu_uword bcr, gfu_uword chcr) {
    gfusx_logf(vm, ">> DMA0 CHIP - %08X ADDR = %08X SIZE = %04X", chcr, madr, bcr);
    switch (chcr) {
        default: {
            gfusx_logf(vm, "  Unknown CHIP DMA CHCR %08X", chcr);
            gfusx_chip_dma_interrupt(vm);
        } break;

        case 0x11000000: {
            gfu_uword nwords = bcr & 0xFFFF;
            gfu_uword nbytes = nwords * 4;

            if (nbytes > vm->chip.size - vm->chip.offset) {
                nbytes = vm->chip.size - vm->chip.offset;
            }

            gfu_ubyte* mem_wptr = gfusx_memory_get_wptr(vm, madr);
            memcpy(mem_wptr, vm->chip.data + vm->chip.offset, (size_t)nbytes);

            gfu_uword cycle_count = nbytes / 16;
            gfusx_schedule_interrupt(vm, GFUSX_INT_CHIP_DMA, cycle_count);
        } break;
    }
}

gfu_uword gfusx_chip_read0(gfusx_t* vm) {
    return 0xFFFFFFFF;
}

gfu_uword gfusx_chip_read1(gfusx_t* vm) {
    gfu_uword status = vm->chip.status.value;
    return status;
}

void gfusx_chip_write0(gfusx_t* vm, gfu_uword value) {
    gfusx_memory_write_hwreg_word(vm, GFU_CHIP0_ADDR & 0xFFFF, value);
}

void gfusx_chip_write1(gfusx_t* vm, gfu_uword value) {
    gfu_uword cmd = (value >> 24) & 0xFF;
    // gfusx_memory_write_hwreg_word(vm, GFU_CHIP1_ADDR & 0xFFFF, value);

    switch (cmd) {
        default: {
            gfusx_logf(vm, "CHIP1(%02X) - Unknown command.", cmd);
        } break;

        case 0x00: {
            vm->chip = (gfusx_chip_t){
                .data = vm->chip.data,
                .size = vm->chip.size,
            };

            if (vm->chip.data != nullptr) {
                vm->chip.status.bits.cmd_ready = 1;
                vm->chip.status.bits.dma_block_ready = 1;
            }
        } break;

        case 0x02: {
            gfusx_clear_irq(vm, 0);
            vm->chip.status.bits.irq = 0;
        } break;

        case 0x03: {
            gfu_uword offset = value & 0x00FFFFFF;
            if (offset > vm->chip.size) {
                offset = vm->chip.size;
            }
            vm->chip.offset = offset;
        } break;

        case 0x04: {
            vm->chip.status.bits.dma_direction = value & 0x03;
        } break;
    }
}

void gfusx_chip_interrupt(gfusx_t* vm) {
    gfusx_log(vm, "TODO: gfusx_chip_interrupt");
}

void gfusx_chip_read_interrupt(gfusx_t* vm) {
    gfusx_log(vm, "TODO: gfusx_chip_read_interrupt");
}

void gfusx_chip_dma_interrupt(gfusx_t* vm) {
    if (!gfusx_memory_is_dma_busy(vm, 0)) {
        gfusx_log(vm, "DMA0 CHIP is busy.");
        return;
    }

    gfusx_log(vm, "DMA0 CHIP triggering.");
    gfusx_memory_clear_dma_busy(vm, 0);
    gfusx_memory_dma_interrupt(vm, 0);
}
