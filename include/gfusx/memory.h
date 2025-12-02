#ifndef GFUSX_MEMORY_H_
#define GFUSX_MEMORY_H_

#include "../common/common.h"

#include <gfusx/prologue.h>

typedef struct gfusx_memory {
    gfu_ubyte_t* wram;
    gfu_ubyte_t* bios;
    gfu_ubyte_t* hard;

    gfu_ubyte_t** rlut;
    gfu_ubyte_t** wlut;
} gfusx_memory_t;

bool gfusx_memory_init(gfusx_t* vm);
void gfusx_memory_reset(gfusx_t* vm);
void gfusx_memory_deinit(gfusx_t* vm);
void gfusx_memory_set_luts(gfusx_t* vm);

gfu_ubyte_t gfusx_memory_read_byte(gfusx_t* vm, gfu_uword_t addr);
gfu_uhalf_t gfusx_memory_read_half(gfusx_t* vm, gfu_uword_t addr);
gfu_uword_t gfusx_memory_read_word(gfusx_t* vm, gfu_uword_t addr);
gfu_uword_t gfusx_memory_read_inst(gfusx_t* vm, gfu_uword_t addr);
void gfusx_memory_write_byte(gfusx_t* vm, gfu_uword_t addr, gfu_ubyte_t value);
void gfusx_memory_write_half(gfusx_t* vm, gfu_uword_t addr, gfu_uhalf_t value);
void gfusx_memory_write_word(gfusx_t* vm, gfu_uword_t addr, gfu_uword_t value);

gfu_ubyte_t gfusx_memory_read_hwreg_byte(gfusx_t* vm, gfu_uhalf_t reg);
gfu_uhalf_t gfusx_memory_read_hwreg_half(gfusx_t* vm, gfu_uhalf_t reg);
gfu_uword_t gfusx_memory_read_hwreg_word(gfusx_t* vm, gfu_uhalf_t reg);
void gfusx_memory_write_hwreg_byte(gfusx_t* vm, gfu_uhalf_t reg, gfu_ubyte_t value);
void gfusx_memory_write_hwreg_half(gfusx_t* vm, gfu_uhalf_t reg, gfu_uhalf_t value);
void gfusx_memory_write_hwreg_word(gfusx_t* vm, gfu_uhalf_t reg, gfu_uword_t value);

gfu_ubyte_t* gfusx_memory_get_rptr(gfusx_t* vm, gfu_uword_t addr);
gfu_ubyte_t* gfusx_memory_get_wptr(gfusx_t* vm, gfu_uword_t addr);

gfu_uword_t gfusx_memory_get_dma_madr(gfusx_t* vm, gfu_uhalf_t channel);
gfu_uword_t gfusx_memory_get_dma_bcr(gfusx_t* vm, gfu_uhalf_t channel);
gfu_uword_t gfusx_memory_get_dma_chcr(gfusx_t* vm, gfu_uhalf_t channel);
void gfusx_memory_set_dma_madr(gfusx_t* vm, gfu_uhalf_t channel, gfu_uword_t madr);
void gfusx_memory_set_dma_bcr(gfusx_t* vm, gfu_uhalf_t channel, gfu_uword_t bcr);
void gfusx_memory_set_dma_chcr(gfusx_t* vm, gfu_uhalf_t channel, gfu_uword_t chcr);

bool gfusx_memory_is_dma_busy(gfusx_t* vm, gfu_uhalf_t channel);
void gfusx_memory_set_dma_busy(gfusx_t* vm, gfu_uhalf_t channel);
void gfusx_memory_clear_dma_busy(gfusx_t* vm, gfu_uhalf_t channel);
void gfusx_memory_dma_interrupt(gfusx_t* vm, gfu_uhalf_t channel);

#endif /* GFUSX_MEMORY_H_ */
