#include "../common/common.h"

#include <gamefu/memory.h>
#include <gamefu/diagnostic.h>
#include <gamefu/source.h>

#include <gamefu/asm.h>
#include <gamefu/isel.h>

#if defined(__linux__)
#  include <unistd.h>
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define ECHOETOK_IMPL
#include <echoetok.h>

#define VERSION "0.0.1.0"

typedef struct fuasm_options {
    const char* program;
    bool verbose;
    source* sources;
    gfu_uword_t source_count;
    const char* output;
} fuasm_options;

typedef struct fuasm_state {
    fuasm_options options;

    gfu_arena_t string_arena;
    gfu_arena_t stmt_arena;

    fuasm_stmt* ir;
    struct address {
        const char* name;
        gfu_uword_t address;
        gfu_uword_t parent;
    }* addresses;
    gfu_uword_t label_count;

    gfu_uword_t address_space;
    const char* entry_name;
    bool is_entry_set;
} fuasm_state;

#define FUASM_TOKEN_KINDS(X) \
    X(ENDL) \
    X(DIRECTIVE) \
    X(LABEL_GLOBAL) \
    X(LABEL_LOCAL) \
    X(MNEMONIC) \
    X(REGISTER) \
    X(IMMEDIATE) \
    X(BYTE_STRING)

typedef enum fuasm_token_kind {
    FUASM_TK_INVALID = 0x00FFFFFF,
    FUASM_TK_EOF = 0,

    FUASM_TK_PRINTABLE_BEGIN = ETOK_PRINTABLE_BEGIN,
    FUASM_TK_PRINTABLE_END = ETOK_PRINTABLE_END,

    FUASM_TK_INVALID_CHARACTER = ETOK_INVALID_CHARACTER,

    _fuasm_tk_multibyte_offset = ETOK_MULTIBYTE_BEGIN,

#define X(Id) FUASM_TK_##Id,
    FUASM_TOKEN_KINDS(X)
#undef X
} fuasm_token_kind;

typedef struct fuasm_token {
    source source;
    fuasm_token_kind kind;
    gfu_uword_t location;
    const char *begin, *end;
    gfu_uword_t line, column;
    union {
        fuasm_directive directive;
        fuasm_mnemonic mnemonic;
        gfu_gpr_t _register;
        gfu_uword_t immediate;
        const char* label;
        struct {
            const char* data;
            gfu_uword_t length;
        } byte_string;
    } as;
} fuasm_token;

typedef struct fuasm_userdata {
    fuasm_state* state;
    source source;
} fuasm_userdata;

typedef struct fuasm_parser {
    fuasm_state* state;
    source source;
    etok_lexer lexer;
    fuasm_token tk, next;
} fuasm_parser;

#define PDATA ((fuasm_userdata) {parser->state, parser->source})

typedef enum fuasm_section {
    FUASM_TEXT,
    FUASM_DATA,
} fuasm_section;

static void show_help(void);
static void show_version(void);
static bool fuasm_options_parse(int argc, char** argv, fuasm_options* options);
static void print_verbose(fuasm_state* state, const char* format, ...);

static char* fuasm_assemble_ir_internal(fuasm_state* state, gfu_uword_t* rom_size);
static char* fuasm_assemble_internal(fuasm_state* state, gfu_uword_t* rom_size);

static void fuasm_lexer_init(fuasm_userdata* userdata, etok_lexer* lexer);
static fuasm_token fuasm_lexer_read(etok_lexer* lexer);
static void fuasm_token_dump(fuasm_token token);

int fuasm_driver_main(int argc, char** argv) {
    int result = 0;

    FILE* f = nullptr;

    diag_color_output(isatty(fileno(stderr)));

    fuasm_state state = {
        .address_space = GFUOBJ_ADDRSPACE_USER,
        .entry_name = "_start",
    };

    if (!fuasm_options_parse(argc, argv, &state.options)) {
        return_defer(1);
    }

    gfu_arena_init(&state.string_arena, 32 * 1024 * 1024);
    gfu_arena_init(&state.stmt_arena, 32 * 1024 * 1024);

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

        char* new_output_name = gfu_arena_alloc(&state.string_arena, (size_t)(last_dot - source_name) + 5);
        sprintf(new_output_name, "%.*s.gfu", (int)(last_dot - source_name), source_name);
        output_name = new_output_name;
    }

    print_verbose(&state, "Generating '%s'", output_name);

    gfu_uword_t rom_size;
    char* rom_data = fuasm_assemble_internal(&state, &rom_size);
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

    for (gfu_uword_t i = 0; i < state.options.source_count; i++) {
        free((char*)state.options.sources[i].text);
    }

    free(state.options.sources);

    gfu_arena_deinit(&state.stmt_arena);
    gfu_arena_deinit(&state.string_arena);

    return result;
}

