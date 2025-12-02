#include "../common/common.h"

#include "../gfu-bfd/object.h"
#include "../gfu-bfd/system.h"

#include "prologue.h"
#include "memory.h"
#include "vm.h"

#include "default_bios.h"
#include "hardware.h"

bool gfusx_memory_init(gfusx_t* vm) {
    assert(vm != nullptr, "Where is the VM?");

    gfu_ubyte* wram = nullptr;
    gfu_ubyte* bios = nullptr;
    gfu_ubyte* hard = nullptr;

    gfu_ubyte** rlut = nullptr;
    gfu_ubyte** wlut = nullptr;

    wram = calloc(GFU_WRAM_SIZE, sizeof *wram);
    if (wram == nullptr) {
        // TODO(echoe): Report memory allocation failure.
        goto error;
    }

    bios = calloc(GFU_BIOS_ROM_SIZE, sizeof *bios);
    if (bios == nullptr) {
        // TODO(echoe): Report memory allocation failure.
        goto error;
    }

    hard = calloc(GFU_EXP1_HWREG_SIZE, sizeof *hard);
    if (hard == nullptr) {
        // TODO(echoe): Report memory allocation failure.
        goto error;
    }

    rlut = calloc(0x10000, sizeof *rlut);
    if (rlut == nullptr) {
        // TODO(echoe): Report memory allocation failure.
        goto error;
    }

    wlut = calloc(0x10000, sizeof *wlut);
    if (wlut == nullptr) {
        // TODO(echoe): Report memory allocation failure.
        goto error;
    }

    // The BIOS ROM pages will always be mapped
    const gfu_uword bios_pages_kseg0 = (GFU_BIOS_ROM_BASE + GFU_KSEG0_BASE) >> 16;
    const gfu_uword bios_pages_kseg1 = (GFU_BIOS_ROM_BASE + GFU_KSEG1_BASE) >> 16;
    for (gfu_uword i = 0; i < GFU_BIOS_ROM_SIZE >> 16; i++) {
        rlut[i + bios_pages_kseg0] = rlut[i + bios_pages_kseg1] = &bios[i << 16];
        wlut[i + bios_pages_kseg0] = wlut[i + bios_pages_kseg1] = &bios[i << 16];
    }

    vm->memory = (gfusx_memory_t) {
        .wram = wram,
        .bios = bios,
        .hard = hard,
        .rlut = rlut,
        .wlut = wlut,
    };

    gfusx_memory_set_luts(vm);
    return true;

error:;
    if (wram != nullptr) free(wram);
    if (bios != nullptr) free(bios);
    if (hard != nullptr) free(hard);

    if (rlut != nullptr) free(rlut);
    if (wlut != nullptr) free(wlut);

    return false;
}

void gfusx_memory_reset(gfusx_t* vm) {
    assert(vm != nullptr, "Where is the VM?");

    gfu_ubyte* wram = vm->memory.wram;
    gfu_ubyte* bios = vm->memory.bios;
    gfu_ubyte* hard = vm->memory.hard;

    gfu_ubyte** rlut = vm->memory.rlut;
    gfu_ubyte** wlut = vm->memory.wlut;

    assert(wram != nullptr, "Where is WRAM?");
    assert(bios != nullptr, "Where is BIOS ROM?");
    assert(hard != nullptr, "Where are the hardware registers?");

    assert(rlut != nullptr, "Where is the RLUT?");
    assert(wlut != nullptr, "Where is the WLUT?");

    memset(wram, 0, GFU_WRAM_SIZE);
    memset(bios, 0, GFU_BIOS_ROM_SIZE);

    // Load the default BIOS into its place in ROM.
    assert(gfusx_default_bios_len <= GFU_BIOS_ROM_SIZE, "Default BIOS is way too big, somehow.");
    memcpy(bios, gfusx_default_bios + sizeof(gfuobj_header_t), (size_t)gfusx_default_bios_len - sizeof(gfuobj_header_t));
}

