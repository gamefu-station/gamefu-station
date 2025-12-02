#include "prologue.h"
#include "chip.h"
#include "hardware.h"
#include "memory.h"
#include "vm.h"

gfu_ubyte gfusx_hardware_read_byte(gfusx_t* vm, gfu_uword addr) {
    gfu_uword hwaddr = addr & 0x1FFFFFFF;
    gfu_ubyte value = 0xFFu;

    switch (hwaddr) {
        default: {
            gfusx_logf(vm, "Unknown 8-bit hardware read at address %08X.", addr);
            gfu_ubyte* hwptr = vm->memory.hard + (hwaddr & 0xFFFF);
            *hwptr = value;
            return value;
        }
    }

    gfusx_logf(vm, "Known 8-bit hardware read at address %08X.", addr);
    gfu_ubyte* hwptr = vm->memory.hard + (hwaddr & 0xFFFF);
    *hwptr = value;
    return value;
}

gfu_uhalf gfusx_hardware_read_half(gfusx_t* vm, gfu_uword addr) {
    gfu_uword hwaddr = addr & 0x1FFFFFFF;
    gfu_uhalf value = 0xFFFFu;

    switch (hwaddr) {
        case GFU_RAM_SIZE_ADDR: {
            value = gfusx_memory_read_hwreg_half(vm, hwaddr);
            gfusx_logf(vm, "RAM size 16-bit read of %08X.", value);
            return value;
        }

        case GFU_ISTAT_ADDR: {
            value = gfusx_memory_read_hwreg_half(vm, hwaddr);
            gfusx_logf(vm, "I_STAT 16-bit read of %08X.", value);
            return value;
        }

        default: {
            gfusx_logf(vm, "Unknown 16-bit hardware read at address %08X.", addr);
            gfu_uhalf* hwptr = cast(gfu_uhalf*, vm->memory.hard + (hwaddr & 0xFFFF));
            *hwptr = SWAP16(value);
            return value;
        }
    }

    gfusx_logf(vm, "Known 16-bit hardware read at address %08X.", addr);
    gfu_uhalf* hwptr = cast(gfu_uhalf*, vm->memory.hard + (hwaddr & 0xFFFF));
    *hwptr = SWAP16(value);
    return value;
}

gfu_uword gfusx_hardware_read_word(gfusx_t* vm, gfu_uword addr) {
    gfu_uword hwaddr = addr & 0x1FFFFFFF;
    gfu_uword value = 0xFFFFFFFFu;

    switch (hwaddr) {
        case GFU_RAM_SIZE_ADDR: {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "RAM size 32-bit read of %08X.", value);
            return value;
        }

        case GFU_ISTAT_ADDR: {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "I_STAT 32-bit read of %08X.", value);
            return value;
        }

        case GFU_DMA_MADR(0): {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "DMA0 (CHIP) MADR 32-bit read of %08X.", value);
            return value;
        }

        case GFU_DMA_BCR(0): {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "DMA0 (CHIP) BCR 32-bit read of %08X.", value);
            return value;
        }

        case GFU_DMA_CHCR(0): {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "DMA0 (CHIP) CHCR 32-bit read of %08X.", value);
            return value;
        }

        case GFU_DMA_MADR(1): {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "DMA1 (GPU) MADR 32-bit read of %08X.", value);
            return value;
        }

        case GFU_DMA_CHCR(1): {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "DMA1 (GPU) CHCR 32-bit read of %08X.", value);
            return value;
        }

        case GFU_DMA_DPCR: {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "DMA PCR 32-bit read of %08X.", value);
            return value;
        }

        case GFU_DMA_DICR: {
            value = gfusx_memory_read_hwreg_word(vm, hwaddr);
            gfusx_logf(vm, "DMA ICR 32-bit read of %08X.", value);
            return value;
        }

        case GFU_CHIP0_ADDR: {
            value = gfusx_chip_read0(vm);
            gfusx_logf(vm, "CHIP0 (Data) 32-bit read of %08X.", value);
            return value;
        }

        case GFU_CHIP1_ADDR: {
            value = gfusx_chip_read1(vm);
            gfusx_logf(vm, "CHIP1 (Status) 32-bit read of %08X.", value);
            return value;
        }

        default: {
            gfusx_logf(vm, "Unknown 32-bit hardware read at address %08X.", addr);
            gfu_uword* hwptr = cast(gfu_uword*, vm->memory.hard + (hwaddr & 0xFFFF));
            *hwptr = SWAP32(value);
            return value;
        }
    }

    gfusx_logf(vm, "Known 32-bit hardware read at address %08X.", addr);
    gfu_uword* hwptr = cast(gfu_uword*, vm->memory.hard + (hwaddr & 0xFFFF));
    *hwptr = SWAP32(value);
    return value;
}

