#include "prologue.h"
#include "memory.h"
#include "fu6109.h"
#include "vm.h"

bool gfusx_vm_init(gfusx_t* vm) {
    if (!gfusx_memory_init(vm)) return false;
    gfusx_memory_reset(vm);

    vm->cpu.regs.pc = GFU_KSEG1_BASE + GFU_BIOS_ROM_BASE;

    if (!gfusx_chip_init(vm)) return false;
    if (!gfusx_platform_init(vm)) return false;

    return true;
}

void gfusx_vm_deinit(gfusx_t* vm) {
    gfusx_platform_deinit(vm);
    gfusx_chip_deinit(vm);
    gfusx_memory_deinit(vm);
}

void gfusx_schedule_interrupt(gfusx_t* vm, gfu_uword_t interrupt, gfu_uword_t cycle_count) {
    gfusx_logf(vm, "Scheduling interrupt %08X in %u cycles.", interrupt, cycle_count);
    const gfu_ulong_t cycle = vm->cpu.cycle;
    gfu_ulong_t target_cycle = cycle + cycle_count;
    vm->cpu.interrupt |= 1 << interrupt;
    vm->cpu.interrupt_targets[interrupt] = target_cycle;
    if (target_cycle < vm->cpu.lowest_target) {
        vm->cpu.lowest_target = target_cycle;
    }
}
