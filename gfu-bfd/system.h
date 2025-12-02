#ifndef GAMEFU_BFD_SYSTEM_H_
#define GAMEFU_BFD_SYSTEM_H_

#include "../common/common.h"

#define GFU_FU6109_ICACHE_SIZE 0x1000

/* GameFU Station Memory Map

KSEG0      KSEG1
0000 0000  2000 0000    64 KiB  BIOS/Kernel RAM
0001 0000  2001 0000    16 MiB  User RAM (at max)
1000 0000  3000 0000   256 KiB  BIOS/Kernel ROM
1F00 0000  3F00 0000     8 MiB  Expansion 1
1F80 0000  3F80 0000     4 KiB  Scratch RAM
1F80 1000  3F80 1000     4 KiB  Expansion 1 I/O Ports

*/

#define GFU_KSEG0_BASE 0x00000000u
#define GFU_KSEG1_BASE 0x20000000u
#define GFU_KSEG2_BASE 0x40000000u
#define GFU_KUSEG_BASE 0x80000000u

#define GFU_WRAM_BASE 0x00000000u
#define GFU_WRAM_SIZE 0x01000000u
#define GFU_BIOS_RAM_SIZE 0x00010000u
static_assert(GFU_BIOS_RAM_SIZE == (64 * 1024), "BIOS RAM should be 64 KiB.");
#define GFU_MAIN0_RAM_SIZE 0x003F0000u
static_assert(GFU_BIOS_RAM_SIZE + GFU_MAIN0_RAM_SIZE == (4 * 1024 * 1024), "Main RAM Type-0 (small, default) should be 4 MiB.");
static_assert(GFU_BIOS_RAM_SIZE + GFU_MAIN0_RAM_SIZE <= GFU_WRAM_SIZE, "Main RAM Type-0 (small, default) size exceeds Main RAM size total.");
#define GFU_MAIN1_RAM_SIZE 0x00FF0000u
static_assert(GFU_BIOS_RAM_SIZE + GFU_MAIN1_RAM_SIZE == (16 * 1024 * 1024), "Main RAM Type-1 (large, opt-in) should be 16 MiB.");
static_assert(GFU_BIOS_RAM_SIZE + GFU_MAIN1_RAM_SIZE <= GFU_WRAM_SIZE, "Main RAM Type-1 (large, opt-in) size exceeds Main RAM size total.");
#define GFU_WRAM_STACK_SIZE 0x00010000u

#define GFU_BIOS_ROM_BASE 0x10000000u
#define GFU_BIOS_ROM_SIZE 0x00080000u
static_assert(GFU_BIOS_ROM_SIZE == (512 * 1024), "BIOS ROM should be 512 KiB.");

#define GFU_EXP1_BASE 0x1F000000u
#define GFU_EXP1_SIZE 0x00800000u
static_assert(GFU_EXP1_SIZE == (8 * 1024 * 1024), "Scratchpad should be 8 MiB.");

#define GFU_EXP1_HWREG_BASE (GFU_EXP1_BASE + 0x00800000u)
#define GFU_EXP1_HWREG_SIZE 0x00010000u

#define GFU_EXP1_SCRATCH_BASE GFU_EXP1_HWREG_BASE
#define GFU_EXP1_SCRATCH_SIZE 0x00001000u
static_assert(GFU_EXP1_SCRATCH_SIZE == (4 * 1024), "Scratchpad should be 4 KiB.");
static_assert(GFU_EXP1_SCRATCH_BASE == (GFU_EXP1_BASE + GFU_EXP1_SIZE), "Scratchpad should be exactly after Expansion 1 RAM/ROM.");

#define GFU_EXP1_IOPORT_BASE (GFU_EXP1_SCRATCH_BASE + 0x00001000u)
#define GFU_EXP1_IOPORT_SIZE 0x00001000u
static_assert(GFU_EXP1_IOPORT_SIZE == (4 * 1024), "Expansion 1 I/O ports should be 4 KiB.");
static_assert(GFU_EXP1_IOPORT_BASE == (GFU_EXP1_SCRATCH_BASE + GFU_EXP1_SCRATCH_SIZE), "Expansion 1 I/O ports should be exactly after Scratchpad.");

