/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

/* -- Address Space ------------------------------------------------------------

The FU6109 architecture uses unsigned 32-bit integer addresses.
This address space is not uniform; it's divided into four segments which are
treated differently by the hardware.
The following diagram shows the segment ranges graphically:

  |-----------|
  | FFFF FFFF |  KUSEG
  |           |  2 GiB user process virtual space
  |           |  mapped and cached
  |           |
  |           |
  |           |
  |           |
  |           |
  |           |
  |           |
  |           |
  | 8000 0000 |
  |-----------|
  | 7FFF FFFF |  KSEG2
  |           |  1 GiB kernel virtual space
  |           |  mapped and cached
  |           |
  |           |
  | 4000 0000 |
  |-----------|
  | 3FFF FFFF |  KSEG1
  |           |  512 MiB physical memory window
  | 2000 0000 |  unmapped and uncached
  |-----------|
  | 1FFF FFFF |  KSEG0
  |           |  512 MiB physical memory window
  | 0000 0000 |  unmapped and cached
  |-----------|

These sections differ in whether or not access to an address is mapped and/or
cached and whether or not access to an address is allowed in user mode.

----- Memory Map ---------------------------------------------------------------

In Console mode, the following table describes the layout of KSEG0 and KSEG1:

KSEG0                  KSEG1
0000 0000 - 0000 FFFF  2000 0000 - 2000 FFFF    64 KiB  BIOS/Kernel RAM
0001 0000 - 0100 FFFF  2001 0000 - 2100 FFFF    16 MiB  User RAM (at max)
1000 0000 - 1007 FFFF  3000 0000 - 3007 FFFF   512 KiB  BIOS/Kernel ROM
1F00 0000 - 1F7F FFFF  3F00 0000 - 3F7F FFFF     8 MiB  Expansion 1
1F80 0000 - 1F80 0FFF  3F80 0000 - 3F80 0FFF     4 KiB  Scratch RAM
1F80 1000 - 1F80 1FFF  3F80 1000 - 3F80 1FFF     4 KiB  Expansion 1 I/O Ports

----------------------------------------------------------------------------- */

#ifndef __GAMEFU_BFD__MEMORY_LAYOUT_H__
#define __GAMEFU_BFD__MEMORY_LAYOUT_H__


#include <gamefu/common.h>


__GAMEFU_C_HEADER_PROLOGUE__


/*
The number of instructions in the FU6109 instruction cache.

During execution, the CPU fetches blocks of instructions from memory.
If the program counter points to an address in the instruction cache, no
additional memory fetch needs to occur.
When the program counter does not find a cached instruction, a new block of
instructions are fetched surroudning the instruction pointer and stored in
the instruction cache for later.

This is the number of instructions in the cache, not the total size of the
cache in bytes.
*/
#define GFU_FU6109_ICACHE_SIZE  0x1000


/*
The base address of the KSEG0 memory segment.
*/
#define GFU_KSEG0_BASE  0x00000000u
/*
The base address of the KSEG1 memory segment.
*/
#define GFU_KSEG1_BASE  0x20000000u
/*
The base address of the KSEG2 memory segment.
*/
#define GFU_KSEG2_BASE  0x40000000u
/*
The base address of the KUSEG memory segment.
*/
#define GFU_KUSEG_BASE  0x80000000u


/*
The relative address of WRAM in either KSEG0 or KSEG1.
WRAM represents the total available main RAM available in the segments.
This encompases all BIOS/Kernel RAM as well as non-virtual User RAM when
operating in Console mode.
*/
#define GFU_WRAM_BASE  0x00000000u
/*
The maximum size of WRAM in either KSEG0 or KSEG1.
WRAM may not refer to a region of memory with a single purpose depending on the
operating mode of the GameFU Station, but this is the maximal bounds of main,
non-virtual RAM in either section.
*/
#define GFU_WRAM_SIZE  0x10000000u

/*
Within the WRAM region of KSEG0 or KSEG1, where main RAM begins for the
BIOS/Kernel.
*/
#define GFU_BIOS_RAM_BASE  0x00000000u
/*
Within the WRAM region of KSEG0 or KSEG1, how much RAM is allocated to the
BIOS/Kernel.
*/
#define GFU_BIOS_RAM_SIZE  0x00010000u

gfu_static_assert(GFU_BIOS_RAM_SIZE == (64 * 1024), \
    "BIOS RAM should be 64 KiB.");

