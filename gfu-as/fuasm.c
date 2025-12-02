#include "../common/common.h"

#include <gamefu/isel.h>
#include <gamefu/system.h>
#include <gamefu/object.h>

#if defined(__linux__)
#  include <unistd.h>
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define ECHOETOK_IMPL
#include <echoetok.h>

#include "asm.h"

#define VERSION "0.0.1.0"

typedef struct gfuas_options {
    const char* program;
    bool verbose;
    source* sources;
    gfu_uword source_count;
    const char* output;
} gfuas_options;

typedef struct gfuas_state {
    gfuas_options options;

    arena string_arena;
    arena stmt_arena;

    gfuas_stmt* ir;
    struct address {
        const char* name;
        gfu_uword address;
        gfu_uword parent;
    }* addresses;
    gfu_uword label_count;

    gfu_uword address_space;
    const char* entry_name;
    bool is_entry_set;
} gfuas_state;

#define FUASM_TOKEN_KINDS(X) \
    X(ENDL) \
    X(DIRECTIVE) \
    X(LABEL_GLOBAL) \
    X(LABEL_LOCAL) \
    X(MNEMONIC) \
    X(REGISTER) \
    X(IMMEDIATE) \
    X(BYTE_STRING)

typedef enum gfuas_token_kind {
    FUASM_TK_INVALID = 0x00FFFFFF,
    FUASM_TK_EOF = 0,

    FUASM_TK_PRINTABLE_BEGIN = ETOK_PRINTABLE_BEGIN,
    FUASM_TK_PRINTABLE_END = ETOK_PRINTABLE_END,

    FUASM_TK_INVALID_CHARACTER = ETOK_INVALID_CHARACTER,

    _gfuas_tk_multibyte_offset = ETOK_MULTIBYTE_BEGIN,

#define X(Id) FUASM_TK_##Id,
    FUASM_TOKEN_KINDS(X)
#undef X
} gfuas_token_kind;

typedef struct gfuas_token {
    source source;
    gfuas_token_kind kind;
    gfu_uword location;
    const char *begin, *end;
    gfu_uword line, column;
    union {
        gfuas_directive directive;
        gfuas_mnemonic mnemonic;
        gfu_gpr _register;
        gfu_uword immediate;
        const char* label;
        struct {
            const char* data;
            gfu_uword length;
        } byte_string;
    } as;
} gfuas_token;

typedef struct gfuas_userdata {
    gfuas_state* state;
    source source;
} gfuas_userdata;

typedef struct gfuas_parser {
    gfuas_state* state;
    source source;
    etok_lexer lexer;
    gfuas_token tk, next;
} gfuas_parser;

#define PDATA ((gfuas_userdata) {parser->state, parser->source})

typedef enum gfuas_section {
    FUASM_TEXT,
    FUASM_DATA,
} gfuas_section;

static void show_help(void);
static void show_version(void);
static bool gfuas_options_parse(int argc, char** argv, gfuas_options* options);
static void print_verbose(gfuas_state* state, const char* format, ...);

static char* gfuas_assemble_ir_internal(gfuas_state* state, gfu_uword* rom_size);
static char* gfuas_assemble_internal(gfuas_state* state, gfu_uword* rom_size);

static void gfuas_lexer_init(gfuas_userdata* userdata, etok_lexer* lexer);
static gfuas_token gfuas_lexer_read(etok_lexer* lexer);
static void gfuas_token_dump(gfuas_token token);

int gfuas_driver_main(int argc, char** argv) {
    int result = 0;

    FILE* f = nullptr;

    diag_color_output(isatty(fileno(stderr)));

    gfuas_state state = {
        .address_space = GFUOBJ_ADDRSPACE_USER,
        .entry_name = "_start",
    };

    if (!gfuas_options_parse(argc, argv, &state.options)) {
        return_defer(1);
    }

    arena_init(&state.string_arena, 32 * 1024 * 1024);
    arena_init(&state.stmt_arena, 32 * 1024 * 1024);

    const char* output_name = state.options.output;
    if (output_name == nullptr) {
        const char* base_source_name = state.options.sources[0].name;
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

        char* new_output_name = arena_alloc(&state.string_arena, (size_t)(last_dot - source_name) + 5);
        sprintf(new_output_name, "%.*s.gfu", (int)(last_dot - source_name), source_name);
        output_name = new_output_name;
    }

    print_verbose(&state, "Generating '%s'", output_name);

    gfu_uword rom_size;
    char* rom_data = gfuas_assemble_internal(&state, &rom_size);
    if (rom_data == nullptr) return_defer(1);

    errno = 0;
    f = fopen(output_name, "wb");
    if (f == nullptr) {
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to open output file '%s': %s", output_name, strerror(errno));
        return_defer(1);
    }

    errno = 0;
    fwrite(rom_data, (size_t)rom_size, 1, f);
    free(rom_data);
    if (ferror(f)) {
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to write to output file '%s': %s", output_name, strerror(errno));
        return_defer(1);
    }

defer:;
    if (f != nullptr) fclose(f);
    free(state.addresses);

    for (gfu_uword i = 0; i < state.options.source_count; i++) {
        free((char*)state.options.sources[i].text);
    }

    free(state.options.sources);

    arena_deinit(&state.stmt_arena);
    arena_deinit(&state.string_arena);

    return result;
}

int gfuas_driver_fuzz(const char* text, size_t length) {
    int result = 0;

    gfuas_state state = {
        .address_space = GFUOBJ_ADDRSPACE_USER,
        .entry_name = "_start",
    };

    state.options.source_count = 1;
    state.options.sources = calloc(1, sizeof *state.options.sources);
    state.options.sources[0] = (struct source) {
        .name = "<fuzz-input>",
        .text = text,
        .length = (gfu_word)length,
    };

    arena_init(&state.string_arena, 32 * 1024 * 1024);
    arena_init(&state.stmt_arena, 32 * 1024 * 1024);

    gfu_uword rom_size;
    char* rom_data = gfuas_assemble_internal(&state, &rom_size);

defer:;
    free(rom_data);
    free(state.addresses);
    free(state.options.sources);
    arena_deinit(&state.stmt_arena);
    arena_deinit(&state.string_arena);
    return result;
}