void gfusx_memory_deinit(gfusx_t* vm) {
    assert(vm != nullptr, "Where is the VM?");

    free(vm->memory.wram);
    free(vm->memory.bios);
    free(vm->memory.hard);

    free(vm->memory.rlut);
    free(vm->memory.wlut);
}

void gfusx_memory_set_luts(gfusx_t* vm) {
    assert(vm != nullptr, "Where is the VM?");

    gfu_ubyte* wram = vm->memory.wram;
    gfu_ubyte* bios = vm->memory.bios;
    gfu_ubyte* hard = vm->memory.hard;

    gfu_ubyte** rlut = vm->memory.rlut;
    gfu_ubyte** wlut = vm->memory.wlut;

    assert(wram != nullptr, "Where is WRAM?");
    assert(bios != nullptr, "Where is BIOS ROM?");
    assert(hard != nullptr, "Where are the hardware registers?");

    assert(rlut != nullptr, "Where is the RLUT?");
    assert(wlut != nullptr, "Where is the WLUT?");

    // WRAM sizse is controled by the RAM_SIZE I/O port and re-mapped based on its value.
    // By default it is the "small" size unless the "large" size is specified.
    gfu_uword main_ram_max;
    if (gfusx_memory_read_hwreg_half(vm, GFU_RAM_SIZE_ADDR & 0xFFFF) == GFU_RAM_SIZE_LARGE) {
        main_ram_max = GFU_WRAM_SIZE >> 16;
    } else {
        main_ram_max = (GFU_BIOS_RAM_SIZE + GFU_MAIN0_RAM_SIZE) >> 16;
    }

    const gfu_uword wram_pages_kseg0 = (GFU_WRAM_BASE + GFU_KSEG0_BASE) >> 16;
    const gfu_uword wram_pages_kseg1 = (GFU_WRAM_BASE + GFU_KSEG1_BASE) >> 16;
    for (gfu_uword i = 0; i < main_ram_max; i++) {
        rlut[i + wram_pages_kseg0] = rlut[i + wram_pages_kseg1] = &wram[i << 16];
        wlut[i + wram_pages_kseg0] = wlut[i + wram_pages_kseg1] = &wram[i << 16];
    }

    // Unmap any WRAM addresses that aren't valid in the current configuration.
    memset(&rlut[wram_pages_kseg0 + main_ram_max], 0, (GFU_WRAM_SIZE >> 16) - main_ram_max);
    memset(&rlut[wram_pages_kseg1 + main_ram_max], 0, (GFU_WRAM_SIZE >> 16) - main_ram_max);

    memset(&wlut[wram_pages_kseg0 + main_ram_max], 0, (GFU_WRAM_SIZE >> 16) - main_ram_max);
    memset(&wlut[wram_pages_kseg1 + main_ram_max], 0, (GFU_WRAM_SIZE >> 16) - main_ram_max);
}

gfu_ubyte gfusx_memory_read_byte(gfusx_t* vm, gfu_uword addr) {
    assert(vm != nullptr, "Where is the VM?");
    vm->cpu.cycle += 1;

    gfu_ubyte** rlut = vm->memory.rlut;
    assert(rlut != nullptr, "Where is the RLUT?");

    const gfu_uword page = addr >> 16;
    const gfu_ubyte* page_ptr = rlut[page];

    if (page_ptr != nullptr) {
        const gfu_uword offset = addr & 0xFFFF;
        const gfu_ubyte* addr_ptr = page_ptr + offset;
        return *addr_ptr;
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            const gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            const gfu_uword offset = (addr & 0xFFFF) & (GFU_EXP1_SCRATCH_SIZE - 1);
            const gfu_ubyte* hard_ptr = hard + offset;
            return *hard_ptr;
        }

        return gfusx_hardware_read_byte(vm, addr);
    }

    // Failed to map the address to anything valid.
    // Return a nice dummy value no matter what happened above.
    return 0xFFu;
}