int fuasm_driver_fuzz(const char* text, size_t length) {
    int result = 0;

    fuasm_state state = {
        .address_space = GFUOBJ_ADDRSPACE_USER,
        .entry_name = "_start",
    };

    state.options.source_count = 1;
    state.options.sources = calloc(1, sizeof *state.options.sources);
    state.options.sources[0] = (struct source) {
        .name = "<fuzz-input>",
        .text = text,
        .length = (gfu_word_t)length,
    };

    gfu_arena_init(&state.string_arena, 32 * 1024 * 1024);
    gfu_arena_init(&state.stmt_arena, 32 * 1024 * 1024);

    gfu_uword_t rom_size;
    char* rom_data = fuasm_assemble_internal(&state, &rom_size);

defer:;
    free(rom_data);
    free(state.addresses);
    free(state.options.sources);
    gfu_arena_deinit(&state.stmt_arena);
    gfu_arena_deinit(&state.string_arena);
    return result;
}

char* fuasm_assemble_ir(fuasm_stmt* ir, gfu_uword_t* rom_size) {
    fuasm_state state = {
        .ir = ir,
    };

    state.options.source_count = 1;
    state.options.sources = calloc(1, sizeof *state.options.sources);
    state.options.sources[0] = (struct source) {
        .name = "<noinput>",
        .text = "\n",
        .length = 1,
    };

    gfu_arena_init(&state.string_arena, 32 * 1024 * 1024);
    gfu_arena_init(&state.stmt_arena, 32 * 1024 * 1024);

    char* rom_data = fuasm_assemble_ir_internal(&state, rom_size);

    free(state.options.sources);
    gfu_arena_deinit(&state.stmt_arena);
    gfu_arena_deinit(&state.string_arena);
    return rom_data;
}

char* fuasm_assemble(source source, gfu_uword_t* rom_size) {
    fuasm_state state = {0};

    state.options.source_count = 1;
    state.options.sources = calloc(1, sizeof *state.options.sources);
    state.options.sources[0] = source;

    gfu_arena_init(&state.string_arena, 32 * 1024 * 1024);
    gfu_arena_init(&state.stmt_arena, 32 * 1024 * 1024);

    char* rom_data = fuasm_assemble_internal(&state, rom_size);

    free(state.options.sources);
    gfu_arena_deinit(&state.stmt_arena);
    gfu_arena_deinit(&state.string_arena);

    return rom_data;
}