char* gfuas_assemble_ir(gfuas_stmt* ir, gfu_uword* rom_size) {
    gfuas_state state = {
        .ir = ir,
    };

    state.options.source_count = 1;
    state.options.sources = calloc(1, sizeof *state.options.sources);
    state.options.sources[0] = (struct source) {
        .name = "<noinput>",
        .text = "\n",
        .length = 1,
    };

    arena_init(&state.string_arena, 32 * 1024 * 1024);
    arena_init(&state.stmt_arena, 32 * 1024 * 1024);

    char* rom_data = gfuas_assemble_ir_internal(&state, rom_size);

    free(state.options.sources);
    arena_deinit(&state.stmt_arena);
    arena_deinit(&state.string_arena);
    return rom_data;
}

char* gfuas_assemble(source source, gfu_uword* rom_size) {
    gfuas_state state = {0};

    state.options.source_count = 1;
    state.options.sources = calloc(1, sizeof *state.options.sources);
    state.options.sources[0] = source;

    arena_init(&state.string_arena, 32 * 1024 * 1024);
    arena_init(&state.stmt_arena, 32 * 1024 * 1024);

    char* rom_data = gfuas_assemble_internal(&state, rom_size);

    free(state.options.sources);
    arena_deinit(&state.stmt_arena);
    arena_deinit(&state.string_arena);

    return rom_data;
}

static const char* gfuas_intern_string(gfuas_state* state, const char* s, gfu_uword length) {
    for (char* strings = state->string_arena.memory; strings < state->string_arena.memory + state->string_arena.allocated; ) {
        size_t existing_length = strlen(strings);
        if (existing_length == (size_t)length && 0 == strncmp(strings, s, (size_t)length)) {
            return strings;
            break;
        }

        strings += existing_length + 1;
    }

    char* new_string = arena_alloc(&state->string_arena, length + 1);
    memcpy(new_string, s, (size_t)length);
    new_string[length] = 0;
    return new_string;
}

#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

static void show_help(void) {
    bool use_color = isatty(fileno(stderr));

    fprintf(
        stderr,
        "fuasm [options...] <file>\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n"
        "\n"
        "  <file>          The single input GameFU ASM source file.\n"
        "  -o <file>, --output <file>\n"
        "                  Output ROM to <file>.\n"
    );
}

static void show_version(void) {
    fprintf(
        stderr,
        "GameFU Assembler version " VERSION "\n"
        "This tool is part of the GameFU Station fantasy console project toolchain.\n"
    );
}

static bool gfuas_options_parse(int argc, char** argv, gfuas_options* options) {
    bool result = true;
    options->program = SHIFT;

    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("--help", option)) {
            show_help();
            exit(0);
        } else if (0 == strcmp("--version", option)) {
            show_version();
            exit(0);
        }
    }

    options->source_count = 0;
    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("-v", option) || 0 == strcmp("--verbose", option)) {
            continue;
        } else if (0 == strcmp("-o", option) || 0 == strcmp("--output", option)) {
            i++;
            continue;
        } else options->source_count++;
    }

    if (options->source_count == 0) {
        show_help();
        fputc('\n', stderr);
        diag_issue(DIAG_ERROR, NOSOURCE, "No source file provided.");
        result = false;
    }

    options->sources = calloc(options->source_count, sizeof *options->sources);
    gfu_uword source_index = 0;

    while (argc > 0) {
        const char* option = SHIFT;

        if (0 == strcmp("-v", option) || 0 == strcmp("--verbose", option)) {
            options->verbose = true;
        } else if (0 == strcmp("-o", option) || 0 == strcmp("--output", option)) {
            options->output = SHIFT;
            if (options->output == nullptr) {
                diag_issue(DIAG_ERROR, NOSOURCE, "Option '%s' requires an argument '<file>'.", option);
                result = false;
            }
        } else {
            result &= load_source_from_file(option, &options->sources[source_index++]);
        }
    }

    return result;
}

#undef SHIFT

static void print_verbose(gfuas_state* state, const char* format, ...) {
    if (!state->options.verbose) return;
    va_list v;
    va_start(v, format);
    (void)vfprintf(stderr, format, v);
    va_end(v);
    fputc('\n', stderr);
}

#define ISEL_LABEL_NOPARENT 0xFFFFFFFFu
#define ISEL_LABEL_NOT_FOUND 0xFFFFFFFEu

static gfu_uword gfuas_lookup_address_raw(gfuas_state* state, gfu_uword parent, source source, gfu_uword location, const char* name) {
    assertn(state != nullptr);

    size_t name_length = strlen(name);

    for (gfu_uword i = 0; i < state->label_count; i++) {
        const char* addr_name = state->addresses[i].name;
        assertn(addr_name != nullptr);

        bool is_valid_scope = state->addresses[i].parent == ISEL_LABEL_NOPARENT ||
            parent == state->addresses[i].parent;
        if (!is_valid_scope) continue;

        size_t addr_name_length = strlen(addr_name);
        if (name_length != addr_name_length) continue;

        if (0 == strcmp(addr_name, name)) return state->addresses[i].address;
    }

    diag_issue(DIAG_ERROR, source, location, "No such label '%s' in this context.", name);
    // not actually necessary right now, as the error exits the program.
    return ISEL_LABEL_NOT_FOUND;
}

static gfu_uword gfuas_lookup_address(gfuas_state* state, gfu_uword parent, gfuas_token token) {
    return gfuas_lookup_address_raw(state, parent, token.source, token.location, token.as.label);
}

/* ===== ISel Shit ===== */

#include "isel_tables.c"

static isel_type gfuas_expr_kindo_isel_type(gfuas_expr expr) {
    switch (expr.kind) {
        default: return ISEL_TY_INVALID;

        case FUASM_EXPR_REG: return ISEL_TY_REG;

        case FUASM_EXPR_IMM:
        case FUASM_EXPR_IMM_UPPER:
        case FUASM_EXPR_IMM_LOWER:
            return ISEL_TY_IMM;

        case FUASM_EXPR_ADDR:
        case FUASM_EXPR_ADDR_UPPER:
        case FUASM_EXPR_ADDR_LOWER:
            return ISEL_TY_ADDR;

        case FUASM_EXPR_BYTE_STRING: return ISEL_TY_STR;
    }
}

