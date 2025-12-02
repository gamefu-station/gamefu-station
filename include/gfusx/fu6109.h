#ifndef GFUSX_FU6109_H_
#define GFUSX_FU6109_H_

#include "../common/common.h"
#include <gamefu/system.h>

#include <gfusx/memory.h>

// #define GFUSX_FU6109_CLOCK_SPEED 33868800
#define GFUSX_FU6109_CLOCK_SPEED 33868800

#define GFUSX_EXC(X) \
    X(INTERRUPT, 0) \
    X(SYSCALL, 1) \
    X(BREAK, 2) \
    X(ARITHMETIC_OVERFLOW, 3) \
    X(BAD_LOAD, 4) \
    X(BAD_STORE, 5) \
    X(IBUS_ERROR, 6) \
    X(DBUS_ERROR, 7) \
    X(RESERVED_INSTRUCTION, 8) \
    X(COPROCESSOR_UNUSABLE, 9)

typedef enum gfusx_exc {
#define X(Id, Value) GFUSX_EXC_##Id = (Value),
    GFUSX_EXC(X)
#undef X
} gfusx_exc_t;

#define GFUSX_INT(X) \
    X(SIO, 0) \
    X(SIO1, 1) \
    X(CHIP, 4) \
    X(CHIP_READ, 5) \
    X(CHIP_DMA, 6) \
    X(GPU_DMA, 12)

typedef enum gfusx_int {
#define X(Id, Value) GFUSX_INT_##Id = (Value),
    GFUSX_INT(X)
#undef X
    GFUSX_INT_COUNT,
} gfusx_int_t;

typedef union gfusx_fu6109_gpr_registers {
    gfu_uword_t values[34];
    struct gpr_regs {
        gfu_uword_t zero, at, r0, r1, a0, a1, a2, a3;
        gfu_uword_t t0, t1, t2, t3, t4, t5, t6, t7;
        gfu_uword_t s0, s1, s2, s3, s4, s5, s6, s7;
        gfu_uword_t t8, t9, k0, k1, gp, sp, fp, ra;
        gfu_uword_t hi, lo;
    } named;
} gfusx_fu6109_gpr_registers_t;

typedef union gfusx_fu6109_cop0_registers {
    gfu_uword_t values[32];
    struct cop0_regs {
        gfu_uword_t reserved0[8];
        gfu_uword_t bad_vaddr;
        gfu_uword_t reserved1[3];
        gfu_uword_t status, cause, epc;
        gfu_uword_t reserved2[1];
        gfu_uword_t reserved3[8];
        gfu_uword_t reserved4[8];
    } named;
} gfusx_fu6109_cop0_registers_t;

static_assert(sizeof(gfusx_fu6109_gpr_registers_t) == 34 * sizeof(gfu_uword_t), "Ensure that the union of GPR registers always maintains matching size.");
static_assert(sizeof(gfusx_fu6109_cop0_registers_t) == 32 * sizeof(gfu_uword_t), "Ensure that the union of COP0 registers always maintains matching size.");

typedef struct gfusx_fu6109_registers {
    gfusx_fu6109_gpr_registers_t gpr;
    gfusx_fu6109_cop0_registers_t cop0;
    gfu_uword_t pc;
} gfusx_fu6109_registers_t;

typedef struct gfusx_fu6109 {
    gfusx_fu6109_registers_t regs;
    gfu_ulong_t inst;
    gfu_ulong_t cycle;
    gfusx_memory_t* memory;
    gfu_uword_t interrupt;
    // 32 bit padding here
    gfu_ulong_t lowest_target;
    gfu_ulong_t interrupt_targets[GFUSX_INT_COUNT];
    gfu_uword_t icache_addr[GFU_FU6109_ICACHE_SIZE];
    gfu_uword_t icache_code[GFU_FU6109_ICACHE_SIZE];
} gfusx_fu6109_t;

void gfusx_exec_block(gfusx_t* vm);

#endif /* GFUSX_FU6109_H_ */