gfu_uhalf gfusx_memory_read_half(gfusx_t* vm, gfu_uword addr) {
    assert(vm != nullptr, "Where is the VM?");
    vm->cpu.cycle += 1;

    gfu_ubyte** rlut = vm->memory.rlut;
    assert(rlut != nullptr, "Where is the RLUT?");

    gfu_uword page = addr >> 16;
    gfu_ubyte* page_ptr = rlut[page];

    if (page_ptr != nullptr) {
        gfu_uword offset = addr & 0xFFFE;
        gfu_uhalf* addr_ptr = cast(gfu_uhalf*, page_ptr + offset);
        return SWAP16(*addr_ptr);
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            gfu_uword offset = (addr & 0xFFFE) & (GFU_EXP1_SCRATCH_SIZE - 1);
            gfu_uhalf* hard_ptr = cast(gfu_uhalf*, hard + offset);
            return SWAP16(*hard_ptr);
        }

        return gfusx_hardware_read_half(vm, addr);
    }

    // Failed to map the address to anything valid.
    // Return a nice dummy value no matter what happened above.
    return 0xFFFFu;
}

static gfu_uword gfusx_memory_read_word_impl(gfusx_t* vm, gfu_uword addr) {
    assert(vm != nullptr, "Where is the VM?");
    // NOTE(echoe): This is a special internal implementation for read.
    // Let the caller decide if we should cost a cycle.
    // vm->cpu.cycle += 1;

    gfu_ubyte** rlut = vm->memory.rlut;
    assert(rlut != nullptr, "Where is the RLUT?");

    gfu_uword page = addr >> 16;
    gfu_ubyte* page_ptr = rlut[page];

    if (page_ptr != nullptr) {
        gfu_uword offset = addr & 0xFFFC;
        gfu_uword* addr_ptr = cast(gfu_uword*, page_ptr + offset);
        return SWAP32(*addr_ptr);
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            gfu_uword offset = (addr & 0xFFFC) & (GFU_EXP1_SCRATCH_SIZE - 1);
            gfu_uword* hard_ptr = cast(gfu_uword*, hard + offset);
            return SWAP32(*hard_ptr);
        }

        return gfusx_hardware_read_word(vm, addr);
    }

    // Failed to map the address to anything valid.
    // Return a nice dummy value no matter what happened above.
    return 0xFFFFFFFFu;
}

gfu_uword gfusx_memory_read_word(gfusx_t* vm, gfu_uword addr) {
    assert(vm != nullptr, "Where is the VM?");
    vm->cpu.cycle += 1;
    return gfusx_memory_read_word_impl(vm, addr);
}

gfu_uword gfusx_memory_read_inst(gfusx_t* vm, gfu_uword addr) {
    assert(vm != nullptr, "Where is the VM?");
    return gfusx_memory_read_word_impl(vm, addr);
}

void gfusx_memory_write_byte(gfusx_t* vm, gfu_uword addr, gfu_ubyte value) {
    assert(vm != nullptr, "Where is the VM?");
    vm->cpu.cycle += 1;

    gfu_ubyte** wlut = vm->memory.wlut;
    assert(wlut != nullptr, "Where is the WLUT?");

    gfu_uword page = addr >> 16;
    gfu_ubyte* page_ptr = wlut[page];

    if (page_ptr != nullptr) {
        gfu_uword offset = addr & 0xFFFF;
        gfu_ubyte* addr_ptr = page_ptr + offset;
        *addr_ptr = value;
        return;
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            gfu_uword offset = (addr & 0xFFFF) & (GFU_EXP1_SCRATCH_SIZE - 1);
            gfu_ubyte* hard_ptr = hard + offset;
            *hard_ptr = value;
            return;
        }

        gfusx_hardware_write_byte(vm, addr, value);
    }
}

