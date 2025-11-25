#include <gamefu/prologue.h>

#include <fucc/context.h>
#include <fucc/driver.h>

static int fucc_driver_parse_options(fucc_context_t* c, int argc, char** argv);

#define FUCC_VERSION "0.0.1.0"

static char* fucc_concat(const char* s0, const char* s1) {
    size_t s0_length = strlen(s0);
    size_t s1_length = strlen(s1);

    char* result = calloc(2 + s0_length + s1_length, sizeof *result);

    memcpy(result, s0, s0_length);
    result[s0_length] = ' ';
    memcpy(result + s0_length + 1, s1, s1_length);
    gfu_assert(result[s0_length + 1 + s1_length] == 0, "Where's the null terminator?");

    return result;
}

int fucc_driver(int argc, char** argv) {
    int result = 0;

    char* child_proc_name = nullptr;
    fucc_context_t c = {0};

    if (argc >= 2) {
        const char* program_name = argv[0];
        if (0 == strcmp(argv[1], "-cc1")) {
            argv[1] = child_proc_name = fucc_concat(program_name, "-cc1");
            gfu_return_defer(fucc_compiler_entry(argc - 1, argv + 1));
        } else if (0 == strcmp(argv[1], "-as1")) {
            argv[1] = child_proc_name = fucc_concat(program_name, "-as1");
            gfu_return_defer(fucc_assembler_entry(argc - 1, argv + 1));
        } else if (0 == strcmp(argv[1], "-ld1")) {
            argv[1] = child_proc_name = fucc_concat(program_name, "-ld1");
            gfu_return_defer(fucc_linker_entry(argc - 1, argv + 1));
        }
    }

    fucc_context_init(&c, argc);

    int opt_result = fucc_driver_parse_options(&c, argc, argv);
    if (opt_result > 0) {
        gfu_return_defer(1);
    } else if (opt_result < 0) {
        // we return negative to indicate help/version info was requested and to not continue.
        gfu_return_defer(0);
    }

    fprintf(stderr, "Hello, '%s'!\n", c.program_name);

defer:;
    if (child_proc_name != nullptr) free(child_proc_name);
    fucc_context_deinit(&c);
    return result;
}

void fucc_show_version(VOIDPROTO) {
    fprintf(
        stderr,
        "GameFU Compiler Collection version " FUCC_VERSION "\n"
        "This tool is part of the GameFU Station fantasy console project toolchain.\n"
    );
}

#undef FUCC_VERSION

#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

static void fucc_driver_show_help(const char* program_name) {
    fprintf(
        stderr,
        "%s [options...] <file...>\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n"
        "\n"
        "  <file...>       The input source, object or library files.\n"
        "                  GameFU C and Assembler source is accepted, as\n"
        "                  are GameFU objects and packaged libraries.\n"
        "  -x <kind>       Override the type of the following input files,\n"
        "                  instead of relying on the file extension.\n"
        "                  One of: default, fuc, asm, obj, lib\n"
        "  -o <file>, --output <file>\n"
        "                  Output object to <file>.\n"
        "                  If compiling with the '-c' option, the ROM\n"
        "                  will be a partial program, not linked.\n"
        "  -S              Emit assembler instead of an object file.\n"
        "                  This option can only be used with a single\n"
        "                  input non-assembler source at a time.\n"
        "  -c              Compile, but don't link, the output.\n"
        "                  This option can only be used with a single\n"
        "                  input source at a time.\n",
        program_name
    );
}