static gfu_uword gfuas_match_isel(const gfuas_stmt* stmt) {
    for (gfu_uword pi = 0; pi < ISEL_PATTERN_COUNT; pi++) {
        isel_pattern pattern = isel_patterns[pi];

        const gfuas_stmt* match_stmt = stmt;
        for (gfu_uword mi = 0; mi < pattern.match_count; mi++, match_stmt = match_stmt->next) {
            if (match_stmt == nullptr) goto not_match;
            if (mi > 0 && match_stmt->label != nullptr) goto not_match;

            isel_match match = isel_matches[pattern.match_index + mi];
            if (match.mnemonic != stmt->mnemonic) goto not_match;
            if (match.operand_count != stmt->operand_count) goto not_match;

            for (gfu_uword oi = 0; oi < match.operand_count; oi++) {
                isel_operand op = match.operands[oi];
                gfuas_expr expr = stmt->operands[oi];

                if (op.is_base != expr.is_base) goto not_match;
                if (op.type != gfuas_expr_kindo_isel_type(expr)) {
                    goto not_match;
                }

                assertn(!op.is_constant);
            }
        }

        return pi;

    not_match:;
    }

    return 0xFFFFFFFF;
}

static bool gfuas_eval_expr(gfuas_state* state, gfu_uword label_scope, source source, gfuas_expr expr, gfu_uword* result) {
#define R(Value) do { *result = (Value); return true; } while (0)
    switch (expr.kind) {
        case FUASM_EXPR_INVALID: R(0);
        case FUASM_EXPR_BYTE_STRING: R(0);

        case FUASM_EXPR_REG: R((gfu_uword)expr.as._register);

        case FUASM_EXPR_IMM: R(expr.as.immediate);
        case FUASM_EXPR_IMM_LOWER: R(expr.as.immediate & 0xFFFF);
        case FUASM_EXPR_IMM_UPPER: R((expr.as.immediate >> 16) & 0xFFFF);

        case FUASM_EXPR_ADDR:
        case FUASM_EXPR_ADDR_LOWER:
        case FUASM_EXPR_ADDR_UPPER: {
            gfu_uword addr = gfuas_lookup_address_raw(state, label_scope, expr.source, expr.as.address.location, expr.as.address.as.label);
            if (addr == ISEL_LABEL_NOT_FOUND) return false;

            if (expr.kind == FUASM_EXPR_ADDR_LOWER) {
                R(addr & 0xFFFF);
            } else if (expr.kind == FUASM_EXPR_ADDR_UPPER) {
                R(addr >> 16);
            } else R(addr);
        }
    }

    diag_issue(DIAG_FATAL, NOSOURCE, "missing case, die");
    return false;
}

static gfu_uword gfuas_emit_isel(gfuas_state* state, gfu_uword label_scope, gfu_uword addr, const gfuas_stmt* ir, char* rom, const gfuas_expr* vars) {
    isel_pattern pattern = isel_patterns[ir->pattern_index];

    if (ir->mnemonic == FUASM_MNEM_BYTES) {
        gfuas_expr expr = vars[isel_emits[pattern.emit_index].arguments[0].as.var_index];
        if (expr.kind == FUASM_EXPR_BYTE_STRING) {
            memcpy(rom, expr.as.byte_string.data, (size_t)expr.as.byte_string.length);
            return expr.as.byte_string.length;
        } else if (expr.kind == FUASM_EXPR_IMM) {
            memset(rom, 0, (size_t)expr.as.immediate);
            return expr.as.immediate;
        } else {
            diag_issue(DIAG_FATAL, NOSOURCE, "unhandled `bytes` case in data section.");
        }
    }

    for (gfu_uword ei = 0; ei < pattern.emit_count; ei++, rom += sizeof(gfu_uword), addr += sizeof(gfu_uword)) {
        isel_emit emit = isel_emits[pattern.emit_index + ei];
        gfu_uword args[4] = {0};

        for (gfu_uword i = 0; i < emit.argument_count; i++) {
            isel_argument arg = emit.arguments[i];
            switch (arg.kind) {
                case ISEL_ARG_INVALID: break;

                case ISEL_ARG_REGISTER: {
                    args[i] = (gfu_uword)arg.as._register;
                } break;

                case ISEL_ARG_VAR:
                case ISEL_ARG_VAR_LOWER: {
                    gfuas_expr expr = vars[arg.as.var_index];
                    if (!gfuas_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] &= 0xFFFF;
                } break;

                case ISEL_ARG_VAR_UPPER: {
                    gfuas_expr expr = vars[arg.as.var_index];
                    if (!gfuas_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] = (args[i] >> 16) & 0xFFFF;
                } break;

                case ISEL_ARG_VAR_ADDR: {
                    gfuas_expr expr = vars[arg.as.var_index];
                    if (!gfuas_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] = (args[i] & 0x0FFFFFFC) >> 2;
                } break;

                case ISEL_ARG_VAR_NEGATE: {
                    gfuas_expr expr = vars[arg.as.var_index];
                    if (!gfuas_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] = (gfu_uword)(-(gfu_word)args[i]);
                } break;

                case ISEL_ARG_VAR_OFFS: {
                    gfuas_expr expr = vars[arg.as.var_index];
                    gfu_uword to;
                    if (!gfuas_eval_expr(state, label_scope, expr.source, expr, &to))
                        return 0;
                    gfu_uword from = addr + sizeof(gfu_uword);
                    args[i] = (gfu_half)(((to - from) & 0x0003FFFC) >> 2);
                } break;

                case ISEL_ARG_IMM:
                case ISEL_ARG_IMM_LOWER: {
                    args[i] = arg.as.immediate & 0xFFFF;
                } break;

                case ISEL_ARG_IMM_UPPER: {
                    args[i] = (arg.as.immediate >> 16) & 0xFFFF;
                } break;

                case ISEL_ARG_IMM_ADDR: {
                    args[i] = (arg.as.immediate & 0x0FFFFFFC) >> 2;
                } break;

                case ISEL_ARG_IMM_OFFS: {
                    gfu_uword from = addr + sizeof(gfu_uword);
                    args[i] = (gfu_half)(((arg.as.immediate - from) & 0x0003FFFC) >> 2);
                } break;
            }
        }

        gfu_inst* inst = (gfu_inst*)rom;
        switch (emit.kind) {
            case ISEL_EMIT_INVALID: break;
            case ISEL_EMIT_BYTES: break;

            case ISEL_EMIT_REG: {
                inst->reg.function = emit.op.function;
                inst->reg.d = args[0];
                inst->reg.l = args[1];
                inst->reg.r = args[2];
                inst->reg.shift = args[3];
            } break;

            case ISEL_EMIT_COP0: {
                inst->reg.opcode = GFU_OP_COP0;
                inst->reg.function = emit.op.cop0_function;
                inst->reg.d = args[0];
                inst->reg.l = args[1];
                inst->reg.r = args[2];
                inst->reg.shift = args[3];
            } break;

            case ISEL_EMIT_IMM: {
                inst->imm.opcode = emit.op.opcode;
                inst->imm.d = args[0];
                inst->imm.l = args[1];
                inst->imm.value = args[2];
            } break;

            case ISEL_EMIT_ADDR: {
                inst->addr.opcode = emit.op.opcode;
                inst->addr.value = args[0];
            } break;
        }
    }

    return pattern.emit_count * sizeof(gfu_uword);
}