void gfusx_memory_write_half(gfusx_t* vm, gfu_uword addr, gfu_uhalf value) {
    assert(vm != nullptr, "Where is the VM?");
    vm->cpu.cycle += 1;

    gfu_ubyte** wlut = vm->memory.wlut;
    assert(wlut != nullptr, "Where is the WLUT?");

    gfu_uword page = addr >> 16;
    gfu_ubyte* page_ptr = wlut[page];

    if (page_ptr != nullptr) {
        gfu_uword offset = addr & 0xFFFE;
        gfu_uhalf* addr_ptr = cast(gfu_uhalf*, page_ptr + offset);
        *addr_ptr = value;
        return;
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            gfu_uword offset = (addr & 0xFFFE) & (GFU_EXP1_SCRATCH_SIZE - 1);
            gfu_uhalf* hard_ptr = cast(gfu_uhalf*, hard + offset);
            *hard_ptr = value;
            return;
        }

        gfusx_hardware_write_half(vm, addr, value);
    }
}

void gfusx_memory_write_word(gfusx_t* vm, gfu_uword addr, gfu_uword value) {
    assert(vm != nullptr, "Where is the VM?");
    vm->cpu.cycle += 1;

    gfu_ubyte** wlut = vm->memory.wlut;
    assert(wlut != nullptr, "Where is the WLUT?");

    gfu_uword page = addr >> 16;
    gfu_ubyte* page_ptr = wlut[page];

    if (page_ptr != nullptr) {
        gfu_uword offset = addr & 0xFFFE;
        gfu_uword* addr_ptr = cast(gfu_uword*, page_ptr + offset);
        *addr_ptr = value;
        return;
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            gfu_uword offset = (addr & 0xFFFE) & (GFU_EXP1_SCRATCH_SIZE - 1);
            gfu_uword* hard_ptr = cast(gfu_uword*, hard + offset);
            *hard_ptr = value;
            return;
        }

        gfusx_hardware_write_word(vm, addr, value);
    }
}

gfu_ubyte gfusx_memory_read_hwreg_byte(gfusx_t* vm, gfu_uhalf reg) {
    assert(vm != nullptr, "Where is the VM?");
    assert((reg & ~(GFU_EXP1_HWREG_SIZE - 1)) == 0, "Hardware register out of range.");

    gfu_ubyte* hard = vm->memory.hard;
    assert(hard != nullptr, "Where are the hardware registers?");

    gfu_ubyte* hard_ptr = hard + reg;
    return *hard_ptr;
}

gfu_uhalf gfusx_memory_read_hwreg_half(gfusx_t* vm, gfu_uhalf reg) {
    assert(vm != nullptr, "Where is the VM?");
    assert((reg & ~(GFU_EXP1_HWREG_SIZE - 1)) == 0, "Hardware register out of range.");

    gfu_ubyte* hard = vm->memory.hard;
    assert(hard != nullptr, "Where are the hardware registers?");

    gfu_uhalf* hard_ptr = cast(gfu_uhalf*, hard + (reg & 0xFFFE));
    return SWAP16(*hard_ptr);
}

gfu_uword gfusx_memory_read_hwreg_word(gfusx_t* vm, gfu_uhalf reg) {
    assert(vm != nullptr, "Where is the VM?");
    assert((reg & ~(GFU_EXP1_HWREG_SIZE - 1)) == 0, "Hardware register out of range.");

    gfu_ubyte* hard = vm->memory.hard;
    assert(hard != nullptr, "Where are the hardware registers?");

    gfu_uword* hard_ptr = cast(gfu_uword*, hard + (reg & 0xFFFC));
    return SWAP32(*hard_ptr);
}

void gfusx_memory_write_hwreg_byte(gfusx_t* vm, gfu_uhalf reg, gfu_ubyte value) {
    assert(vm != nullptr, "Where is the VM?");
    assert((reg & ~(GFU_EXP1_HWREG_SIZE - 1)) == 0, "Hardware register out of range.");

    gfu_ubyte* hard = vm->memory.hard;
    assert(hard != nullptr, "Where are the hardware registers?");

    gfu_ubyte* hard_ptr = hard + reg;
    *hard_ptr = value;
}