#define GFU_EXP2_IOPORT_BASE (GFU_EXP1_IOPORT_BASE + 0x00001000u)
#define GFU_EXP2_IOPORT_SIZE 0x00001000u
static_assert(GFU_EXP2_IOPORT_SIZE == (4 * 1024), "Expansion 2 I/O ports should be 4 KiB.");
static_assert(GFU_EXP2_IOPORT_BASE == (GFU_EXP1_IOPORT_BASE + GFU_EXP1_IOPORT_SIZE), "Expansion 2 I/O ports should be exactly after Expansion 1 I/O ports.");

#define GFU_RAM_SIZE_ADDR (GFU_EXP1_IOPORT_BASE + 0x0060)
#define GFU_RAM_SIZE_SMALL 0x3F88
#define GFU_RAM_SIZE_LARGE 0xFF88

enum {
    GFU_IRQ_VBLANK = 1 << 0,
    GFU_IRQ_GPU = 1 << 1,
    GFU_IRQ_DMA = 1 << 3,
    GFU_IRQ_CTRL = 1 << 7,
    GFU_IRQ_SIO = 1 << 8,
    GFU_IRQ_SPU = 1 << 9,
};

#define GFU_ISTAT_ADDR (GFU_EXP1_IOPORT_BASE + 0x0070)
#define GFU_IMASK_ADDR (GFU_EXP1_IOPORT_BASE + 0x0074)

/* Memory Base Address R/W */
#define GFU_DMA_MADR(Channel) (GFU_EXP1_IOPORT_BASE + 0x0080 + ((Channel) * 0x10))
/* 24-bit addresses in RAM are used for DMA */
#define GFU_DMA_MADR_MASK 0x00FFFFFF
/* Block Control R/W */
#define GFU_DMA_BCR(Channel) (GFU_EXP1_IOPORT_BASE + 0x0084 + ((Channel) * 0x10))
/* 16-bit word-count, where 0x0000 == 0x10000 */
#define GFU_DMA_BCR_SYNC0_NWORDS(Reg) 1 + (gfu_uword)(((gfu_uhalf)Reg) - 1)
/* 16-bit block size in words */
#define GFU_DMA_BCR_SYNC1_BLOCK_SIZE(Reg) ((gfu_uhalf)(Reg))
/* 16-bit block count */
#define GFU_DMA_BCR_SYNC1_BLOCK_COUNT(Reg) ((gfu_uhalf)((Reg) >> 16))
/* Channel Control R/W */
#define GFU_DMA_CHCR(Channel) (GFU_EXP1_IOPORT_BASE + 0x0088 + ((Channel) * 0x10))
/* Transfer direction (0 = device to RAM, 1 = RAM to device) */
#define GFU_DMA_CHCR_TDIR(Reg) ((Reg) & 0x01)
/* MADR increment per step (0 = +4, 1 = -4) */
#define GFU_DMA_CHCR_STEP(Reg) (((Reg) >> 1) & 0x01)
/* DMA Control Register R/W */
#define GFU_DMA_DPCR (GFU_EXP1_IOPORT_BASE + 0x00F0)
#define GFU_DMA_DPCR_ENABLED(Reg, Channel) (0 != ((Reg) & (0x08 << (4 * (Channel)))))
/* DMA Interrupt Register R/W */
#define GFU_DMA_DICR (GFU_EXP1_IOPORT_BASE + 0x00F4)
#define GFU_DMA_DICR_INT_MASK(Channel) (1 << (16 + Channel))
#define GFU_DMA_DICR_BUS_ERROR 0x00008000
#define GFU_DMA_DICR_MASTER_ENABLE 0x00800000
#define GFU_DMA_DICR_INT_FLAG(Channel) (1 << (24 + Channel))
#define GFU_DMA_DICR_MASTER_INT_FLAG 0x80000000

/* Boot Post status register R/W */
#define GFU_POST_ADDR (GFU_EXP2_IOPORT_BASE + 0x0041)

#define GFU_CHIP0_ADDR (GFU_EXP1_IOPORT_BASE + 0x0800)
#define GFU_CHIP1_ADDR (GFU_EXP1_IOPORT_BASE + 0x0804)

#endif /* GAMEFU_BFD_SYSTEM_H_ */