void gfusx_hardware_write_byte(gfusx_t* vm, gfu_uword addr, gfu_ubyte value) {
    gfu_uword hwaddr = addr & 0x1FFFFFFF;

    switch (hwaddr) {
        case GFU_POST_ADDR: {
            gfusx_logf(vm, "BIOS POST Trace1: 0x%02X", value);
        } break;

        default: {
            gfusx_logf(vm, "Unknown 8-bit hardware write at address %02X of value %08X.", addr, value);
            gfu_ubyte* hwptr = vm->memory.hard + (hwaddr & 0xFFFF);
            *hwptr = value;
            return;
        }
    }

    gfusx_logf(vm, "Known 8-bit hardware write at address %08X of value %02X.", addr, value);
    gfu_ubyte* hwptr = vm->memory.hard + (hwaddr & 0xFFFF);
    *hwptr = value;
}

void gfusx_hardware_write_half(gfusx_t* vm, gfu_uword addr, gfu_uhalf value) {
    gfu_uword hwaddr = addr & 0x1FFFFFFF;

    switch (hwaddr) {
        case GFU_RAM_SIZE_ADDR: {
            gfusx_logf(vm, "RAM size 16-bit write of %04X.", value);
            gfusx_memory_write_hwreg_half(vm, hwaddr, value);
            gfusx_memory_set_luts(vm);
            return;
        }

        case GFU_ISTAT_ADDR: {
            gfusx_logf(vm, "I_STAT 16-bit write of %04X.", value);
            gfusx_memory_write_hwreg_half(vm, hwaddr, value);
            gfusx_clear_irq(vm, ~value);
            return;
        }

        case GFU_IMASK_ADDR: {
            gfusx_logf(vm, "I_MASK 16-bit write of %04X.", value);
            gfusx_memory_write_hwreg_half(vm, hwaddr, value);
            return;
        }

        default: {
            gfusx_logf(vm, "Unknown 16-bit hardware write at address %08X of value %04X.", addr, value);
            gfu_uhalf* hwptr = cast(gfu_uhalf*, vm->memory.hard + (hwaddr & 0xFFFF));
            *hwptr = SWAP16(value);
            return;
        }
    }

    gfusx_logf(vm, "Known 16-bit hardware write at address %08X of value %04X.", addr, value);
    gfu_uhalf* hwptr = cast(gfu_uhalf*, vm->memory.hard + (hwaddr & 0xFFFF));
    *hwptr = SWAP16(value);
}

void gfusx_hardware_write_word(gfusx_t* vm, gfu_uword addr, gfu_uword value) {
    gfu_uword hwaddr = addr & 0x1FFFFFFF;

    switch (hwaddr) {
        case GFU_RAM_SIZE_ADDR: {
            gfusx_logf(vm, "RAM size 32-bit write of %08X.", value);
            gfusx_memory_write_hwreg_word(vm, hwaddr, value & 0xFFFF);
            gfusx_memory_set_luts(vm);
            return;
        }

        case GFU_ISTAT_ADDR: {
            gfusx_logf(vm, "I_STAT 32-bit write of %08X.", value);
            gfu_uhalf half_value = (gfu_uhalf)value;
            gfusx_memory_write_hwreg_half(vm, hwaddr, half_value);
            gfusx_clear_irq(vm, ~half_value);
            return;
        }

        case GFU_IMASK_ADDR: {
            gfusx_logf(vm, "I_MASK 32-bit write of %08X.", value);
            gfu_uhalf half_value = (gfu_uhalf)value;
            gfusx_memory_write_hwreg_half(vm, hwaddr, half_value);
            return;
        }

        case GFU_DMA_MADR(0): {
            gfusx_logf(vm, "DMA0 (CHIP) MADR 32-bit write of %08X.", value);
            gfusx_memory_set_dma_madr(vm, 0, value);
            return;
        }

        case GFU_DMA_BCR(0): {
            gfusx_logf(vm, "DMA0 (CHIP) BCR 32-bit write of %08X.", value);
            gfusx_memory_set_dma_bcr(vm, 0, value);
            return;
        }

        case GFU_DMA_CHCR(0): {
            gfusx_logf(vm, "DMA0 (CHIP) CHCR 32-bit write of %08X.", value);
            gfusx_dma_exec(vm, 0, value);
            return;
        }

        case GFU_DMA_MADR(1): {
            gfusx_logf(vm, "DMA1 (GPU) MADR 32-bit write of %08X.", value);
            gfusx_memory_set_dma_madr(vm, 1, value);
            return;
        }

        case GFU_DMA_CHCR(1): {
            gfusx_logf(vm, "DMA1 (GPU) CHCR 32-bit write of %08X.", value);
            gfusx_dma_exec(vm, 1, value);
            return;
        }

        case GFU_DMA_DPCR: {
            gfusx_logf(vm, "DMA PCR 32-bit write of %08X.", value);
            gfusx_memory_write_hwreg_word(vm, hwaddr, value);
            return;
        }

        case GFU_DMA_DICR: {
            gfusx_logf(vm, "DMA ICR 32-bit write of %08X.", value);
            gfu_uword icr = gfusx_memory_read_hwreg_word(vm, hwaddr);
            // 01111111'00000000'00000000'00000000
            // 00000000'11111111'10000000'00111111
            gfu_uword ack = (value & 0x7F000000) ^ 0x7F000000;
            bool was_not_triggered = 0 == (icr & GFU_DMA_DICR_MASTER_ENABLE);
            bool is_triggered = false;
            bool has_error = 0 != (value & GFU_DMA_DICR_BUS_ERROR);
            bool is_enabled = 0 != (value & GFU_DMA_DICR_MASTER_ENABLE);
            value &= 0x00FF807F;
            gfusx_logf(vm, "  ICR %08X  ACK %08X  VALUE %08X", icr, ack, value);
            icr &= ack;
            icr |= value;
            gfusx_logf(vm, "  ICR = %08X.", icr);
            if (((icr & 0x7F008000) != 0) && (has_error || is_enabled)) {
                gfusx_log(vm, "DMA ICR rising-edge trigger.");
                icr |= 0x80000000;
                is_triggered = true;
            }
            gfusx_logf(vm, "  WAS_NOT %d  IS %d  ERROR %d  ENABLED %d", was_not_triggered, is_triggered, has_error, is_enabled);
            gfusx_memory_write_hwreg_word(vm, hwaddr, icr);
            if (was_not_triggered && is_triggered) {
                gfusx_log(vm, "Setting DMA IRQ.");
                gfusx_set_irq(vm, GFU_IRQ_DMA);
            }
            return;
        }

        case GFU_CHIP0_ADDR: {
            gfusx_logf(vm, "CHIP0 (Data) 32-bit write of %08X.", value);
            gfusx_chip_write0(vm, value);
            return;
        }

        case GFU_CHIP1_ADDR: {
            gfusx_logf(vm, "CHIP1 (Status) 32-bit write of %08X.", value);
            gfusx_chip_write1(vm, value);
            return;
        }

        default: {
            gfusx_logf(vm, "Unknown 32-bit hardware write at address %08X of value %08X.", addr, value);
            gfu_uword* hwptr = cast(gfu_uword*, vm->memory.hard + (hwaddr & 0xFFFF));
            *hwptr = SWAP32(value);
            return;
        }
    }

    gfusx_logf(vm, "Known 32-bit hardware write at address %08X of value %08X.", addr, value);
    gfu_uword* hwptr = cast(gfu_uword*, vm->memory.hard + (hwaddr & 0xFFFF));
    *hwptr = SWAP32(value);
}

