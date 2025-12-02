#ifndef GFUSX_CHIP_H_
#define GFUSX_CHIP_H_

#include "prologue.h"

typedef union gfusx_chip_status {
    gfu_uword_t value;
    struct {
        gfu_uword_t reserved0 : 24;
        gfu_uword_t irq : 1;
        gfu_uword_t dma_request_state : 1;
        gfu_uword_t cmd_ready : 1;
        gfu_uword_t chip2cpu_ready : 1;
        gfu_uword_t dma_block_ready : 1;
        gfu_uword_t dma_direction : 2;
        gfu_uword_t reserved2 : 1;
    } bits;
} gfusx_chip_status_t;

static_assert(sizeof(gfusx_chip_status_t) == sizeof(gfu_uword_t), "Ensure that CHIP status is always word-sized.");

typedef struct gfusx_chip {
    gfu_ubyte_t* data;
    gfu_uword_t size, offset;
    gfusx_chip_status_t status;
} gfusx_chip_t;

bool gfusx_chip_init(gfusx_t* vm);
void gfusx_chip_deinit(gfusx_t* vm);
bool gfusx_chip_insert_rom_data(gfusx_t* vm, gfu_ubyte_t* data, gfu_uword_t size);
bool gfusx_chip_insert_rom_file(gfusx_t* vm, const char* rom_file_path);

void gfusx_chip_dma(gfusx_t* vm, gfu_uword_t madr, gfu_uword_t bcr, gfu_uword_t chcr);

gfu_uword_t gfusx_chip_read0(gfusx_t* vm);
gfu_uword_t gfusx_chip_read1(gfusx_t* vm);
void gfusx_chip_write0(gfusx_t* vm, gfu_uword_t value);
void gfusx_chip_write1(gfusx_t* vm, gfu_uword_t value);

void gfusx_chip_interrupt(gfusx_t* vm);
void gfusx_chip_read_interrupt(gfusx_t* vm);
void gfusx_chip_dma_interrupt(gfusx_t* vm);

#endif /* GFUSX_CHIP_H_ */