/* ===== Parser Shit ===== */

static bool gfuas_parser_is_at_end(gfuas_parser* parser) {
    return parser->tk.kind == FUASM_TK_EOF;
}

static gfuas_token gfuas_parser_peek(gfuas_parser* parser) {
    if (parser->next.kind == FUASM_TK_INVALID) {
        parser->next = gfuas_lexer_read(&parser->lexer);
    }
    return parser->next;
}

static void gfuas_parser_advance(gfuas_parser* parser) {
    if (parser->tk.kind == FUASM_TK_EOF) return;
    if (parser->next.kind != FUASM_TK_INVALID) {
        parser->tk = parser->next;
        parser->next = (gfuas_token) { .kind = FUASM_TK_INVALID };
    } else parser->tk = gfuas_lexer_read(&parser->lexer);
    assertn(parser->next.kind == FUASM_TK_INVALID);
}

static bool gfuas_parserry(gfuas_parser* parser, gfuas_token_kind kind) {
    if (parser->tk.kind != kind) return false;
    gfuas_parser_advance(parser);
    return true;
}

static bool gfuas_parser_expect(gfuas_parser* parser, gfuas_token_kind kind, const char* what, gfuas_token* token) {
    if (parser->tk.kind != kind) {
        // gfuas_token_dump(parser->tk);
        if (kind >= FUASM_TK_PRINTABLE_BEGIN && kind <= FUASM_TK_PRINTABLE_END) {
            diag_issue(DIAG_FATAL, parser->source, parser->tk.location, "Expected '%c'.", kind);
        } else {
            diag_issue(DIAG_FATAL, parser->source, parser->tk.location, "Expected %s.", what);
        }

        return false;
    }

    if (token != nullptr) *token = parser->tk;
    gfuas_parser_advance(parser);
    return true;
}

static bool gfuas_parse_expr(gfuas_parser* parser, gfuas_expr* out_expr) {
    gfuas_expr expr = {0};
    expr.source = parser->source;
    expr.location = parser->tk.location;

    if (parser->tk.kind == FUASM_TK_LABEL_GLOBAL || parser->tk.kind == FUASM_TK_LABEL_LOCAL) {
        expr.kind = FUASM_EXPR_ADDR;
        expr.as.address.kind = FUASM_ADDR_LABEL;
        expr.as.address.location = parser->tk.location;
        expr.as.address.as.label = parser->tk.as.label;
        gfuas_parser_advance(parser);
    } else if (parser->tk.kind == FUASM_TK_REGISTER) {
        expr.kind = FUASM_EXPR_REG;
        expr.as._register = parser->tk.as._register;
        gfuas_parser_advance(parser);
    } else if (parser->tk.kind == FUASM_TK_IMMEDIATE) {
        expr.kind = FUASM_EXPR_IMM;
        expr.as.immediate = parser->tk.as.immediate;
        gfuas_parser_advance(parser);
    } else if (parser->tk.kind == FUASM_TK_BYTE_STRING) {
        expr.kind = FUASM_EXPR_BYTE_STRING;
        expr.as.byte_string.data = parser->tk.as.byte_string.data;
        expr.as.byte_string.length = parser->tk.as.byte_string.length;
        gfuas_parser_advance(parser);
    } else if (parser->tk.kind == '-') {
        gfuas_parser_advance(parser);
        if (parser->tk.kind == FUASM_TK_IMMEDIATE) {
            expr.kind = FUASM_EXPR_IMM;
            expr.as.immediate = (gfu_uword)(-(gfu_word)parser->tk.as.immediate);
            gfuas_parser_advance(parser);
        } else {
            diag_issue(DIAG_ERROR, parser->source, expr.location, "Expected an immediate to negate.");
            return false;
        }
    } else {
        diag_issue(DIAG_ERROR, parser->source, expr.location, "Expected an operand expression.");
        return false;
    }

    *out_expr = expr;
    return true;
}