void gfusx_set_irq(gfusx_t* vm, gfu_uword irq) {
    gfu_uword istat = gfusx_memory_read_hwreg_word(vm, GFU_ISTAT_ADDR & 0xFFFF);
    // gfusx_memory_write_hwreg_word(vm, GFU_ISTAT_ADDR & 0xFFFF, istat | irq);
    gfusx_hardware_write_word(vm, GFU_ISTAT_ADDR, istat | irq);
}

void gfusx_clear_irq(gfusx_t* vm, gfu_uword irq) {
    gfu_uword istat = gfusx_memory_read_hwreg_word(vm, GFU_ISTAT_ADDR & 0xFFFF);
    gfusx_memory_write_hwreg_word(vm, GFU_ISTAT_ADDR & 0xFFFF, istat & ~irq);
}

void gfusx_dma_exec(gfusx_t* vm, gfu_uhalf channel, gfu_uword chcr) {
    gfusx_memory_set_dma_chcr(vm, channel, chcr);
    bool start_transfer = chcr & 0x01000000;
    if (!start_transfer) return;

    gfu_uword dpcr = gfusx_memory_read_hwreg_word(vm, GFU_DMA_DPCR & 0xFFFF);
    bool is_dma_channel_enabled = GFU_DMA_DPCR_ENABLED(dpcr, channel);

    if (!is_dma_channel_enabled) {
        gfusx_logf(vm, "DMA%u exec (CHCR = 0x%08X): channel not enabled.", channel, chcr);
        return;
    }

    assert(start_transfer && is_dma_channel_enabled, "We should have early-exited if either condition was false.");
    gfusx_logf(vm, "Begin DMA%u exec", channel);

    gfu_uword madr = gfusx_memory_get_dma_madr(vm, channel);
    gfu_uword bcr = gfusx_memory_get_dma_bcr(vm, channel);
    gfu_uword mode = (chcr & 0x0600) >> 9;

    gfusx_logf(vm, "    MADR = 0x%08X", madr);
    gfusx_logf(vm, "    BCR  = 0x%08X", bcr);
    gfusx_logf(vm, "    CHCR = 0x%08X", chcr);
    gfusx_logf(vm, "    MODE = %d", mode);

    switch (channel) {
        default: {
            gfusx_logf(vm, "Unknown or unimplemented DMA channel %d.", channel);
        } break;

        case 0: gfusx_chip_dma(vm, madr, bcr, chcr); break;
    }
}
