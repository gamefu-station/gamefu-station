#ifndef GAMEFU_GFUSX_H_
#define GAMEFU_GFUSX_H_

#include "gamefu.h"
#include "diagnostic.h"
#include "asm.h"

typedef union gfusx_gpr {
    gfu_uword raw[34];
    struct gpr_regs {
        gfu_uword zero, at, r0, r1, a0, a1, a2, a3;
        gfu_uword t0, t1, t2, t3, t4, t5, t6, t7;
        gfu_uword s0, s1, s2, s3, s4, s5, s6, s7;
        gfu_uword t8, t9, k0, k1, gp, sp, fp, ra;
        gfu_uword hi, lo;
    } regs;
} gfusx_gpr, gfusx_gpr_t;

static_assert(sizeof(gfusx_gpr_t) == 34 * sizeof(gfu_uword), "");

typedef struct gfusx {
    gfu_ubyte** mem_rlut;
    gfu_ubyte** mem_wlut;
    gfu_ubyte* main_ram;
    gfu_ubyte* bios_rom;
    gfu_ubyte* hwreg;
    gfusx_gpr_t gpr;
    gfu_uword pc;
    bool running : 1;
    bool branched : 1;
    bool trace : 1;
} gfusx, gfusx_t;

gfusx_t* gfusx_create(void);
void gfusx_destroy(gfusx_t* vm);

void gfusx_interpret(gfusx_t* vm);

#define gfusx_mem(Vm, Addr) ((Vm)->mem_rlut[(Addr) >> 16] == nullptr ? nullptr : (Vm)->mem_rlut[(Addr) >> 16] + ((Addr) & 0xFFFF))

gfu_ubyte gfusx_mem_read_byte(gfusx_t* vm, gfu_uword addr);
gfu_uhalf gfusx_mem_read_half(gfusx_t* vm, gfu_uword addr);
gfu_uword gfusx_mem_read_word(gfusx_t* vm, gfu_uword addr);

void gfusx_mem_write_byte(gfusx_t* vm, gfu_uword addr, gfu_ubyte v);
void gfusx_mem_write_half(gfusx_t* vm, gfu_uword addr, gfu_uhalf v);
void gfusx_mem_write_word(gfusx_t* vm, gfu_uword addr, gfu_uword v);

#endif /* GAMEFU_GFUSX_H_ */
