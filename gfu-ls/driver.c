/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/common.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <io.h>
#  define isatty _isatty
#  define fileno _fileno
#elif defined(__linux__)
#  include <unistd.h>
#endif

#include "internal.h"

#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

bool gfuls_options_parse(int argc, char** argv, gfuls_state* state) {
    bool result = false;

    state->program = SHIFT;

    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("--help", option)) {
            state->show_help = true;
            return true;
        } else if (0 == strcmp("--version", option)) {
            state->show_version = true;
            return true;
        }
    }

    while (argc > 0) {
        const char* opt = SHIFT;
        gfu_assertn(opt != nullptr);

        if (0 == strcmp(opt, "--verbose") || 0 == strcmp(opt, "-v")) {
            state->verbose = true;
        } else {
        }
    }

    result = true;
fail:;
    gfu_diag_flush();
    return result;
}

#undef SHIFT

static void gfuls_show_help(const char* program) {
    fprintf(
        stderr,
        "%s [options...]\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n",
        program
    );
}

static void gfuls_show_version(void) {
    fprintf(
        stderr,
        "GameFU Assembler version " VERSION "\n"
        "This tool is part of the GameFU Station fantasy console project toolchain.\n"
    );
}

__GAMEFU_API__ int gfuls_driver_main(int argc, char** argv) {
    int result = 1;
success:;
    result = 0;
fail:;
    gfu_diag_flush();
    return result;
}

__GAMEFU_API__ int gfuls_driver_fuzz(const char* text, size_t length) {
    return 0;
}