static int fucc_driver_parse_options(fucc_context_t* c, int argc, char** argv) {
    int result = 0;

    c->program_name = SHIFT;
    c->stage = FUCC_LINK;

    fucc_input_kind_t current_kind = FUCC_INPUT_UNKNOWN;
    gfu_uword_t source_count = 0;

    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("--help", option)) {
            fucc_driver_show_help(c->program_name);
            gfu_return_defer(-1);
        } else if (0 == strcmp("--version", option)) {
            fucc_show_version();
            gfu_return_defer(-1);
        }
    }

    while (argc > 0) {
        const char* opt = SHIFT;
        if (0 == strcmp("-v", opt) || 0 == strcmp("--verbose", opt)) {
            c->verbose = true;
        } else if (0 == strcmp("-o", opt) || 0 == strcmp("--output", opt)) {
            c->output_path = SHIFT;
            if (c->output_path == nullptr) {
                fprintf(stderr, "Error: Option '%s' requires an argument '<file>'.\n", opt);
                gfu_return_defer(1);
            }
        } else if (0 == strcmp("-S", opt)) {
            c->stage = FUCC_COMPILE;
        } else if (0 == strcmp("-c", opt)) {
            c->stage = FUCC_ASSEMBLE;
        } else if (0 == strcmp("-x", opt)) {
            const char* kind = SHIFT;
            if (kind == nullptr) {
                fprintf(stderr, "Error: Option '%s' requires an argument '<kind>'.\n", opt);
                gfu_return_defer(1);
            }

            if (0 == strcmp(kind, "default")) {
                current_kind = FUCC_INPUT_UNKNOWN;
            } else if (0 == strcmp(kind, "fuc") || 0 == strcmp(kind, "c")) {
                current_kind = FUCC_INPUT_FUC;
            } else if (0 == strcmp(kind, "fus") || 0 == strcmp(kind, "asm") || 0 == strcmp(kind, "as") || 0 == strcmp(kind, "s")) {
                current_kind = FUCC_INPUT_FUS;
            } else if (0 == strcmp(kind, "fuo") || 0 == strcmp(kind, "obj") || 0 == strcmp(kind, "o")) {
                current_kind = FUCC_INPUT_FUO;
            } else if (0 == strcmp(kind, "ful") || 0 == strcmp(kind, "lib") || 0 == strcmp(kind, "l") || 0 == strcmp(kind, "a")) {
                current_kind = FUCC_INPUT_FUL;
            } else {
                fprintf(stderr, "Error: Unknown argument '%s' to option '%s'.\n", kind, opt);
                gfu_return_defer(1);
            }
        } else {
            fucc_input_kind_t input_kind = current_kind;
            if (input_kind == FUCC_INPUT_UNKNOWN) {
                const char* ext = opt + strlen(opt) - 1;
                while (ext > opt && *ext != '.') {
                    ext--;
                }

                if (0 == strcmp(ext, ".fuc")) {
                    input_kind = FUCC_INPUT_FUC;
                } else if (0 == strcmp(ext, ".fus")) {
                    input_kind = FUCC_INPUT_FUS;
                } else if (0 == strcmp(ext, ".fuo")) {
                    input_kind = FUCC_INPUT_FUO;
                } else if (0 == strcmp(ext, ".ful")) {
                    input_kind = FUCC_INPUT_FUL;
                } else {
                    fprintf(stderr, "Error: Unknown file kind. Use the '-x' option to specify, or change the extension.\n");
                    fprintf(stderr, "Note: '.fuc', '.fus', '.fuo' and '.ful' files are recognized.\n");
                    gfu_return_defer(1);
                }
            }

            if (input_kind == FUCC_INPUT_FUC || input_kind == FUCC_INPUT_FUS) {
                source_count++;
            }

            c->inputs[c->input_count++] = (fucc_input_file_t) {
                .kind = input_kind,
                .path = opt,
            };
        }
    }

    if (c->input_count == 0) {
        fucc_driver_show_help(c->program_name);
        fprintf(stderr, "\nError: No input files provided.\n");
        gfu_return_defer(1);
    }

    if (source_count == 0 && c->stage <= FUCC_ASSEMBLE) {
        fucc_driver_show_help(c->program_name);
        fprintf(stderr, "\nError: No source files provided. Remove '-S' or '-c' options to link.\n");
        gfu_return_defer(1);
    }

defer:;
    return result;
}

#undef SHIFT