void gfusx_memory_write_hwreg_half(gfusx_t* vm, gfu_uhalf reg, gfu_uhalf value) {
    assert(vm != nullptr, "Where is the VM?");
    assert((reg & ~(GFU_EXP1_HWREG_SIZE - 1)) == 0, "Hardware register out of range.");

    gfu_ubyte* hard = vm->memory.hard;
    assert(hard != nullptr, "Where are the hardware registers?");

    gfu_uhalf* hard_ptr = cast(gfu_uhalf*, hard + (reg & 0xFFFE));
    *hard_ptr = SWAP16(value);
}

void gfusx_memory_write_hwreg_word(gfusx_t* vm, gfu_uhalf reg, gfu_uword value) {
    assert(vm != nullptr, "Where is the VM?");
    assert((reg & ~(GFU_EXP1_HWREG_SIZE - 1)) == 0, "Hardware register out of range.");

    gfu_ubyte* hard = vm->memory.hard;
    assert(hard != nullptr, "Where are the hardware registers?");

    gfu_uword* hard_ptr = cast(gfu_uword*, hard + (reg & 0xFFFC));
    *hard_ptr = SWAP32(value);
}

gfu_ubyte* gfusx_memory_get_rptr(gfusx_t* vm, gfu_uword addr) {
    assert(vm != nullptr, "Where is the VM?");

    gfu_ubyte** rlut = vm->memory.rlut;
    assert(rlut != nullptr, "Where is the RLUT?");

    gfu_uword page = addr >> 16;
    gfu_ubyte* page_ptr = rlut[page];

    if (page_ptr != nullptr) {
        gfu_uword offset = addr & 0xFFFF;
        return page_ptr + offset;
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            gfu_uword offset = (addr & 0xFFFF) & (GFU_EXP1_SCRATCH_SIZE - 1);
            return hard + offset;
        }

        switch (addr) {
            default: break;
        }
    }

    return nullptr;
}

gfu_ubyte* gfusx_memory_get_wptr(gfusx_t* vm, gfu_uword addr) {
    assert(vm != nullptr, "Where is the VM?");

    gfu_ubyte** wlut = vm->memory.wlut;
    assert(wlut != nullptr, "Where is the WLUT?");

    gfu_uword page = addr >> 16;
    gfu_ubyte* page_ptr = wlut[page];

    if (page_ptr != nullptr) {
        gfu_uword offset = addr & 0xFFFF;
        return page_ptr + offset;
    }

    if (
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG0_BASE) >> 16 ||
        page == (GFU_EXP1_HWREG_BASE + GFU_KSEG1_BASE) >> 16
    ) {
        if ((addr & 0xFFFF) < GFU_EXP1_SCRATCH_SIZE) {
            gfu_ubyte* hard = vm->memory.hard;
            assert(hard != nullptr, "Where are the hardware registers?");
            gfu_uword offset = (addr & 0xFFFF) & (GFU_EXP1_SCRATCH_SIZE - 1);
            return hard + offset;
        }

        switch (addr) {
            default: break;
        }
    }

    return nullptr;
}

gfu_uword gfusx_memory_get_dma_madr(gfusx_t* vm, gfu_uhalf channel) {
    return gfusx_memory_read_hwreg_word(vm, GFU_DMA_MADR(channel) & 0xFFFF);
}

gfu_uword gfusx_memory_get_dma_bcr(gfusx_t* vm, gfu_uhalf channel) {
    return gfusx_memory_read_hwreg_word(vm, GFU_DMA_BCR(channel) & 0xFFFF);
}

gfu_uword gfusx_memory_get_dma_chcr(gfusx_t* vm, gfu_uhalf channel) {
    return gfusx_memory_read_hwreg_word(vm, GFU_DMA_CHCR(channel) & 0xFFFF);
}