static gfuas_stmt* parse_statement(gfuas_parser* parser) {
    static bool is_first = true;

    gfuas_state* state = parser->state;
    source source = parser->source;

    assertn(parser->tk.kind != FUASM_TK_ENDL && parser->tk.kind != FUASM_TK_EOF);

    gfuas_stmt* stmt = arena_alloc(&state->stmt_arena, sizeof *stmt);
    stmt->source = source;
    stmt->location = parser->tk.location;

    if (parser->tk.kind == FUASM_TK_DIRECTIVE) {
        stmt->directive = parser->tk.as.directive;
        gfuas_parser_advance(parser);

        if (stmt->directive == FUASM_DIR_ADDRESS_SPACE) {
            if (!is_first) {
                diag_issue(DIAG_ERROR, source, stmt->location, "'#address_space' directive must be the first statement.");
                return nullptr;
            }

            if (parser->tk.kind == FUASM_TK_LABEL_GLOBAL && 0 == strcmp("bios", parser->tk.as.label)) {
                state->address_space = GFUOBJ_ADDRSPACE_BIOS;
                gfuas_parser_advance(parser);
            } else if (parser->tk.kind == FUASM_TK_LABEL_GLOBAL && 0 == strcmp("user", parser->tk.as.label)) {
                state->address_space = GFUOBJ_ADDRSPACE_USER;
                gfuas_parser_advance(parser);
            } else {
                diag_issue(DIAG_ERROR, source, parser->tk.location, "Expected 'bios' or 'user'.");
                return nullptr;
            }
        } else if (stmt->directive == FUASM_DIR_ENTRY) {
            if (state->is_entry_set) {
                diag_issue(DIAG_ERROR, source, stmt->location, "'#entry' may only be specified once.");
                return nullptr;
            }

            gfuas_token entry_name_token;
            if (!gfuas_parser_expect(parser, FUASM_TK_LABEL_GLOBAL, "a global label", &entry_name_token)) {
                return nullptr;
            }

            state->entry_name = entry_name_token.as.label;
            state->is_entry_set = true;
        }

        goto stmt_end;
    }

    if (
        (parser->tk.kind == FUASM_TK_LABEL_GLOBAL || parser->tk.kind == FUASM_TK_LABEL_LOCAL) &&
        gfuas_parser_peek(parser).kind == ':'
    ) {
        stmt->label = parser->tk.as.label;
        stmt->is_label_local = parser->tk.kind == FUASM_TK_LABEL_LOCAL;
        gfuas_parser_advance(parser); // label
        gfuas_parser_advance(parser); // colon
    }

    if (parser->tk.kind == FUASM_TK_MNEMONIC) {
        stmt->mnemonic = parser->tk.as.mnemonic;
        gfuas_parser_advance(parser);

        if (parser->tk.kind == FUASM_TK_ENDL || parser->tk.kind == FUASM_TK_EOF) {
            goto stmt_end;
        }

        do {
            if ((size_t)stmt->operand_count >= sizeof(stmt->operands) / sizeof(stmt->operands[0])) {
                diag_issue(DIAG_ERROR, source, parser->tk.location, "Too many operands to statement.");
                return nullptr;
            }

            gfuas_expr* expr = &stmt->operands[stmt->operand_count++];
            if (!gfuas_parse_expr(parser, expr)) {
                return nullptr;
            }

            if (parser->tk.kind == '(') {
                if (expr->kind != FUASM_EXPR_IMM) {
                    diag_issue(DIAG_ERROR, source, parser->tk.location, "Base register syntax can only be applied after an immediate value.");
                    return nullptr;
                }

                if ((size_t)stmt->operand_count >= sizeof(stmt->operands) / sizeof(stmt->operands[0])) {
                    diag_issue(DIAG_ERROR, source, parser->tk.location, "Too many operands to statement.");
                    return nullptr;
                }

                gfuas_parser_advance(parser);
                gfuas_expr base_expr = {
                    .kind = FUASM_EXPR_REG,
                    .is_base = true,
                    .location = parser->tk.location,
                };

                gfuas_token tk;
                if (!gfuas_parser_expect(parser, FUASM_TK_REGISTER, "a register name", &tk)) {
                    return nullptr;
                }

                base_expr.as._register = tk.as._register;
                stmt->operands[stmt->operand_count++] = base_expr;

                if (!gfuas_parser_expect(parser, ')', nullptr, nullptr)) {
                    return nullptr;
                }
            }
        } while (gfuas_parserry(parser, ','));
    }

stmt_end:;
    is_first = false;

    if (parser->tk.kind != FUASM_TK_ENDL) {
        diag_issue(DIAG_ERROR, source, parser->tk.location, "Extra tokens at the end of a statement.");
        return nullptr;
    }

    while (parser->tk.kind != FUASM_TK_ENDL) {
        gfuas_parser_advance(parser);
    }

    assertn(parser->tk.kind == FUASM_TK_ENDL);
    gfuas_parser_advance(parser);

    return stmt;
}

