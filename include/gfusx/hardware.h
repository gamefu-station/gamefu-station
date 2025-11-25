#ifndef GFUSX_HARDWARE_H_
#define GFUSX_HARDWARE_H_

#include <gfusx/prologue.h>

gfu_ubyte_t gfusx_hardware_read_byte(gfusx_t* vm, gfu_uword_t addr);
gfu_uhalf_t gfusx_hardware_read_half(gfusx_t* vm, gfu_uword_t addr);
gfu_uword_t gfusx_hardware_read_word(gfusx_t* vm, gfu_uword_t addr);
void gfusx_hardware_write_byte(gfusx_t* vm, gfu_uword_t addr, gfu_ubyte_t value);
void gfusx_hardware_write_half(gfusx_t* vm, gfu_uword_t addr, gfu_uhalf_t value);
void gfusx_hardware_write_word(gfusx_t* vm, gfu_uword_t addr, gfu_uword_t value);

void gfusx_set_irq(gfusx_t* vm, gfu_uword_t irq);
void gfusx_clear_irq(gfusx_t* vm, gfu_uword_t irq);

void gfusx_dma_exec(gfusx_t* vm, gfu_uhalf_t channel, gfu_uword_t chcr);

#endif /* GFUSX_HARDWARE_H_ */
