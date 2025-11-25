#include <gamefu/prologue.h>

#include <fucc/context.h>
#include <fucc/driver.h>
#include <fucc/parser.h>

#include <errno.h>
#include <stdio.h>

static int fucc_compiler_parse_options(fucc_context_t* c, int argc, char** argv);

int fucc_compiler_entry(int argc, char** argv) {
    int result = 0;

    fucc_context_t c = {0};
    fucc_source_t source = {0};
    FILE* outfile = nullptr;

    fucc_context_init(&c, argc);

    int opt_result = fucc_compiler_parse_options(&c, argc, argv);
    if (opt_result > 0) {
        gfu_return_defer(1);
    } else if (opt_result < 0) {
        // we return negative to indicate help/version info was requested and to not continue.
        gfu_return_defer(0);
    }

    const char* output_name = c.output_path;
    if (output_name == nullptr) {
        const char* base_source_name = c.inputs[0].path;
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

        char* new_output_name = gfu_arena_alloc(&c.string_arena, (size_t)(last_dot - source_name) + 5);
        sprintf(new_output_name, "%.*s.fus", (int)(last_dot - source_name), source_name);
        output_name = new_output_name;
    }

    if (!fucc_read_source_from_file(c.inputs[0].path, &source)) {
        gfu_return_defer(1);
    }

    errno = 0;
    outfile = fopen(output_name, "w");
    if (outfile == nullptr) {
        fprintf(stderr, "Error: Failed to open output file for writing: %s.", strerror(errno));
        gfu_return_defer(1);
    }

    fucc_parser_t p = {0};
    fucc_parser_init(&p, &c);
    fucc_lexer_init(&p.lexer, &c, &source);
    fucc_asm_builder_init(&p.asm_builder, &c);
    fucc_asm_builder_begin_file(&p.asm_builder, outfile);
    p.lexer.state = FUCC_LEX_CC;
    fucc_parse_cc_unit(&p);
    fucc_parser_deinit(&p);

defer:;
    fucc_source_free(&source);
    fucc_context_deinit(&c);
    return result;
}


#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

static void fucc_compiler_show_help(const char* program_name) {
    fprintf(
        stderr,
        "%s [options...] <file>\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n"
        "\n"
        "  <file>          The input GameFU C file.\n"
        "  -o <file>, --output <file>\n"
        "                  Output partial-program object to <file>.\n"
        "  -S              Emit assembler instead of an object file.\n",
        program_name
    );
}

static int fucc_compiler_parse_options(fucc_context_t* c, int argc, char** argv) {
    int result = 0;

    c->program_name = SHIFT;
    c->stage = FUCC_ASSEMBLE;

    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("--help", option)) {
            fucc_compiler_show_help(c->program_name);
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
        } else {
            c->inputs[c->input_count++] = (fucc_input_file_t) {
                .kind = FUCC_INPUT_FUC,
                .path = opt,
            };
        }
    }

    if (c->input_count == 0) {
        fucc_compiler_show_help(c->program_name);
        fprintf(stderr, "\nError: No source GameFU C file provided.\n");
        gfu_return_defer(1);
    }

defer:;
    return result;
}

#undef SHIFT