static char* gfuas_assemble_ir_internal(gfuas_state* state, gfu_uword* rom_size) {
    gfuas_stmt* const ir = state->ir;
    gfuas_section section = FUASM_TEXT;

    /* Step 0: Collect label count */

    for (gfuas_stmt* stmt = ir; stmt != nullptr; stmt = stmt->next) {
        if (stmt->label != nullptr) {
            state->label_count++;
        }
    }

    /* Step 1: Validate the assembler statements */

    state->addresses = calloc((size_t)state->label_count, sizeof *state->addresses);

    gfu_uword text_instruction_count = 0;

    section = FUASM_TEXT;
    for (gfuas_stmt* stmt = ir; stmt != nullptr; ) {
        switch (stmt->directive) {
            default: assertn(false && "Unhandled directive"); break;
            case FUASM_DIR_INVALID: break;
            case FUASM_DIR_ADDRESS_SPACE: break;
            case FUASM_DIR_ENTRY: break;
            case FUASM_DIR_TEXT: section = FUASM_TEXT; break;
            case FUASM_DIR_DATA: section = FUASM_DATA; break;
        }

        if (stmt->mnemonic == FUASM_MNEM_INVALID) {
            stmt = stmt->next;
            continue;
        }

        stmt->pattern_index = gfuas_match_isel(stmt);
        if (stmt->pattern_index == 0xFFFFFFFF) {
            diag_issue(DIAG_ERROR, stmt->source, stmt->location, "Invalid instruction format.");
            return nullptr;
        }

        isel_pattern pattern = isel_patterns[stmt->pattern_index];
        assertn(pattern.match_count > 0);

        if (section == FUASM_TEXT) {
            text_instruction_count += pattern.emit_count;
        }

        for (gfu_uword i = 0; i < pattern.match_count; i++) {
            if (section == FUASM_TEXT && stmt->mnemonic == FUASM_MNEM_BYTES) {
                diag_issue(DIAG_ERROR, stmt->source, stmt->location, "'bytes' statement can only occur inside the #data section.");
                return nullptr;
            } else if (section == FUASM_DATA && stmt->mnemonic != FUASM_MNEM_BYTES) {
                diag_issue(DIAG_ERROR, stmt->source, stmt->location, "Currently only 'bytes' statement may occur inside the #data section.");
                return nullptr;
            }

            assertn(stmt != nullptr);
            stmt = stmt->next;
        }
    }

    /* Step 2: Calculate label addresses */

    gfu_uword label_index = 0;
    gfu_uword text_index = 0;
    gfu_uword data_index = 0;
    gfu_uword label_scope = ISEL_LABEL_NOPARENT;

    section = FUASM_TEXT;
    for (gfuas_stmt* stmt = ir; stmt != nullptr; ) {
        switch (stmt->directive) {
            default: assertn(false && "Unhandled directive"); break;
            case FUASM_DIR_INVALID: break;
            case FUASM_DIR_ADDRESS_SPACE: break;
            case FUASM_DIR_ENTRY: break;
            case FUASM_DIR_TEXT: section = FUASM_TEXT; break;
            case FUASM_DIR_DATA: section = FUASM_DATA; break;
        }

        if (stmt->label != nullptr) {
            gfu_uword relative_address = section == FUASM_TEXT
                ? text_index : data_index + (text_instruction_count * 4);
            gfu_uword parent_label = ISEL_LABEL_NOPARENT;

            if (stmt->is_label_local) {
                parent_label = label_scope;
            } else {
                label_scope = relative_address;
            }

            gfu_uword addr_offset = state->address_space == GFUOBJ_ADDRSPACE_USER
                ? GFU_KSEG0_BASE + GFU_BIOS_RAM_SIZE
                : GFU_KSEG1_BASE + GFU_BIOS_ROM_BASE;
            state->addresses[label_index++] = (struct address) {
                .name = stmt->label,
                .address = addr_offset + relative_address,
                .parent = parent_label,
            };
        }

        if (stmt->mnemonic == FUASM_MNEM_INVALID) {
            stmt = stmt->next;
            continue;
        }

        isel_pattern pattern = isel_patterns[stmt->pattern_index];
        gfuas_expr vars[pattern.var_count];

        const gfuas_stmt* match_stmt = stmt;
        for (gfu_uword mi = 0, var_index = 0; mi < pattern.match_count; mi++, match_stmt = match_stmt->next) {
            isel_match match = isel_matches[pattern.match_index + mi];
            for (gfu_uword oi = 0; oi < match.operand_count; oi++) {
                isel_operand op = match.operands[oi];
                if (!op.is_constant) {
                    vars[var_index++] = stmt->operands[oi];
                }
            }
        }

        if (section == FUASM_TEXT) {
            text_index += pattern.emit_count * sizeof(gfu_uword);
        } else if (section == FUASM_DATA) {
            assertn(pattern.match_count == 1);
            assertn(pattern.emit_count == 1);
            assertn(stmt->mnemonic == FUASM_MNEM_BYTES);
            assertn(stmt->operand_count == 1);
            isel_emit emit = isel_emits[pattern.emit_index];
            assertn(emit.arguments[0].kind == ISEL_ARG_VAR);
            gfuas_expr expr = vars[emit.arguments[0].as.var_index];
            if (expr.kind == FUASM_EXPR_BYTE_STRING) {
                data_index += expr.as.byte_string.length;
            } else if (expr.kind == FUASM_EXPR_IMM) {
                data_index += expr.as.immediate;
            } else {
                diag_issue(DIAG_FATAL, NOSOURCE, "unhandled `bytes` case in data section.");
            }
        }

        for (gfu_uword i = 0; i < pattern.match_count; i++) {
            stmt = stmt->next;
        }
    }

    assertn(text_index == text_instruction_count * sizeof(gfu_uword));

    print_verbose(state, "Label count: %d", state->label_count);
    print_verbose(state, "Instruction byte count: %d", text_index);
    print_verbose(state, "Instruction word count: %d", text_instruction_count);
    print_verbose(state, "Data byte count: %d", data_index);

    /* Step 3: Generate the final ROM binary */

    gfu_uword start_label_address = gfuas_lookup_address_raw(state, ISEL_LABEL_NOPARENT, (struct source){0}, 0, state->entry_name);
    if (start_label_address == 0xFFFFFFFFu) {
        diag_issue(DIAG_ERROR, NOSOURCE, "No '_start' label found.");
        return nullptr;
    }

    gfuobj_header_t rom_header = {
        .magic = GFUOBJ_MAGIC,
        .address_space = state->address_space,
        .entry_address = start_label_address,
    };

    const gfu_uword align = sizeof(gfu_uword);
    *rom_size = sizeof(rom_header) + text_index + (data_index + ((align - (data_index % align)) % align));
    rom_header.rom_size = *rom_size - sizeof(rom_header);
    char* rom_data = calloc((size_t)(*rom_size), sizeof *rom_data);
    char* rom_base = rom_data + sizeof(rom_header);
    print_verbose(state, "Final ROM size is %d bytes", *rom_size);

    memcpy(rom_data, &rom_header, sizeof(rom_header));

    text_index = 0;
    data_index = 0;
    label_scope = ISEL_LABEL_NOPARENT;

    section = FUASM_TEXT;
    for (gfuas_stmt* stmt = ir; stmt != nullptr; ) {
        switch (stmt->directive) {
            default: assertn(false && "Unhandled directive"); break;
            case FUASM_DIR_INVALID: break;
            case FUASM_DIR_ADDRESS_SPACE: break;
            case FUASM_DIR_ENTRY: break;
            case FUASM_DIR_TEXT: section = FUASM_TEXT; break;
            case FUASM_DIR_DATA: section = FUASM_DATA; break;
        }

        if (stmt->label != nullptr) {
            if (!stmt->is_label_local) {
                gfu_uword relative_address = section == FUASM_TEXT
                    ? text_index : data_index + text_index;
                label_scope = relative_address;
            }
        }

        if (stmt->mnemonic == FUASM_MNEM_INVALID) {
            stmt = stmt->next;
            continue;
        }

        isel_pattern pattern = isel_patterns[stmt->pattern_index];
        gfuas_expr vars[pattern.var_count];

        const gfuas_stmt* match_stmt = stmt;
        for (gfu_uword mi = 0, var_index = 0; mi < pattern.match_count; mi++, match_stmt = match_stmt->next) {
            isel_match match = isel_matches[pattern.match_index + mi];
            for (gfu_uword oi = 0; oi < match.operand_count; oi++) {
                isel_operand op = match.operands[oi];
                if (!op.is_constant) {
                    vars[var_index++] = stmt->operands[oi];
                }
            }
        }

        gfu_uword addr_offset = state->address_space == GFUOBJ_ADDRSPACE_USER
            ? GFU_KSEG0_BASE + GFU_BIOS_RAM_SIZE
            : GFU_KSEG1_BASE + GFU_BIOS_ROM_BASE;

        if (section == FUASM_TEXT) {
            gfu_uword icount = gfuas_emit_isel(state, label_scope, text_index + addr_offset, stmt, rom_base + text_index, vars);
            if (icount == 0) {
                free(rom_data);
                return nullptr;
            }
            text_index += icount;
        } else if (section == FUASM_DATA) {
            gfu_uword addr = (text_instruction_count * sizeof(gfu_uword)) + data_index;
            gfu_uword icount = gfuas_emit_isel(state, label_scope, addr + addr_offset, stmt, rom_base + addr, vars);
            if (icount == 0) {
                free(rom_data);
                return nullptr;
            }
            data_index += icount;
        }

        for (gfu_uword i = 0; i < pattern.match_count; i++) {
            stmt = stmt->next;
        }
    }

    return rom_data;
}

