#include <gamefu/prologue.h>

#include <fucc/context.h>
#include <fucc/driver.h>

static int fucc_linker_parse_options(fucc_context_t* c, int argc, char** argv);

int fucc_linker_entry(int argc, char** argv) {
    int result = 0;

    fucc_context_t c = {0};
    fucc_context_init(&c, argc);

    int opt_result = fucc_linker_parse_options(&c, argc, argv);
    if (opt_result > 0) {
        gfu_return_defer(1);
    } else if (opt_result < 0) {
        // we return negative to indicate help/version info was requested and to not continue.
        gfu_return_defer(0);
    }

    fprintf(stderr, "Hello, '%s'!\n", c.program_name);

defer:;
    fucc_context_deinit(&c);
    return result;
}

#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

static void fucc_linker_show_help(const char* program_name) {
    fprintf(
        stderr,
        "%s [options...] <file...>\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n"
        "\n"
        "  <file>          The input GameFU object or library files.\n"
        "  -x <kind>       Override the type of the following input files,\n"
        "                  instead of relying on the file extension.\n"
        "                  One of: default, obj, lib\n"
        "  -o <file>, --output <file>\n"
        "                  Output linked object to <file>.\n",
        program_name
    );
}

static int fucc_linker_parse_options(fucc_context_t* c, int argc, char** argv) {
    int result = 0;

    c->program_name = SHIFT;
    c->stage = FUCC_LINK;

    fucc_input_kind_t current_kind = FUCC_INPUT_UNKNOWN;

    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("--help", option)) {
            fucc_linker_show_help(c->program_name);
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
        } else if (0 == strcmp("-x", opt)) {
            const char* kind = SHIFT;
            if (kind == nullptr) {
                fprintf(stderr, "Error: Option '%s' requires an argument '<kind>'.\n", opt);
                gfu_return_defer(1);
            }

            if (0 == strcmp(kind, "default")) {
                current_kind = FUCC_INPUT_UNKNOWN;
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

                if (0 == strcmp(ext, ".fuo")) {
                    input_kind = FUCC_INPUT_FUO;
                } else if (0 == strcmp(ext, ".ful")) {
                    input_kind = FUCC_INPUT_FUL;
                } else {
                    fprintf(stderr, "Error: Unknown file kind. Use the '-x' option to specify, or change the extension.\n");
                    fprintf(stderr, "Note: '.fuo' and '.ful' files are recognized.\n");
                    gfu_return_defer(1);
                }
            }

            c->inputs[c->input_count++] = (fucc_input_file_t) {
                .kind = input_kind,
                .path = opt,
            };
        }
    }

    if (c->input_count == 0) {
        fucc_linker_show_help(c->program_name);
        fprintf(stderr, "\nError: No object or library files provided.\n");
        gfu_return_defer(1);
    }

defer:;
    return result;
}

#undef SHIFT