/*
Within the WRAM region of KSEG0 or KSEG1, where main RAM begins for the User.
*/
#define GFU_MAIN_RAM_BASE  (GFU_BIOS_RAM_SIZE)
/*
Within the WRAM region of KSEG0 or KSEG1, how much RAM is allocated to the User
when the "small" RAM flag is enabled.
4 MiB of space is available for the "small" RAM.
*/
#define GFU_MAIN0_RAM_SIZE  0x00400000u
/*
Within the WRAM region of KSEG0 or KSEG1, how much RAM is allocated to the User
when the "large" RAM flag is enabled.
16 MiB of space is available for the "large" RAM.
*/
#define GFU_MAIN1_RAM_SIZE  0x01000000u
/*
Within the WRAM region of KSEG0 or KSEG1, how big the program stack is.
64 KiB of space is allocated for the program stack.
*/
#define GFU_WRAM_STACK_SIZE  0x00010000u
/*
Within the WRAM region of KSEG0 or KSEG1, where the program stack begins.
*/
#define GFU_WRAM_STACK_BASE  \
    ((GFU_WRAM_BASE + GFU_WRAM_SIZE) - GFU_WRAM_STACK_SIZE)

/*
Within KSEG0 or KSEG1, where main BIOS/Kernel ROM storage begins.
*/
#define GFU_BIOS_ROM_BASE  0x10000000u
/*
Within KSEG0 or KSEG1, the size allocated for the BIOS/Kernel ROM.
512 KiB of space is allocated for BIOS/Kernel ROM.
*/
#define GFU_BIOS_ROM_SIZE  0x00080000u

#define GFU_EXP1_BASE 0x1F000000u
#define GFU_EXP1_SIZE 0x00800000u

#define GFU_EXP1_HWREG_BASE (GFU_EXP1_BASE + 0x00800000u)
#define GFU_EXP1_HWREG_SIZE 0x00010000u

#define GFU_EXP1_SCRATCH_BASE GFU_EXP1_HWREG_BASE
#define GFU_EXP1_SCRATCH_SIZE 0x00001000u

#define GFU_EXP1_IOPORT_BASE (GFU_EXP1_SCRATCH_BASE + 0x00001000u)
#define GFU_EXP1_IOPORT_SIZE 0x00001000u

#define GFU_EXP2_IOPORT_BASE (GFU_EXP1_IOPORT_BASE + 0x00001000u)
#define GFU_EXP2_IOPORT_SIZE 0x00001000u

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


gfu_static_assert(GFU_MAIN0_RAM_SIZE == (4 * 1024 * 1024), \
    "Main RAM Type-0 (small, default) should be 4 MiB.");
gfu_static_assert(GFU_BIOS_RAM_SIZE + GFU_MAIN0_RAM_SIZE <= GFU_WRAM_SIZE, \
    "Main RAM Type-0 (small, default) size exceeds Main RAM size total.");

gfu_static_assert(GFU_MAIN1_RAM_SIZE == (16 * 1024 * 1024), \
    "Main RAM Type-1 (large, opt-in) should be 16 MiB.");
gfu_static_assert(GFU_BIOS_RAM_SIZE + GFU_MAIN1_RAM_SIZE <= GFU_WRAM_SIZE, \
    "Main RAM Type-1 (large, opt-in) size exceeds Main RAM size total.");

gfu_static_assert(GFU_BIOS_ROM_SIZE == (512 * 1024), \
    "BIOS ROM should be 512 KiB.");

gfu_static_assert(GFU_EXP1_SIZE == (8 * 1024 * 1024), \
    "Scratchpad should be 8 MiB.");

gfu_static_assert(GFU_EXP1_SCRATCH_SIZE == (4 * 1024), \
    "Scratchpad should be 4 KiB.");
gfu_static_assert(GFU_EXP1_SCRATCH_BASE == (GFU_EXP1_BASE + GFU_EXP1_SIZE), \
    "Scratchpad should be exactly after Expansion 1 RAM/ROM.");

gfu_static_assert(GFU_EXP1_IOPORT_SIZE == (4 * 1024), \
    "Expansion 1 I/O ports should be 4 KiB.");
gfu_static_assert(GFU_EXP1_IOPORT_BASE == \
    (GFU_EXP1_SCRATCH_BASE + GFU_EXP1_SCRATCH_SIZE), \
    "Expansion 1 I/O ports should be exactly after Scratchpad.");

gfu_static_assert(GFU_EXP2_IOPORT_SIZE == (4 * 1024), \
    "Expansion 2 I/O ports should be 4 KiB.");
gfu_static_assert(GFU_EXP2_IOPORT_BASE == \
    (GFU_EXP1_IOPORT_BASE + GFU_EXP1_IOPORT_SIZE), \
    "Expansion 2 I/O ports should be exactly after Expansion 1 I/O ports.");


__GAMEFU_C_HEADER_EPILOGUE__


#endif /* __GAMEFU_BFD__MEMORY_LAYOUT_H__ */