static char* gfuas_assemble_internal(gfuas_state* state, gfu_uword* rom_size) {
    gfuas_stmt* head = nullptr;
    gfuas_stmt* tail = nullptr;

    for (gfu_uword i = 0; i < state->options.source_count; i++) {
        source source = state->options.sources[i];

        gfuas_parser parser = {
            .state = state,
            .source = source,
            .tk.kind = FUASM_TK_INVALID,
            .next.kind = FUASM_TK_INVALID,
        };

        gfuas_userdata userdata = {
            .state = state,
            .source = source,
        };
        gfuas_lexer_init(&userdata, &parser.lexer);

        parser.tk = gfuas_lexer_read(&parser.lexer);
        while (parser.tk.kind != FUASM_TK_EOF) {
            while (parser.tk.kind == FUASM_TK_ENDL) {
                gfuas_parser_advance(&parser);
            }

            if (parser.tk.kind == FUASM_TK_EOF) break;

            gfu_uword location  = parser.tk.location;
            gfuas_stmt* stmt = parse_statement(&parser);
            if (stmt == nullptr) return nullptr;
            assertn(location != parser.tk.location);

            if (head == nullptr) {
                head = tail = stmt;
            } else {
                tail->next = stmt;
                tail = stmt;
            }
        }
    }

    state->ir = head;
    return gfuas_assemble_ir_internal(state, rom_size);
}

/* ===== Lexer Shit ===== */

static const char* gfuas_token_kind_names[] = {
    [FUASM_TK_INVALID] = "INVALID",
    [FUASM_TK_EOF] = "EOF",
#define X(Id) [FUASM_TK_##Id] = #Id,
    FUASM_TOKEN_KINDS(X)
#undef X
};

static void gfuas_token_dump(gfuas_token token) {
    if (token.kind >= FUASM_TK_PRINTABLE_BEGIN && token.kind <= FUASM_TK_PRINTABLE_END) {
        fprintf(
            stderr,
            "(%d,%d):  %c",
            token.line,
            token.column,
            token.kind
        );
    } else {
        fprintf(
            stderr,
            "(%d,%d): [%s]  %.*s",
            token.line,
            token.column,
            gfuas_token_kind_names[token.kind],
            (int)(token.end - token.begin),
            token.begin
        );
    }

    fputc('\n', stderr);
}

static gfu_uword gfuas_lexer_location(etok_lexer* lexer) {
    return (gfu_uword)(lexer->source_current - lexer->source_begin);
}

static void gfuas_etok_error_callback(void* userdata, const char* source_name, const char* source_text, const char* where, uint64_t line, uint64_t column, const char* format, ...) {
    gfuas_userdata* data = userdata;

    va_list v;
    va_start(v, format);
    diag_issue_v(DIAG_ERROR, data->source, (uint32_t)(where - source_text), format, v);
    va_end(v);
}

static bool gfuas_etok_comment_consumer(etok_lexer* lexer) {
    if (lexer->ch != ';') return false;

    while (!etok_lexer_is_at_end(lexer) && lexer->ch != '\n') {
        etok_lexer_advance(lexer);
    }

    return true;
}

static bool gfuas_is_white_space(void* userdata, int ch) {
    return ch == ' ' || ch == '\f' || ch == '\r' || ch == '\t' || ch == '\v';
}

static bool gfuas_is_ident_begin(void* userdata, int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '.' || ch == '_';
}

static bool gfuas_is_ident_continue(void* userdata, int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '_';
}

static int gfuas_get_hex_digit_value(int ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return 10 + (ch - 'a');
    if (ch >= 'A' && ch <= 'F') return 10 + (ch - 'A');
    return -1;
}

static bool gfuas_lexer_is_hex_digit(int ch) {
    return gfuas_get_hex_digit_value(ch) >= 0;
}

static void gfuas_lexer_init(gfuas_userdata* userdata, etok_lexer* lexer) {
    etok_lexer_init(lexer, userdata->source.name, userdata->source.text, userdata->source.text + userdata->source.length);

    lexer->userdata = userdata;
    lexer->error_callback = gfuas_etok_error_callback;
    lexer->comment_consumer = gfuas_etok_comment_consumer;
    lexer->white_space_predicate = gfuas_is_white_space;
    lexer->ident_begin_predicate = gfuas_is_ident_begin;
    lexer->ident_continue_predicate = gfuas_is_ident_continue;

    etok_lexer_advance(lexer);
}