static const char* fuasm_intern_string(fuasm_state* state, const char* s, gfu_uword_t length) {
    for (char* strings = state->string_arena.memory; strings < state->string_arena.memory + state->string_arena.allocated; ) {
        size_t existing_length = strlen(strings);
        if (existing_length == (size_t)length && 0 == strncmp(strings, s, (size_t)length)) {
            return strings;
            break;
        }

        strings += existing_length + 1;
    }

    char* new_string = gfu_arena_alloc(&state->string_arena, length + 1);
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

static bool fuasm_options_parse(int argc, char** argv, fuasm_options* options) {
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
    gfu_uword_t source_index = 0;

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

static void print_verbose(fuasm_state* state, const char* format, ...) {
    if (!state->options.verbose) return;
    va_list v;
    va_start(v, format);
    (void)vfprintf(stderr, format, v);
    va_end(v);
    fputc('\n', stderr);
}

#define ISEL_LABEL_NOPARENT 0xFFFFFFFFu
#define ISEL_LABEL_NOT_FOUND 0xFFFFFFFEu

static gfu_uword_t fuasm_lookup_address_raw(fuasm_state* state, gfu_uword_t parent, source source, gfu_uword_t location, const char* name) {
    fu_assert(state != nullptr);

    size_t name_length = strlen(name);

    for (gfu_uword_t i = 0; i < state->label_count; i++) {
        const char* addr_name = state->addresses[i].name;
        fu_assert(addr_name != nullptr);

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

static gfu_uword_t fuasm_lookup_address(fuasm_state* state, gfu_uword_t parent, fuasm_token token) {
    return fuasm_lookup_address_raw(state, parent, token.source, token.location, token.as.label);
}

/* ===== ISel Shit ===== */

#include "isel_tables.c"

static isel_type fuasm_expr_kind_to_isel_type(fuasm_expr expr) {
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

static gfu_uword_t fuasm_match_isel(const fuasm_stmt* stmt) {
    for (gfu_uword_t pi = 0; pi < ISEL_PATTERN_COUNT; pi++) {
        isel_pattern pattern = isel_patterns[pi];

        const fuasm_stmt* match_stmt = stmt;
        for (gfu_uword_t mi = 0; mi < pattern.match_count; mi++, match_stmt = match_stmt->next) {
            if (match_stmt == nullptr) goto not_match;
            if (mi > 0 && match_stmt->label != nullptr) goto not_match;

            isel_match match = isel_matches[pattern.match_index + mi];
            if (match.mnemonic != stmt->mnemonic) goto not_match;
            if (match.operand_count != stmt->operand_count) goto not_match;

            for (gfu_uword_t oi = 0; oi < match.operand_count; oi++) {
                isel_operand op = match.operands[oi];
                fuasm_expr expr = stmt->operands[oi];

                if (op.is_base != expr.is_base) goto not_match;
                if (op.type != fuasm_expr_kind_to_isel_type(expr)) {
                    goto not_match;
                }

                fu_assert(!op.is_constant);
            }
        }

        return pi;

    not_match:;
    }

    return 0xFFFFFFFF;
}

static bool fuasm_eval_expr(fuasm_state* state, gfu_uword_t label_scope, source source, fuasm_expr expr, gfu_uword_t* result) {
#define R(Value) do { *result = (Value); return true; } while (0)
    switch (expr.kind) {
        case FUASM_EXPR_INVALID: R(0);
        case FUASM_EXPR_BYTE_STRING: R(0);

        case FUASM_EXPR_REG: R((gfu_uword_t)expr.as._register);

        case FUASM_EXPR_IMM: R(expr.as.immediate);
        case FUASM_EXPR_IMM_LOWER: R(expr.as.immediate & 0xFFFF);
        case FUASM_EXPR_IMM_UPPER: R((expr.as.immediate >> 16) & 0xFFFF);

        case FUASM_EXPR_ADDR:
        case FUASM_EXPR_ADDR_LOWER:
        case FUASM_EXPR_ADDR_UPPER: {
            gfu_uword_t addr = fuasm_lookup_address_raw(state, label_scope, expr.source, expr.as.address.location, expr.as.address.as.label);
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

static gfu_uword_t fuasm_emit_isel(fuasm_state* state, gfu_uword_t label_scope, gfu_uword_t addr, const fuasm_stmt* ir, char* rom, const fuasm_expr* vars) {
    isel_pattern pattern = isel_patterns[ir->pattern_index];

    if (ir->mnemonic == FUASM_MNEM_BYTES) {
        fuasm_expr expr = vars[isel_emits[pattern.emit_index].arguments[0].as.var_index];
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

    for (gfu_uword_t ei = 0; ei < pattern.emit_count; ei++, rom += sizeof(gfu_uword_t), addr += sizeof(gfu_uword_t)) {
        isel_emit emit = isel_emits[pattern.emit_index + ei];
        gfu_uword_t args[4] = {0};

        for (gfu_uword_t i = 0; i < emit.argument_count; i++) {
            isel_argument arg = emit.arguments[i];
            switch (arg.kind) {
                case ISEL_ARG_INVALID: break;

                case ISEL_ARG_REGISTER: {
                    args[i] = (gfu_uword_t)arg.as._register;
                } break;

                case ISEL_ARG_VAR:
                case ISEL_ARG_VAR_LOWER: {
                    fuasm_expr expr = vars[arg.as.var_index];
                    if (!fuasm_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] &= 0xFFFF;
                } break;

                case ISEL_ARG_VAR_UPPER: {
                    fuasm_expr expr = vars[arg.as.var_index];
                    if (!fuasm_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] = (args[i] >> 16) & 0xFFFF;
                } break;

                case ISEL_ARG_VAR_ADDR: {
                    fuasm_expr expr = vars[arg.as.var_index];
                    if (!fuasm_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] = (args[i] & 0x0FFFFFFC) >> 2;
                } break;

                case ISEL_ARG_VAR_NEGATE: {
                    fuasm_expr expr = vars[arg.as.var_index];
                    if (!fuasm_eval_expr(state, label_scope, expr.source, expr, &args[i]))
                        return 0;
                    args[i] = (gfu_uword_t)(-(gfu_word_t)args[i]);
                } break;

                case ISEL_ARG_VAR_OFFS: {
                    fuasm_expr expr = vars[arg.as.var_index];
                    gfu_uword_t to;
                    if (!fuasm_eval_expr(state, label_scope, expr.source, expr, &to))
                        return 0;
                    gfu_uword_t from = addr + sizeof(gfu_uword_t);
                    args[i] = (gfu_half_t)(((to - from) & 0x0003FFFC) >> 2);
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
                    gfu_uword_t from = addr + sizeof(gfu_uword_t);
                    args[i] = (gfu_half_t)(((arg.as.immediate - from) & 0x0003FFFC) >> 2);
                } break;
            }
        }

        fuasm_inst* inst = (fuasm_inst*)rom;
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

    return pattern.emit_count * sizeof(gfu_uword_t);
}

/* ===== Parser Shit ===== */

static bool fuasm_parser_is_at_end(fuasm_parser* parser) {
    return parser->tk.kind == FUASM_TK_EOF;
}

static fuasm_token fuasm_parser_peek(fuasm_parser* parser) {
    if (parser->next.kind == FUASM_TK_INVALID) {
        parser->next = fuasm_lexer_read(&parser->lexer);
    }
    return parser->next;
}

static void fuasm_parser_advance(fuasm_parser* parser) {
    if (parser->tk.kind == FUASM_TK_EOF) return;
    if (parser->next.kind != FUASM_TK_INVALID) {
        parser->tk = parser->next;
        parser->next = (fuasm_token) { .kind = FUASM_TK_INVALID };
    } else parser->tk = fuasm_lexer_read(&parser->lexer);
    fu_assert(parser->next.kind == FUASM_TK_INVALID);
}

static bool fuasm_parser_try(fuasm_parser* parser, fuasm_token_kind kind) {
    if (parser->tk.kind != kind) return false;
    fuasm_parser_advance(parser);
    return true;
}

static bool fuasm_parser_expect(fuasm_parser* parser, fuasm_token_kind kind, const char* what, fuasm_token* token) {
    if (parser->tk.kind != kind) {
        // fuasm_token_dump(parser->tk);
        if (kind >= FUASM_TK_PRINTABLE_BEGIN && kind <= FUASM_TK_PRINTABLE_END) {
            diag_issue(DIAG_FATAL, parser->source, parser->tk.location, "Expected '%c'.", kind);
        } else {
            diag_issue(DIAG_FATAL, parser->source, parser->tk.location, "Expected %s.", what);
        }

        return false;
    }

    if (token != nullptr) *token = parser->tk;
    fuasm_parser_advance(parser);
    return true;
}

static bool fuasm_parse_expr(fuasm_parser* parser, fuasm_expr* out_expr) {
    fuasm_expr expr = {0};
    expr.source = parser->source;
    expr.location = parser->tk.location;

    if (parser->tk.kind == FUASM_TK_LABEL_GLOBAL || parser->tk.kind == FUASM_TK_LABEL_LOCAL) {
        expr.kind = FUASM_EXPR_ADDR;
        expr.as.address.kind = FUASM_ADDR_LABEL;
        expr.as.address.location = parser->tk.location;
        expr.as.address.as.label = parser->tk.as.label;
        fuasm_parser_advance(parser);
    } else if (parser->tk.kind == FUASM_TK_REGISTER) {
        expr.kind = FUASM_EXPR_REG;
        expr.as._register = parser->tk.as._register;
        fuasm_parser_advance(parser);
    } else if (parser->tk.kind == FUASM_TK_IMMEDIATE) {
        expr.kind = FUASM_EXPR_IMM;
        expr.as.immediate = parser->tk.as.immediate;
        fuasm_parser_advance(parser);
    } else if (parser->tk.kind == FUASM_TK_BYTE_STRING) {
        expr.kind = FUASM_EXPR_BYTE_STRING;
        expr.as.byte_string.data = parser->tk.as.byte_string.data;
        expr.as.byte_string.length = parser->tk.as.byte_string.length;
        fuasm_parser_advance(parser);
    } else if (parser->tk.kind == '-') {
        fuasm_parser_advance(parser);
        if (parser->tk.kind == FUASM_TK_IMMEDIATE) {
            expr.kind = FUASM_EXPR_IMM;
            expr.as.immediate = (gfu_uword_t)(-(gfu_word_t)parser->tk.as.immediate);
            fuasm_parser_advance(parser);
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

static fuasm_stmt* parse_statement(fuasm_parser* parser) {
    static bool is_first = true;

    fuasm_state* state = parser->state;
    source source = parser->source;

    fu_assert(parser->tk.kind != FUASM_TK_ENDL && parser->tk.kind != FUASM_TK_EOF);

    fuasm_stmt* stmt = gfu_arena_alloc(&state->stmt_arena, sizeof *stmt);
    stmt->source = source;
    stmt->location = parser->tk.location;

    if (parser->tk.kind == FUASM_TK_DIRECTIVE) {
        stmt->directive = parser->tk.as.directive;
        fuasm_parser_advance(parser);

        if (stmt->directive == FUASM_DIR_ADDRESS_SPACE) {
            if (!is_first) {
                diag_issue(DIAG_ERROR, source, stmt->location, "'#address_space' directive must be the first statement.");
                return nullptr;
            }

            if (parser->tk.kind == FUASM_TK_LABEL_GLOBAL && 0 == strcmp("bios", parser->tk.as.label)) {
                state->address_space = GFUOBJ_ADDRSPACE_BIOS;
                fuasm_parser_advance(parser);
            } else if (parser->tk.kind == FUASM_TK_LABEL_GLOBAL && 0 == strcmp("user", parser->tk.as.label)) {
                state->address_space = GFUOBJ_ADDRSPACE_USER;
                fuasm_parser_advance(parser);
            } else {
                diag_issue(DIAG_ERROR, source, parser->tk.location, "Expected 'bios' or 'user'.");
                return nullptr;
            }
        } else if (stmt->directive == FUASM_DIR_ENTRY) {
            if (state->is_entry_set) {
                diag_issue(DIAG_ERROR, source, stmt->location, "'#entry' may only be specified once.");
                return nullptr;
            }

            fuasm_token entry_name_token;
            if (!fuasm_parser_expect(parser, FUASM_TK_LABEL_GLOBAL, "a global label", &entry_name_token)) {
                return nullptr;
            }

            state->entry_name = entry_name_token.as.label;
            state->is_entry_set = true;
        }

        goto stmt_end;
    }

    if (
        (parser->tk.kind == FUASM_TK_LABEL_GLOBAL || parser->tk.kind == FUASM_TK_LABEL_LOCAL) &&
        fuasm_parser_peek(parser).kind == ':'
    ) {
        stmt->label = parser->tk.as.label;
        stmt->is_label_local = parser->tk.kind == FUASM_TK_LABEL_LOCAL;
        fuasm_parser_advance(parser); // label
        fuasm_parser_advance(parser); // colon
    }

    if (parser->tk.kind == FUASM_TK_MNEMONIC) {
        stmt->mnemonic = parser->tk.as.mnemonic;
        fuasm_parser_advance(parser);

        if (parser->tk.kind == FUASM_TK_ENDL || parser->tk.kind == FUASM_TK_EOF) {
            goto stmt_end;
        }

        do {
            if ((size_t)stmt->operand_count >= sizeof(stmt->operands) / sizeof(stmt->operands[0])) {
                diag_issue(DIAG_ERROR, source, parser->tk.location, "Too many operands to statement.");
                return nullptr;
            }

            fuasm_expr* expr = &stmt->operands[stmt->operand_count++];
            if (!fuasm_parse_expr(parser, expr)) {
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

                fuasm_parser_advance(parser);
                fuasm_expr base_expr = {
                    .kind = FUASM_EXPR_REG,
                    .is_base = true,
                    .location = parser->tk.location,
                };

                fuasm_token tk;
                if (!fuasm_parser_expect(parser, FUASM_TK_REGISTER, "a register name", &tk)) {
                    return nullptr;
                }

                base_expr.as._register = tk.as._register;
                stmt->operands[stmt->operand_count++] = base_expr;

                if (!fuasm_parser_expect(parser, ')', nullptr, nullptr)) {
                    return nullptr;
                }
            }
        } while (fuasm_parser_try(parser, ','));
    }

stmt_end:;
    is_first = false;

    if (parser->tk.kind != FUASM_TK_ENDL) {
        diag_issue(DIAG_ERROR, source, parser->tk.location, "Extra tokens at the end of a statement.");
        return nullptr;
    }

    while (parser->tk.kind != FUASM_TK_ENDL) {
        fuasm_parser_advance(parser);
    }

    fu_assert(parser->tk.kind == FUASM_TK_ENDL);
    fuasm_parser_advance(parser);

    return stmt;
}

static char* fuasm_assemble_ir_internal(fuasm_state* state, gfu_uword_t* rom_size) {
    fuasm_stmt* const ir = state->ir;
    fuasm_section section = FUASM_TEXT;

    /* Step 0: Collect label count */

    for (fuasm_stmt* stmt = ir; stmt != nullptr; stmt = stmt->next) {
        if (stmt->label != nullptr) {
            state->label_count++;
        }
    }

    /* Step 1: Validate the assembler statements */

    state->addresses = calloc((size_t)state->label_count, sizeof *state->addresses);

    gfu_uword_t text_instruction_count = 0;

    section = FUASM_TEXT;
    for (fuasm_stmt* stmt = ir; stmt != nullptr; ) {
        switch (stmt->directive) {
            default: fu_assert(false && "Unhandled directive"); break;
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

        stmt->pattern_index = fuasm_match_isel(stmt);
        if (stmt->pattern_index == 0xFFFFFFFF) {
            diag_issue(DIAG_ERROR, stmt->source, stmt->location, "Invalid instruction format.");
            return nullptr;
        }

        isel_pattern pattern = isel_patterns[stmt->pattern_index];
        fu_assert(pattern.match_count > 0);

        if (section == FUASM_TEXT) {
            text_instruction_count += pattern.emit_count;
        }

        for (gfu_uword_t i = 0; i < pattern.match_count; i++) {
            if (section == FUASM_TEXT && stmt->mnemonic == FUASM_MNEM_BYTES) {
                diag_issue(DIAG_ERROR, stmt->source, stmt->location, "'bytes' statement can only occur inside the #data section.");
                return nullptr;
            } else if (section == FUASM_DATA && stmt->mnemonic != FUASM_MNEM_BYTES) {
                diag_issue(DIAG_ERROR, stmt->source, stmt->location, "Currently only 'bytes' statement may occur inside the #data section.");
                return nullptr;
            }

            fu_assert(stmt != nullptr);
            stmt = stmt->next;
        }
    }

    /* Step 2: Calculate label addresses */

    gfu_uword_t label_index = 0;
    gfu_uword_t text_index = 0;
    gfu_uword_t data_index = 0;
    gfu_uword_t label_scope = ISEL_LABEL_NOPARENT;

    section = FUASM_TEXT;
    for (fuasm_stmt* stmt = ir; stmt != nullptr; ) {
        switch (stmt->directive) {
            default: fu_assert(false && "Unhandled directive"); break;
            case FUASM_DIR_INVALID: break;
            case FUASM_DIR_ADDRESS_SPACE: break;
            case FUASM_DIR_ENTRY: break;
            case FUASM_DIR_TEXT: section = FUASM_TEXT; break;
            case FUASM_DIR_DATA: section = FUASM_DATA; break;
        }

        if (stmt->label != nullptr) {
            gfu_uword_t relative_address = section == FUASM_TEXT
                ? text_index : data_index + (text_instruction_count * 4);
            gfu_uword_t parent_label = ISEL_LABEL_NOPARENT;

            if (stmt->is_label_local) {
                parent_label = label_scope;
            } else {
                label_scope = relative_address;
            }

            gfu_uword_t addr_offset = state->address_space == GFUOBJ_ADDRSPACE_USER
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
        fuasm_expr vars[pattern.var_count];

        const fuasm_stmt* match_stmt = stmt;
        for (gfu_uword_t mi = 0, var_index = 0; mi < pattern.match_count; mi++, match_stmt = match_stmt->next) {
            isel_match match = isel_matches[pattern.match_index + mi];
            for (gfu_uword_t oi = 0; oi < match.operand_count; oi++) {
                isel_operand op = match.operands[oi];
                if (!op.is_constant) {
                    vars[var_index++] = stmt->operands[oi];
                }
            }
        }

        if (section == FUASM_TEXT) {
            text_index += pattern.emit_count * sizeof(gfu_uword_t);
        } else if (section == FUASM_DATA) {
            fu_assert(pattern.match_count == 1);
            fu_assert(pattern.emit_count == 1);
            fu_assert(stmt->mnemonic == FUASM_MNEM_BYTES);
            fu_assert(stmt->operand_count == 1);
            isel_emit emit = isel_emits[pattern.emit_index];
            fu_assert(emit.arguments[0].kind == ISEL_ARG_VAR);
            fuasm_expr expr = vars[emit.arguments[0].as.var_index];
            if (expr.kind == FUASM_EXPR_BYTE_STRING) {
                data_index += expr.as.byte_string.length;
            } else if (expr.kind == FUASM_EXPR_IMM) {
                data_index += expr.as.immediate;
            } else {
                diag_issue(DIAG_FATAL, NOSOURCE, "unhandled `bytes` case in data section.");
            }
        }

        for (gfu_uword_t i = 0; i < pattern.match_count; i++) {
            stmt = stmt->next;
        }
    }

    fu_assert(text_index == text_instruction_count * sizeof(gfu_uword_t));

    print_verbose(state, "Label count: %d", state->label_count);
    print_verbose(state, "Instruction byte count: %d", text_index);
    print_verbose(state, "Instruction word count: %d", text_instruction_count);
    print_verbose(state, "Data byte count: %d", data_index);

    /* Step 3: Generate the final ROM binary */

    gfu_uword_t start_label_address = fuasm_lookup_address_raw(state, ISEL_LABEL_NOPARENT, (struct source){0}, 0, state->entry_name);
    if (start_label_address == 0xFFFFFFFFu) {
        diag_issue(DIAG_ERROR, NOSOURCE, "No '_start' label found.");
        return nullptr;
    }

    gfuobj_header_t rom_header = {
        .magic = GFUOBJ_MAGIC,
        .address_space = state->address_space,
        .entry_address = start_label_address,
    };

    const gfu_uword_t align = sizeof(gfu_uword_t);
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
    for (fuasm_stmt* stmt = ir; stmt != nullptr; ) {
        switch (stmt->directive) {
            default: fu_assert(false && "Unhandled directive"); break;
            case FUASM_DIR_INVALID: break;
            case FUASM_DIR_ADDRESS_SPACE: break;
            case FUASM_DIR_ENTRY: break;
            case FUASM_DIR_TEXT: section = FUASM_TEXT; break;
            case FUASM_DIR_DATA: section = FUASM_DATA; break;
        }

        if (stmt->label != nullptr) {
            if (!stmt->is_label_local) {
                gfu_uword_t relative_address = section == FUASM_TEXT
                    ? text_index : data_index + text_index;
                label_scope = relative_address;
            }
        }

        if (stmt->mnemonic == FUASM_MNEM_INVALID) {
            stmt = stmt->next;
            continue;
        }

        isel_pattern pattern = isel_patterns[stmt->pattern_index];
        fuasm_expr vars[pattern.var_count];

        const fuasm_stmt* match_stmt = stmt;
        for (gfu_uword_t mi = 0, var_index = 0; mi < pattern.match_count; mi++, match_stmt = match_stmt->next) {
            isel_match match = isel_matches[pattern.match_index + mi];
            for (gfu_uword_t oi = 0; oi < match.operand_count; oi++) {
                isel_operand op = match.operands[oi];
                if (!op.is_constant) {
                    vars[var_index++] = stmt->operands[oi];
                }
            }
        }

        gfu_uword_t addr_offset = state->address_space == GFUOBJ_ADDRSPACE_USER
            ? GFU_KSEG0_BASE + GFU_BIOS_RAM_SIZE
            : GFU_KSEG1_BASE + GFU_BIOS_ROM_BASE;

        if (section == FUASM_TEXT) {
            gfu_uword_t icount = fuasm_emit_isel(state, label_scope, text_index + addr_offset, stmt, rom_base + text_index, vars);
            if (icount == 0) {
                free(rom_data);
                return nullptr;
            }
            text_index += icount;
        } else if (section == FUASM_DATA) {
            gfu_uword_t addr = (text_instruction_count * sizeof(gfu_uword_t)) + data_index;
            gfu_uword_t icount = fuasm_emit_isel(state, label_scope, addr + addr_offset, stmt, rom_base + addr, vars);
            if (icount == 0) {
                free(rom_data);
                return nullptr;
            }
            data_index += icount;
        }

        for (gfu_uword_t i = 0; i < pattern.match_count; i++) {
            stmt = stmt->next;
        }
    }

    return rom_data;
}

static char* fuasm_assemble_internal(fuasm_state* state, gfu_uword_t* rom_size) {
    fuasm_stmt* head = nullptr;
    fuasm_stmt* tail = nullptr;

    for (gfu_uword_t i = 0; i < state->options.source_count; i++) {
        source source = state->options.sources[i];

        fuasm_parser parser = {
            .state = state,
            .source = source,
            .tk.kind = FUASM_TK_INVALID,
            .next.kind = FUASM_TK_INVALID,
        };

        fuasm_userdata userdata = {
            .state = state,
            .source = source,
        };
        fuasm_lexer_init(&userdata, &parser.lexer);

        parser.tk = fuasm_lexer_read(&parser.lexer);
        while (parser.tk.kind != FUASM_TK_EOF) {
            while (parser.tk.kind == FUASM_TK_ENDL) {
                fuasm_parser_advance(&parser);
            }

            if (parser.tk.kind == FUASM_TK_EOF) break;

            gfu_uword_t location  = parser.tk.location;
            fuasm_stmt* stmt = parse_statement(&parser);
            if (stmt == nullptr) return nullptr;
            fu_assert(location != parser.tk.location);

            if (head == nullptr) {
                head = tail = stmt;
            } else {
                tail->next = stmt;
                tail = stmt;
            }
        }
    }

    state->ir = head;
    return fuasm_assemble_ir_internal(state, rom_size);
}

/* ===== Lexer Shit ===== */

static const char* fuasm_token_kind_names[] = {
    [FUASM_TK_INVALID] = "INVALID",
    [FUASM_TK_EOF] = "EOF",
#define X(Id) [FUASM_TK_##Id] = #Id,
    FUASM_TOKEN_KINDS(X)
#undef X
};

static void fuasm_token_dump(fuasm_token token) {
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
            fuasm_token_kind_names[token.kind],
            (int)(token.end - token.begin),
            token.begin
        );
    }

    fputc('\n', stderr);
}

static gfu_uword_t fuasm_lexer_location(etok_lexer* lexer) {
    return (gfu_uword_t)(lexer->source_current - lexer->source_begin);
}

static void fuasm_etok_error_callback(void* userdata, const char* source_name, const char* source_text, const char* where, uint64_t line, uint64_t column, const char* format, ...) {
    fuasm_userdata* data = userdata;

    va_list v;
    va_start(v, format);
    diag_issue_v(DIAG_ERROR, data->source, (uint32_t)(where - source_text), format, v);
    va_end(v);
}

static bool fuasm_etok_comment_consumer(etok_lexer* lexer) {
    if (lexer->ch != ';') return false;

    while (!etok_lexer_is_at_end(lexer) && lexer->ch != '\n') {
        etok_lexer_advance(lexer);
    }

    return true;
}

static bool fuasm_is_white_space(void* userdata, int ch) {
    return ch == ' ' || ch == '\f' || ch == '\r' || ch == '\t' || ch == '\v';
}

static bool fuasm_is_ident_begin(void* userdata, int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '.' || ch == '_';
}

static bool fuasm_is_ident_continue(void* userdata, int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '_';
}

static int fuasm_get_hex_digit_value(int ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return 10 + (ch - 'a');
    if (ch >= 'A' && ch <= 'F') return 10 + (ch - 'A');
    return -1;
}

static bool fuasm_lexer_is_hex_digit(int ch) {
    return fuasm_get_hex_digit_value(ch) >= 0;
}

static void fuasm_lexer_init(fuasm_userdata* userdata, etok_lexer* lexer) {
    etok_lexer_init(lexer, userdata->source.name, userdata->source.text, userdata->source.text + userdata->source.length);

    lexer->userdata = userdata;
    lexer->error_callback = fuasm_etok_error_callback;
    lexer->comment_consumer = fuasm_etok_comment_consumer;
    lexer->white_space_predicate = fuasm_is_white_space;
    lexer->ident_begin_predicate = fuasm_is_ident_begin;
    lexer->ident_continue_predicate = fuasm_is_ident_continue;

    etok_lexer_advance(lexer);
}

static fuasm_token fuasm_lexer_read(etok_lexer* lexer) {
    if (lexer == nullptr) return (fuasm_token) {0};

    fu_assert(lexer->userdata != nullptr);
    fuasm_userdata* userdata = lexer->userdata;

    fuasm_state* state = userdata->state;
    fu_assert(state != nullptr);

    source source = userdata->source;

    etok_lexer_skip_white_space(lexer);
    fuasm_token token = {
        .source = source,
        .kind = FUASM_TK_INVALID,
        .location = fuasm_lexer_location(lexer),
        .begin = lexer->source_current,
        .line = (gfu_uword_t)lexer->line,
        .column = (gfu_uword_t)lexer->column,
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
                fuasm_directive directive;
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
                fuasm_mnemonic mnemonic;
                const char* image;
            } mnemonics[] = {
#define MNEM(Id, Name) { FUASM_MNEM_##Id, Name },
#include <gamefu/mnemonics.h>
                {0},
            };

            static struct {
                gfu_gpr_t _register;
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
            token.as.label = fuasm_intern_string(state, name, (gfu_uword_t)length);
        } break;

        case '#': {
            etok_lexer_advance(lexer);

            token.kind = FUASM_TK_IMMEDIATE;
            if (!fuasm_lexer_is_hex_digit(lexer->ch)) {
                diag_issue(DIAG_ERROR, source, fuasm_lexer_location(lexer), "Expected a hex digit.");
                goto return_token;
            }

            while (!etok_lexer_is_at_end(lexer) && fuasm_lexer_is_hex_digit(lexer->ch)) {
                token.as.immediate = (16 * token.as.immediate) + fuasm_get_hex_digit_value(lexer->ch);
                etok_lexer_advance(lexer);
            }
        } break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            gfu_long_t immediate = lexer->ch - '0';
            etok_lexer_advance(lexer);

            while (!etok_lexer_is_at_end(lexer) && etok_lexer_is_decimal_digit(lexer, lexer->ch)) {
                immediate = (10 * immediate) + (lexer->ch - '0');
                etok_lexer_advance(lexer);
            }

            token.kind = FUASM_TK_IMMEDIATE;
            token.as.immediate = (gfu_uword_t)immediate;
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

            char* byte_string = gfu_arena_alloc(&state->string_arena, max_length);
            gfu_uword_t length = 0;

            while (!etok_lexer_is_at_end(lexer) && lexer->ch != '\n' && lexer->ch != '"') {
                if (lexer->ch == '\\') {
                    etok_lexer_advance(lexer);
                    int digits[2];
                    for (int i = 0; i < 2; i++) {
                        if (etok_lexer_is_at_end(lexer) || lexer->ch == '\n' || lexer->ch == '"' || (digits[i] = fuasm_get_hex_digit_value(lexer->ch), digits[i] < 0)) {
                            diag_issue(DIAG_ERROR, source, fuasm_lexer_location(lexer), "Expected hex digit in escape sequence.");
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
    fu_assert(token.kind != FUASM_TK_INVALID);
    token.end = lexer->source_current;
    return token;
}

#include "../lib/diagnostic.c"
#include "../lib/memory.c"
#include "../lib/source.c"
