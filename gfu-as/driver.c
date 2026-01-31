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

bool gfuas_options_parse(int argc, char** argv, gfuas_state* state) {
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
        assertn(opt != nullptr);

        if (0 == strcmp(opt, "-as1")) {
            continue;
        } else if (0 == strcmp(opt, "--verbose") || 0 == strcmp(opt, "-v")) {
            state->verbose = true;
        } else if (0 == strcmp(opt, "-o")) {
            const char* path = SHIFT;
            if (path == nullptr) {
                gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Option '-o' requires an argument.");
                goto fail;
            }
        } else {
            if (state->source.name != nullptr) {
                gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Only one source file may be assembled at a time.");
                goto fail;
            }
            if (!gfu_source_load_from_file(opt, &state->source)) {
                goto fail;
            }
        }
    }

    result = true;
fail:;
    gfu_diag_flush();
    return result;
}

#undef SHIFT

static void gfuas_show_help(const char* program) {
    fprintf(
        stderr,
        "%s [options...] <file>\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n"
        "\n"
        "  <file>          The single input GameFU ASM source file.\n"
        "  -o <file>, --output <file>\n"
        "                  Output ROM to <file>.\n",
        program
    );
}

static void gfuas_show_version(void) {
    fprintf(
        stderr,
        "GameFU Assembler version " VERSION "\n"
        "This tool is part of the GameFU Station fantasy console project toolchain.\n"
    );
}

__GAMEFU_API__ int gfuas_driver_main(int argc, char** argv) {
    int result = 1;
    FILE* f = nullptr;

    gfu_diag_color_output(isatty(fileno(stderr)));

    gfuas_state state = {
        .entry_name = "_start",
    };

    if (!gfuas_options_parse(argc, argv, &state)) {
        goto fail;
    }

    if (state.show_help) {
        gfuas_show_help(state.program);
        goto success;
    }

    if (state.show_version) {
        gfuas_show_version();
        goto success;
    }

    if (state.source.name == nullptr) {
        gfuas_show_help(state.program);
        fprintf(stderr, "\n");
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "No source file provided.");
        goto fail;
    }

    gfu_arena_init(&state.arena, 32 * 1024);

    const char* output_name = state.output;
    if (output_name == nullptr) {
        const char* base_source_name = state.source.name;
        const char* source_name = base_source_name + strlen(base_source_name);
        while (
            source_name > base_source_name &&
            *(source_name - 1) != '/' &&
            *(source_name - 1) != '\\'
        ) {
            source_name--;
        }

        const char* last_dot = strrchr(source_name, '.');
        if (last_dot == nullptr) {
            last_dot = source_name + strlen(source_name);
        }

        char* new_output_name = gfu_arena_alloc(&state.arena, (last_dot - source_name) + 5);
        sprintf(new_output_name, "%.*s.gfu", (int)(last_dot - source_name), source_name);
        output_name = new_output_name;
    }

    gfuas_print_verbose(&state, "Generating '%s'", output_name);

    gfuobj_raw* rom_data = gfuas_assemble_internal(&state);
    if (rom_data == nullptr) goto fail;

    errno = 0;
    f = fopen(output_name, "wb");
    if (f == nullptr) {
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Failed to open output file '%s': %s", output_name, strerror(errno));
        goto fail;
    }

    errno = 0;
    fwrite(rom_data, (size_t) rom_data->header.rom_size, 1, f);
    free(rom_data);
    if (ferror(f)) {
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Failed to write to output file '%s': %s", output_name, strerror(errno));
        goto fail;
    }

    // if we make it here, we succeeded; set the exit code appropriately.
success:;
    result = 0;
fail:;
    gfu_diag_flush();
    if (f != nullptr) fclose(f);
    free((void*) state.source.text);
    gfu_arena_deinit(&state.arena);
    return result;
}

__GAMEFU_API__ int gfuas_driver_fuzz(const char* text, size_t length) {
    return 0;
}
