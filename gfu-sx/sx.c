#include <gamefu/common.h>

#include "../gfu-bfd/object.h"

#include "../gfu-opcodes/instruction.h"
#include "../gfu-opcodes/register.h"
#include "../gfu-opcodes/syscall.h"

#include "prologue.h"
#include "chip.h"
#include "memory.h"
#include "platform.h"
#include "vm.h"

typedef struct gfusx_options {
    const char* program_name;
    const char* rom_path;
    bool verbose : 1;
    bool trace : 1;
    bool headless : 1;
} gfusx_options_t;

static bool gfusx_parse_options(int argc, char** argv, gfusx_options_t* options);

int main(int argc, char** argv) {
    int result = 0;

    gfusx_options_t options = {0};
    FILE* rom_file = nullptr;
    gfusx_t vm = {0};

    if (!gfusx_parse_options(argc, argv, &options)) return 1;

    vm.verbose = options.verbose;
    vm.trace = options.trace;
    vm.headless = options.headless;

    if (!gfusx_vm_init(&vm)) return_defer(1);

    gfu_ubyte* ram_ptr = gfusx_memory_get_wptr(&vm, GFU_KSEG1_BASE + GFU_WRAM_BASE + GFU_BIOS_RAM_SIZE);
    if (options.rom_path != nullptr) {
        if (!gfusx_chip_insert_rom_file(&vm, options.rom_path)) {
            return_defer(1);
        }
    } else {
        gfu_inst inst = {0};

        inst.reg.function = GFU_OPFN_OR;
        inst.reg.d = GFU_GPR_R0;
        inst.reg.l = inst.reg.r = 0;
        *(cast(gfu_uword*, ram_ptr)) = inst.raw;
        ram_ptr += 4;

        inst.reg.function = GFU_OPFN_SYSCALL;
        *(cast(gfu_uword*, ram_ptr)) = inst.raw;
        ram_ptr += 4;
    }

    gfusx_platform_main_loop(&vm);

defer:;
    if (rom_file != nullptr) fclose(rom_file);
    gfusx_vm_deinit(&vm);
    return result;
}

#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

static bool gfusx_parse_options(int argc, char** argv, gfusx_options_t* options) {
    options->program_name = SHIFT;

    while (argc > 0) {
        const char* opt = SHIFT;
        if (0 == strcmp(opt, "--verbose")) {
            options->verbose = true;
        } else if (0 == strcmp(opt, "--trace")) {
            options->trace = true;
        } else if (0 == strcmp(opt, "--headless")) {
            options->headless = true;
        } else {
            if (options->rom_path != nullptr) {
                diag_issue(DIAG_ERROR, NOSOURCE, "Can only run a single ROM.");
                return false;
            }

            options->rom_path = opt;
        }
    }

    if (options->rom_path == nullptr) {
        diag_issue(DIAG_WARNING, NOSOURCE, "No ROM file provided. The BIOS will temporarily halt rather than running a ROM or a shell.");
    }

    diag_flush();
    return true;
}

#undef SHIFT

// #include "../gfu-common/diagnostic.c"
// #include "../gfu-common/source.c"
// #include "../gfu-common/arena.c"

// #include "chip.c"
// #include "fu6109.c"
// #include "hardware.c"
// #include "memory.c"
// #include "vm.c"

#if defined(GFUSX_PLATFORM_GLFW)
#  include "platform/glfw3.c"
#else
#  include "null.c"
#endif

#if defined(_WIN32)
#  include "platform/win32.c"
#elif defined(__linux__)
#  include "platform/linux.c"
#endif
