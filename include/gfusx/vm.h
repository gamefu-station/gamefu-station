#ifndef GFUSX_VM_H_
#define GFUSX_VM_H_

#include <gfusx/prologue.h>
#include <gfusx/chip.h>
#include <gfusx/fu6109.h>
#include <gfusx/memory.h>
#include <gfusx/platform.h>

#define GFUSX_CYCLE_BIAS 2

struct gfusx {
    gfusx_platform_t* platform;
    gfusx_memory_t memory;
    gfusx_fu6109_t cpu;
    gfusx_chip_t chip;
    bool halt : 1;
    bool verbose : 1;
    bool trace : 1;
    bool headless : 1;
};

bool gfusx_vm_init(gfusx_t* vm);
void gfusx_vm_deinit(gfusx_t* vm);

void gfusx_schedule_interrupt(gfusx_t* vm, gfu_uword_t interrupt, gfu_uword_t cycle_count);

#endif /* GFUSX_VM_H_ */