void gfusx_memory_set_dma_madr(gfusx_t* vm, gfu_uhalf channel, gfu_uword madr) {
    gfusx_memory_write_hwreg_word(vm, GFU_DMA_MADR(channel) & 0xFFFF, madr & GFU_DMA_MADR_MASK);
}

void gfusx_memory_set_dma_bcr(gfusx_t* vm, gfu_uhalf channel, gfu_uword bcr) {
    gfusx_memory_write_hwreg_word(vm, GFU_DMA_BCR(channel) & 0xFFFF, bcr);
}

void gfusx_memory_set_dma_chcr(gfusx_t* vm, gfu_uhalf channel, gfu_uword chcr) {
    gfusx_memory_write_hwreg_word(vm, GFU_DMA_CHCR(channel) & 0xFFFF, chcr);
}

bool gfusx_memory_is_dma_busy(gfusx_t* vm, gfu_uhalf channel) {
    gfu_uword chcr = gfusx_memory_read_hwreg_word(vm, GFU_DMA_CHCR(channel) & 0xFFFF);
    return 0 != (chcr & 0x01000000);
}

void gfusx_memory_set_dma_busy(gfusx_t* vm, gfu_uhalf channel) {
    gfu_uword chcr = gfusx_memory_read_hwreg_word(vm, GFU_DMA_CHCR(channel) & 0xFFFF);
    gfusx_memory_write_hwreg_word(vm, GFU_DMA_CHCR(channel) & 0xFFFF, chcr | 0x01000000);
}

void gfusx_memory_clear_dma_busy(gfusx_t* vm, gfu_uhalf channel) {
    gfu_uword chcr = gfusx_memory_read_hwreg_word(vm, GFU_DMA_CHCR(channel) & 0xFFFF);
    gfusx_memory_write_hwreg_word(vm, GFU_DMA_CHCR(channel) & 0xFFFF, chcr & ~0x01000000);
}

void gfusx_memory_dma_interrupt(gfusx_t* vm, gfu_uhalf channel) {
    gfu_uword icr = gfusx_memory_read_hwreg_word(vm, GFU_DMA_DICR & 0xFFFF);
    if (0 == (icr & GFU_DMA_DICR_MASTER_ENABLE)) {
        gfusx_logf(vm, "DMA%u cannot trigger: DMA ICR master interrupt not enabled.", channel);
        return;
    }

    // If this channel's interrupt is not enabled, stop
    if (0 == (icr & GFU_DMA_DICR_INT_MASK(channel))) {
        gfusx_logf(vm, "DMA%u cannot trigger: DMA ICR channel interrupt not enabled (0x%08X).", channel, GFU_DMA_DICR_INT_MASK(channel));
        return;
    }

    // Set the interrupt flag if we're continuing on
    icr |= GFU_DMA_DICR_INT_FLAG(channel);

    // Set the master interrupt flag as well, and store the register data back
    gfusx_memory_write_hwreg_word(vm, GFU_DMA_DICR & 0xFFFF, icr | GFU_DMA_DICR_MASTER_INT_FLAG);
    gfusx_logf(vm, "  DMA ICR = %08X", gfusx_memory_read_hwreg_word(vm, GFU_DMA_DICR & 0xFFFF));
    // If the master interrupt flag was not already set, set IRQ DMA
    if (0 == (icr & GFU_DMA_DICR_MASTER_INT_FLAG)) {
        gfusx_logf(vm, "DMA%u setting DMA IRQ.", channel);
        gfusx_set_irq(vm, GFU_IRQ_DMA);
        // gfusx_logf(vm, "  IRQ 0x%08X", gfusx_memory_read_hwreg_word(vm, GFU_ISTAT_ADDR & 0xFFFF));
    } else {
        gfusx_log(vm, "DMA master interrupt flag already set, not setting DMA IRQ.");
    }
}
