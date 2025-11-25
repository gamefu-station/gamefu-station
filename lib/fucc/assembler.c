#include <gamefu/prologue.h>

#include <fucc/context.h>
#include <fucc/driver.h>

#include <fucc/source.h>
#include <fucc/lexer2.h>

static int fucc_assembler_parse_options(fucc_context_t* c, int argc, char** argv);

int fucc_assembler_entry(int argc, char** argv) {
    int result = 0;

    fucc_context_t c = {0};
    fucc_source_t source = {0};

    fucc_context_init(&c, argc);

    int opt_result = fucc_assembler_parse_options(&c, argc, argv);
    if (opt_result > 0) {
        gfu_return_defer(1);
    } else if (opt_result < 0) {
        // we return negative to indicate help/version info was requested and to not continue.
        gfu_return_defer(0);
    }

    // fprintf(stderr, "Hello, '%s'!\n", c.program_name);

    if (!fucc_read_source_from_file(c.inputs[0].path, &source)) {
        gfu_return_defer(1);
    }

    fucc_lexer_t l = {0};
    fucc_lexer_init(&l, &c, &source);
    l.state = FUCC_LEX_AS;

    fucc_token_t t = {0};
    while (t = fucc_read_token(&l), t.kind != FUCC_TK_EOF) {
        fucc_dump_token(&c, t);
    }

    fucc_lexer_deinit(&l);

defer:;
    fucc_source_free(&source);
    fucc_context_deinit(&c);
    return result;
}

#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

static void fucc_assembler_show_help(const char* program_name) {
    fprintf(
        stderr,
        "%s [options...] <file>\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n"
        "\n"
        "  <file>          The input GameFU Assembler file.\n"
        "  -o <file>, --output <file>\n"
        "                  Output partial-program object to <file>.\n",
        program_name
    );
}

static int fucc_assembler_parse_options(fucc_context_t* c, int argc, char** argv) {
    int result = 0;

    c->program_name = SHIFT;
    c->stage = FUCC_ASSEMBLE;

    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("--help", option)) {
            fucc_assembler_show_help(c->program_name);
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
        } else {
            c->inputs[c->input_count++] = (fucc_input_file_t) {
                .kind = FUCC_INPUT_FUS,
                .path = opt,
            };
        }
    }

    if (c->input_count == 0) {
        fucc_assembler_show_help(c->program_name);
        fprintf(stderr, "\nError: No source GameFU Assembler file provided.\n");
        gfu_return_defer(1);
    }

defer:;
    return result;
}

#undef SHIFT