static gfuas_token gfuas_lexer_read(etok_lexer* lexer) {
    if (lexer == nullptr) return (gfuas_token) {0};

    assertn(lexer->userdata != nullptr);
    gfuas_userdata* userdata = lexer->userdata;

    gfuas_state* state = userdata->state;
    assertn(state != nullptr);

    source source = userdata->source;

    etok_lexer_skip_white_space(lexer);
    gfuas_token token = {
        .source = source,
        .kind = FUASM_TK_INVALID,
        .location = gfuas_lexer_location(lexer),
        .begin = lexer->source_current,
        .line = (gfu_uword)lexer->line,
        .column = (gfu_uword)lexer->column,
    };

    if (etok_lexer_is_at_end(lexer)) {
        token.kind = FUASM_TK_EOF;
        token.end = token.begin;
        return token;
    }

    int ch = lexer->ch;
    switch (ch) {
        case '\n': case '\0': {
            token.kind = FUASM_TK_ENDL;
            etok_lexer_advance(lexer);
        } break;

        case '%': {
            etok_lexer_advance(lexer);
            token.kind = FUASM_TK_DIRECTIVE;

            const char* name = lexer->source_current;
            while (!etok_lexer_is_at_end(lexer) && etok_lexer_is_ident_continue(lexer, lexer->ch)) {
                etok_lexer_advance(lexer);
            }

            size_t length = (size_t)(lexer->source_current - name);
            if (length == 0) {
                diag_issue(DIAG_ERROR, source, token.location, "Expected a directive name.");
                goto return_token;
            }

            static struct {
                gfuas_directive directive;
                const char* image;
            } directives[] = {
#define X(Id, Image) { FUASM_DIR_##Id, Image },
                FUASM_DIRECTIVES(X)
#undef X
                {0},
            };

            for (int i = 0; directives[i].image != nullptr; i++) {
                if (0 == strncmp(name, directives[i].image, length)) {
                    token.as.directive = directives[i].directive;
                    break;
                }
            }

            if (token.as.directive == FUASM_DIR_INVALID) {
                diag_issue(DIAG_ERROR, source, token.location, "Invalid directive.");
                goto return_token;
            }
        } break;

        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
        case '.': case '_': {
            etok_lexer_advance(lexer);

            const char* name = token.begin;
            while (!etok_lexer_is_at_end(lexer) && etok_lexer_is_ident_continue(lexer, lexer->ch)) {
                etok_lexer_advance(lexer);
            }

            size_t length = (size_t)(lexer->source_current - name);

            static struct {
                gfuas_mnemonic mnemonic;
                const char* image;
            } mnemonics[] = {
#define MNEM(Id, Name) { FUASM_MNEM_##Id, Name },
#include "x/mnemonics.h"
                {0},
            };

            static struct {
                gfu_gpr _register;
                const char* image;
            } registers[] = {
#define X(Id, Image) { GFU_GPR_##Id, Image },
                GFU_GPR(X)
#undef X
                {0},
            };

            for (int i = 0; mnemonics[i].image != nullptr; i++) {
                size_t mnemlength = strlen(mnemonics[i].image);
                if (length != mnemlength) continue;
                if (0 == strncmp(name, mnemonics[i].image, length)) {
                    token.as.mnemonic = mnemonics[i].mnemonic;
                    break;
                }
            }

            if (token.as.mnemonic != FUASM_MNEM_INVALID) {
                token.kind = FUASM_TK_MNEMONIC;
                break;
            }

            token.as._register = 255;
            for (int i = 0; registers[i].image != nullptr; i++) {
                if (0 == strncmp(name, registers[i].image, length)) {
                    token.as._register = registers[i]._register;
                    break;
                }
            }

            if (token.as._register != 255)  {
                token.kind = FUASM_TK_REGISTER;
                break;
            }

            token.as._register = 0;
            token.kind = *name == '.' ? FUASM_TK_LABEL_LOCAL : FUASM_TK_LABEL_GLOBAL;
            token.as.label = gfuas_intern_string(state, name, (gfu_uword)length);
        } break;

        case '#': {
            etok_lexer_advance(lexer);

            token.kind = FUASM_TK_IMMEDIATE;
            if (!gfuas_lexer_is_hex_digit(lexer->ch)) {
                diag_issue(DIAG_ERROR, source, gfuas_lexer_location(lexer), "Expected a hex digit.");
                goto return_token;
            }

            while (!etok_lexer_is_at_end(lexer) && gfuas_lexer_is_hex_digit(lexer->ch)) {
                token.as.immediate = (16 * token.as.immediate) + gfuas_get_hex_digit_value(lexer->ch);
                etok_lexer_advance(lexer);
            }
        } break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            gfu_long immediate = lexer->ch - '0';
            etok_lexer_advance(lexer);

            while (!etok_lexer_is_at_end(lexer) && etok_lexer_is_decimal_digit(lexer, lexer->ch)) {
                immediate = (10 * immediate) + (lexer->ch - '0');
                etok_lexer_advance(lexer);
            }

            token.kind = FUASM_TK_IMMEDIATE;
            token.as.immediate = (gfu_uword)immediate;
        } break;

        case '"': {
            etok_lexer_advance(lexer);

            size_t max_length = 0;
            while (
                lexer->source_current + max_length < lexer->source_end &&
                lexer->source_current[max_length] != '\n' &&
                lexer->source_current[max_length] != '"'
            ) {
                max_length++;
            }

            char* byte_string = arena_alloc(&state->string_arena, max_length);
            gfu_uword length = 0;

            while (!etok_lexer_is_at_end(lexer) && lexer->ch != '\n' && lexer->ch != '"') {
                if (lexer->ch == '\\') {
                    etok_lexer_advance(lexer);
                    int digits[2];
                    for (int i = 0; i < 2; i++) {
                        if (etok_lexer_is_at_end(lexer) || lexer->ch == '\n' || lexer->ch == '"' || (digits[i] = gfuas_get_hex_digit_value(lexer->ch), digits[i] < 0)) {
                            diag_issue(DIAG_ERROR, source, gfuas_lexer_location(lexer), "Expected hex digit in escape sequence.");
                            digits[i] = 0;
                        }
                        etok_lexer_advance(lexer);
                    }
                    byte_string[length++] = (char)((digits[0] * 16) + digits[1]);
                } else {
                    byte_string[length++] = lexer->ch;
                    etok_lexer_advance(lexer);
                }
            }

            if (lexer->ch != '"') {
                diag_issue(DIAG_ERROR, source, token.location, "Unterminated byte-string constant.");
                goto return_token;
            }

            etok_lexer_advance(lexer);

            token.kind = FUASM_TK_BYTE_STRING;
            token.as.byte_string.data = byte_string;
            token.as.byte_string.length = length;
        } break;

        default: {
            if (ch >= FUASM_TK_PRINTABLE_BEGIN && ch <= FUASM_TK_PRINTABLE_END) {
                token.kind = ch;
                etok_lexer_advance(lexer);
            } else {
                /* Somewhat catch-all for generic character errors. */
                if (ch < 32) {
                    diag_issue(DIAG_ERROR, source, token.location, "Invalid or unexpected character in source text (0x%02X).", ch);
                } else if (ch < 128) {
                    diag_issue(DIAG_ERROR, source, token.location, "Invalid or unexpected character in source text '%c'.", (char)ch);
                } else {
                    diag_issue(DIAG_ERROR, source, token.location, "Invalid or unexpected character in source text (0x%08X).", ch);
                }

                token.kind = FUASM_TK_INVALID_CHARACTER;
                etok_lexer_advance(lexer);
            }
        } break;
    }

return_token:;
    assertn(token.kind != FUASM_TK_INVALID);
    token.end = lexer->source_current;
    return token;
}

#include "../common/diagnostic.c"
#include "../common/arena.c"
#include "../common/source.c"
