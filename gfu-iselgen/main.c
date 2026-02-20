/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/common.h>

#include <gamefu/iselgen/data.h>

#include <gamefu/opcodes/instruction.h>
#include <gamefu/opcodes/register.h>
#include <gamefu/opcodes/syscall.h>

#if defined(__linux__)
#  include <unistd.h>
#endif

#define ECHOETOK_IMPL
#include <echoetok.h>

#include "isel_source.h"

#define ISEL_TOKEN_KINDS(X) \
    X(MATCH) \
    X(EMITS) \
    X(REG) \
    X(IMM) \
    X(COP0) \
    X(STR) \
    X(BYTES) \
    X(UPPER) \
    X(LOWER) \
    X(ADDR) \
    X(OFFS) \
    X(VAR) \
    X(OPCODE) \
    X(FUNCTION) \
    X(COP0_FUNCTION) \
    X(MNEMONIC) \
    X(REGISTER) \
    X(IMMEDIATE)

typedef enum isel_token_kind {
    ISEL_TK_INVALID = 0x00FFFFFF,
    ISEL_TK_EOF = 0,

    ISEL_TK_PRINTABLE_BEGIN = ETOK_PRINTABLE_BEGIN,
    ISEL_TK_PRINTABLE_END = ETOK_PRINTABLE_END,

    ISEL_TK_INVALID_CHARACTER = ETOK_INVALID_CHARACTER,

    _isel_tk_multibyte_offset = ETOK_MULTIBYTE_BEGIN - 1,
#define X(Id) ISEL_TK_##Id,
    ISEL_TOKEN_KINDS(X)
#undef X
} isel_token_kind;

typedef struct isel_token {
    isel_token_kind kind;
    gfu_uword location;
    const char *begin, *end;
    gfu_uword line, column;
    union {
        gfu_opcode opcode;
        gfu_opfn function;
        gfu_c0fn cop0_function;
        int mnemonic;
        gfu_gpr _register;
        gfu_uword immediate;
        const char* var;
    } as;
} isel_token;

typedef struct isel_parser {
    etok_lexer lexer;
    isel_token tk;
} isel_parser;

static void isel_lexer_init(etok_lexer* lexer);
static isel_token isel_lexer_read(etok_lexer* lexer);
static void isel_token_dump(isel_token token);

static void isel_parse_pattern(isel_parser* parser);

static gfu_source isel_source = {0};

static char isel_string_data[64] = {0};
gfu_uword isel_string_allocated = 0;

static isel_match isel_match_data[256] = {0};
gfu_uword isel_match_allocated = 0;

static isel_emit isel_emit_data[256] = {0};
gfu_uword isel_emit_allocated = 0;

static isel_pattern isel_pattern_data[256] = {0};
gfu_uword isel_pattern_allocated = 0;

static struct mnemonic {
    char mnemonic[16];
    char constant[16];
} defined_mnemonics[512] = {0};
static int mnemonic_count = 0;

static const char* isel_register_constants[] = {
#define GPR(Id, Image) [GFU_GPR_##Id] = "GFU_GPR_" #Id,
#include <gamefu/opcodes/x/registers.h>
};

static const char* isel_opcode_constants[] = {
#define X(Id, Value) [GFU_OP_##Id] = "GFU_OP_" #Id,
    GFU_OP(X)
#undef X
};

static const char* isel_function_constants[] = {
#define X(Id, Value) [GFU_OPFN_##Id] = "GFU_OPFN_" #Id,
    GFU_OPFN(X)
#undef X
};

static const char* isel_cop0_function_constants[] = {
#define X(Id, Value) [GFU_C0FN_##Id] = "GFU_C0FN_" #Id,
    GFU_C0FN(X)
#undef X
};

static const char* isel_type_constants[] = {
    [ISEL_TY_REG] = "ISEL_TY_REG",
    [ISEL_TY_IMM] = "ISEL_TY_IMM",
    [ISEL_TY_ADDR] = "ISEL_TY_ADDR",
    [ISEL_TY_STR] = "ISEL_TY_STR",
};

static const char* isel_argument_kind_constants[] = {
    [ISEL_ARG_REGISTER] = "ISEL_ARG_REGISTER",
    [ISEL_ARG_VAR] = "ISEL_ARG_VAR",
    [ISEL_ARG_VAR_UPPER] = "ISEL_ARG_VAR_UPPER",
    [ISEL_ARG_VAR_LOWER] = "ISEL_ARG_VAR_LOWER",
    [ISEL_ARG_VAR_ADDR] = "ISEL_ARG_VAR_ADDR",
    [ISEL_ARG_VAR_OFFS] = "ISEL_ARG_VAR_OFFS",
    [ISEL_ARG_VAR_NEGATE] = "ISEL_ARG_VAR_NEGATE",
    [ISEL_ARG_IMM] = "ISEL_ARG_IMM",
    [ISEL_ARG_IMM_UPPER] = "ISEL_ARG_IMM_UPPER",
    [ISEL_ARG_IMM_LOWER] = "ISEL_ARG_IMM_LOWER",
    [ISEL_ARG_IMM_ADDR] = "ISEL_ARG_IMM_ADDR",
    [ISEL_ARG_IMM_OFFS] = "ISEL_ARG_IMM_OFFS",
};

static const char* isel_emit_kind_constants[] = {
    [ISEL_EMIT_REG] = "ISEL_EMIT_REG",
    [ISEL_EMIT_IMM] = "ISEL_EMIT_IMM",
    [ISEL_EMIT_ADDR] = "ISEL_EMIT_ADDR",
    [ISEL_EMIT_COP0] = "ISEL_EMIT_COP0",
    [ISEL_EMIT_BYTES] = "ISEL_EMIT_BYTES",
};

int main(int argc, char** argv) {
    int result = 0;

    FILE* f = nullptr;

    gfu_diag_color_output(isatty(fileno(stderr)));

    isel_source = (gfu_source) {
        .name = "isel.txt",
        .text = (const char*)isel,
        .length = isel_len,
    };

    isel_parser parser = {0};
    isel_lexer_init(&parser.lexer);
    parser.tk = isel_lexer_read(&parser.lexer);

    while (parser.tk.kind != ISEL_TK_EOF) {
        isel_parse_pattern(&parser);
    }

    gfu_uword match_count = isel_match_allocated;
    isel_match* matches = (isel_match*)isel_match_data;

    gfu_uword emit_count = isel_emit_allocated;
    isel_emit* emits = (isel_emit*)isel_emit_data;

    gfu_uword pattern_count = isel_pattern_allocated;
    isel_pattern* patterns = (isel_pattern*)isel_pattern_data;

    fprintf(stderr, "Variable name storage required: %d bytes\n", isel_string_allocated);
    fprintf(stderr, "Match storage required: %d bytes (%d items)\n", isel_match_allocated, match_count);
    fprintf(stderr, "Emit storage required: %d bytes (%d items)\n", isel_emit_allocated, emit_count);
    fprintf(stderr, "Pattern storage required: %d bytes (%d items)\n", isel_pattern_allocated, pattern_count);

    f = fopen("./include/gamefu/as/x/mnemonics.h", "w");

    fprintf(f, "#ifndef MNEM\n");
    fprintf(f, "#  define MNEM(Id, ...)\n");
    fprintf(f, "#endif\n\n");
    for (int i = 0; i < mnemonic_count; i++) {
        fprintf(f, "MNEM(%s, \"%s\")\n", defined_mnemonics[i].constant, defined_mnemonics[i].mnemonic);
    }
    fprintf(f, "\n#undef MNEM\n");

    fclose(f);
    f = fopen("./gfu-as/isel_tables.c", "w");

    fprintf(f, "#include <gamefu/as.h>\n");
    fprintf(f, "#include <gamefu/iselgen/data.h>\n");
    fprintf(f, "#define ISEL_MATCH_COUNT %d\n", match_count);
    fprintf(f, "static isel_match isel_matches[%d] = {\n", match_count + 1);
    for (gfu_uword i = 0; i < match_count; i++) {
        isel_match match = matches[i];
        fprintf(f, "    [%u] = {\n", i);
        fprintf(f, "        .mnemonic = GFUAS_MNEM_%s,\n", defined_mnemonics[match.mnemonic].constant);
        if (match.operand_count > 0) {
            fprintf(f, "        .operand_count = %d,\n", match.operand_count);
            fprintf(f, "        .operands = {\n");
            for (gfu_uword j = 0; j < match.operand_count; j++) {
                isel_operand operand = match.operands[j];
                fprintf(f, "            {\n");
                fprintf(f, "                .type = %s,\n", isel_type_constants[operand.type]);
                fprintf(f, "                .is_constant = %s,\n", operand.is_constant ? "true" : "false");
                fprintf(f, "                .is_base = %s,\n", operand.is_base ? "true" : "false");
                if (operand.is_constant) {
                    fprintf(f, "                .as.immediate = %d,\n", operand.as.immediate);
                } else {
                    fprintf(f, "                .as.var = \"%s\",\n", operand.as.var);
                }
                fprintf(f, "            },\n");
            }
            fprintf(f, "        },\n");
        }
        fprintf(f, "    },\n");
    }
    fprintf(f, "    {0},\n");
    fprintf(f, "};\n\n");

    fprintf(f, "#define ISEL_EMIT_COUNT %d\n", emit_count);
    fprintf(f, "static isel_emit isel_emits[%d] = {\n", emit_count + 1);
    for (gfu_uword i = 0; i < emit_count; i++) {
        isel_emit emit = emits[i];
        fprintf(f, "    [%u] = {\n", i);
        fprintf(f, "        .kind = %s,\n", isel_emit_kind_constants[emit.kind]);
        if (emit.kind == ISEL_EMIT_REG) {
            fprintf(f, "        .op.function = %s,\n", isel_function_constants[emit.op.function]);
        } else if (emit.kind != ISEL_EMIT_BYTES) {
            fprintf(f, "        .op.opcode = %s,\n", isel_opcode_constants[emit.op.opcode]);
        }
        if (emit.argument_count > 0) {
            fprintf(f, "        .argument_count = %d,\n", emit.argument_count);
            fprintf(f, "        .arguments = {\n");
            for (gfu_uword j = 0; j < emit.argument_count; j++) {
                isel_argument argument = emit.arguments[j];
                fprintf(f, "            {\n");
                fprintf(f, "                .kind = %s,\n", isel_argument_kind_constants[argument.kind]);
                if (argument.kind == ISEL_ARG_REGISTER) {
                    fprintf(f, "                .as._register = %s,\n", isel_register_constants[argument.as._register]);
                } else if (argument.kind >= ISEL_ARG_VAR && argument.kind <= ISEL_ARG_VAR_OFFS) {
                    fprintf(f, "                .as.var_index = %d,\n", argument.as.var_index);
                } else if (argument.kind >= ISEL_ARG_IMM && argument.kind <= ISEL_ARG_IMM_OFFS) {
                    fprintf(f, "                .as.immediate = %"PRIi64",\n", (int64_t)(gfu_word)argument.as.immediate);
                }
                fprintf(f, "            },\n");
            }
            fprintf(f, "        },\n");
        }
        fprintf(f, "    },\n");
    }
    fprintf(f, "    {0},\n");
    fprintf(f, "};\n\n");

    fprintf(f, "#define ISEL_PATTERN_COUNT %d\n", pattern_count);
    fprintf(f, "static isel_pattern isel_patterns[%d] = {\n", pattern_count + 1);
    for (gfu_uword i = 0; i < pattern_count; i++) {
        isel_pattern pattern = patterns[i];
        fprintf(f, "    [%u] = {\n", i);
        fprintf(f, "        .var_count = %d,\n", pattern.var_count);
        fprintf(f, "        .match_index = %d,\n", pattern.match_index);
        fprintf(f, "        .match_count = %d,\n", pattern.match_count);
        fprintf(f, "        .emit_index = %d,\n", pattern.emit_index);
        fprintf(f, "        .emit_count = %d,\n", pattern.emit_count);
        fprintf(f, "    },\n");
    }
    fprintf(f, "    {0},\n");
    fprintf(f, "};\n");

    fprintf(stderr, "Wrote table output to './gfu-as/isel_tables.c'.\n");

defer:;
    if (f != nullptr) fclose(f);
    return result;
}

/* ===== Parser Shit ===== */

static bool isel_parser_is_at_end(isel_parser* parser) {
    return parser->tk.kind == ISEL_TK_EOF;
}

static void isel_parser_advance(isel_parser* parser) {
    if (isel_parser_is_at_end(parser)) return;
    parser->tk = isel_lexer_read(&parser->lexer);
}

static bool isel_parser_try(isel_parser* parser, isel_token_kind kind) {
    if (parser->tk.kind != kind) return false;
    isel_parser_advance(parser);
    return true;
}

static isel_token isel_parser_expect(isel_parser* parser, isel_token_kind kind, const char* what) {
    if (parser->tk.kind != kind) {
        isel_token_dump(parser->tk);
        if (kind >= ISEL_TK_PRINTABLE_BEGIN && kind <= ISEL_TK_PRINTABLE_END) {
            gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected '%c'.", kind);
        } else {
            gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected %s.", what);
        }
    }

    isel_token result = parser->tk;
    isel_parser_advance(parser);
    return result;
}

static isel_type isel_parser_expect_type(isel_parser* parser) {
    if (isel_parser_try(parser, ISEL_TK_REG)) return ISEL_TY_REG;
    if (isel_parser_try(parser, ISEL_TK_IMM)) return ISEL_TY_IMM;
    if (isel_parser_try(parser, ISEL_TK_ADDR)) return ISEL_TY_ADDR;
    if (isel_parser_try(parser, ISEL_TK_STR)) return ISEL_TY_STR;
    isel_token_dump(parser->tk);
    gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected a type.");
    return ISEL_TY_INVALID;
}

static isel_emit_kind isel_parser_expect_emit_kind(isel_parser* parser) {
    if (isel_parser_try(parser, ISEL_TK_REG)) return ISEL_EMIT_REG;
    if (isel_parser_try(parser, ISEL_TK_IMM)) return ISEL_EMIT_IMM;
    if (isel_parser_try(parser, ISEL_TK_ADDR)) return ISEL_EMIT_ADDR;
    if (isel_parser_try(parser, ISEL_TK_COP0)) return ISEL_EMIT_COP0;
    if (isel_parser_try(parser, ISEL_TK_BYTES)) return ISEL_EMIT_BYTES;
    isel_token_dump(parser->tk);
    gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected an instruction kind.");
    return ISEL_EMIT_INVALID;
}

static gfu_opcode isel_parser_expect_opcode(isel_parser* parser) {
    return isel_parser_expect(parser, ISEL_TK_OPCODE, "an opcode").as.opcode;
}

static gfu_opfn isel_parser_expect_function(isel_parser* parser) {
    return isel_parser_expect(parser, ISEL_TK_FUNCTION, "a function").as.function;
}

static gfu_c0fn isel_parser_expect_cop0_function(isel_parser* parser) {
    return isel_parser_expect(parser, ISEL_TK_COP0_FUNCTION, "a COP0 function").as.cop0_function;
}

static void isel_parse_match(isel_parser* parser) {
    isel_match* match = &isel_match_data[isel_match_allocated++];

    match->mnemonic = isel_parser_expect(parser, ISEL_TK_MNEMONIC, "a mnemonic").as.mnemonic;
    if (!isel_parser_is_at_end(parser) && parser->tk.kind != ISEL_TK_EMITS && parser->tk.kind != ISEL_TK_MNEMONIC) {
        do {
            if ((size_t)match->operand_count >= sizeof(match->operands) / sizeof(match->operands[0])) {
                gfu_diag_issue(DIAG_ERROR, isel_source, parser->tk.location, "Too many operands to match.");
            }

            isel_operand* operand = &match->operands[match->operand_count++];
            operand->type = isel_parser_expect_type(parser);
            if (parser->tk.kind == ISEL_TK_VAR) {
                operand->as.var = parser->tk.as.var;
                isel_parser_advance(parser);
            } else if (parser->tk.kind == ISEL_TK_IMM) {
                operand->is_constant = true;
                operand->as.immediate = parser->tk.as.immediate;
                isel_parser_advance(parser);
            } else {
                gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected a variable name or a constant.");
            }

            if (parser->tk.kind == '(') {
                if ((size_t)match->operand_count >= sizeof(match->operands) / sizeof(match->operands[0])) {
                    gfu_diag_issue(DIAG_ERROR, isel_source, parser->tk.location, "Too many operands to match.");
                }

                isel_parser_advance(parser);
                isel_parser_expect(parser, ISEL_TK_REG, "'reg'");

                isel_operand* base_operand = &match->operands[match->operand_count++];
                base_operand->is_base = true;
                base_operand->type = ISEL_TY_REG;
                base_operand->as.var = isel_parser_expect(parser, ISEL_TK_VAR, "a variable name").as.var;

                isel_parser_expect(parser, ')', nullptr);
            }
        } while (isel_parser_try(parser, ','));
    }
}

static gfu_uword isel_lookup_var(gfu_uword location, const char* var, const char** vars, gfu_uword var_count) {
    for (gfu_uword i = 0; i < var_count; i++) {
        if (0 == strcmp(var, vars[i])) {
            return i;
        }
    }

    gfu_diag_issue(DIAG_FATAL, isel_source, location, "Use of undeclared variable '%s'.", var);
    return -1;
}

static void isel_parse_emit_argument(isel_parser* parser, isel_argument* argument, const char** vars, gfu_uword var_count) {
    if (parser->tk.kind == ISEL_TK_REGISTER) {
        argument->kind = ISEL_ARG_REGISTER;
        argument->as._register = parser->tk.as._register;
        isel_parser_advance(parser);
    } else if (parser->tk.kind == ISEL_TK_VAR) {
        argument->kind = ISEL_ARG_VAR;
        argument->as.var_index = isel_lookup_var(parser->tk.location, parser->tk.as.var, vars, var_count);
        isel_parser_advance(parser);
    } else if (parser->tk.kind == ISEL_TK_IMMEDIATE) {
        argument->kind = ISEL_ARG_IMM;
        argument->as.immediate = parser->tk.as.immediate;
        isel_parser_advance(parser);
    } else if (parser->tk.kind == '-') {
        isel_parser_advance(parser);
        if (parser->tk.kind == ISEL_TK_IMMEDIATE) {
            argument->kind = ISEL_ARG_IMM;
            argument->as.immediate = (gfu_uword)(-(gfu_word)(parser->tk.as.immediate));
            isel_parser_advance(parser);
        } else {
            argument->kind = ISEL_ARG_VAR_NEGATE;
            isel_token vartk = isel_parser_expect(parser, ISEL_TK_VAR, "a variable name");
            argument->as.var_index = isel_lookup_var(vartk.location, vartk.as.var, vars, var_count);
        }
    } else if (parser->tk.kind == ISEL_TK_UPPER || parser->tk.kind == ISEL_TK_LOWER || parser->tk.kind == ISEL_TK_ADDR || parser->tk.kind == ISEL_TK_OFFS) {
        int offs = parser->tk.kind - ISEL_TK_UPPER;
        isel_parser_advance(parser);
        if (parser->tk.kind == ISEL_TK_VAR) {
            argument->kind = ISEL_ARG_VAR_UPPER + offs;
            argument->as.var_index = isel_lookup_var(parser->tk.location, parser->tk.as.var, vars, var_count);
            isel_parser_advance(parser);
        } else if (parser->tk.kind == ISEL_TK_IMMEDIATE) {
            argument->kind = ISEL_ARG_IMM_UPPER + offs;
            argument->as.immediate = parser->tk.as.immediate;
            isel_parser_advance(parser);
        } else {
            isel_token_dump(parser->tk);
            gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected a variable or an immediate.");
        }
    } else {
        isel_token_dump(parser->tk);
        gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected an argument.");
    }
}

static void isel_parse_emit(isel_parser* parser, const char** vars, gfu_uword var_count) {
    isel_emit* emit = &isel_emit_data[isel_emit_allocated++];

    emit->kind = isel_parser_expect_emit_kind(parser);
    (void)isel_parser_expect(parser, '(', nullptr);
    if (emit->kind != ISEL_EMIT_BYTES) {
        if (emit->kind == ISEL_EMIT_REG) {
            emit->op.function = isel_parser_expect_function(parser);
        } else if (emit->kind == ISEL_EMIT_COP0) {
            emit->op.cop0_function = isel_parser_expect_cop0_function(parser);
        } else {
            emit->op.opcode = isel_parser_expect_opcode(parser);
        }

        while (isel_parser_try(parser, ',')) {
            if (emit->argument_count == sizeof(emit->arguments) / sizeof(emit->arguments[0])) {
                gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Too many arguments.");
            }

            isel_argument* argument = &emit->arguments[emit->argument_count++];
            isel_parse_emit_argument(parser, argument, vars, var_count);
        }
    } else {
        do {
            isel_argument* argument = &emit->arguments[emit->argument_count++];
            isel_parse_emit_argument(parser, argument, vars, var_count);
        } while (isel_parser_try(parser, ','));
    }
    (void)isel_parser_expect(parser, ')', nullptr);
}

static void isel_parse_pattern(isel_parser* parser) {
    gfu_uword location = parser->tk.location;

    isel_pattern* pattern = &isel_pattern_data[isel_pattern_allocated++];
    isel_match* matches = &isel_match_data[isel_match_allocated];

    isel_parser_expect(parser, ISEL_TK_MATCH, "'match'");

    pattern->match_index = isel_match_allocated;
    while (!isel_parser_is_at_end(parser) && parser->tk.kind != ISEL_TK_EMITS) {
        isel_parse_match(parser);
        isel_match* match = &matches[pattern->match_count++];
        for (gfu_uword j = 0; j < match->operand_count; j++) {
            if (!match->operands[j].is_constant) {
                pattern->var_count++;
            }
        }
    }

    if (pattern->match_count == 0) {
        gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected a match clause.");
    }

    const char* vars[pattern->var_count <= 0 ? 1 : pattern->var_count];
    for (gfu_uword i = 0, var_index = 0; i < pattern->match_count; i++) {
        isel_match* match = &matches[i];
        for (gfu_uword j = 0; j < match->operand_count; j++) {
            if (!match->operands[j].is_constant) {
                for (gfu_uword k = 0; k < var_index; k++) {
                    if (0 == strcmp(match->operands[j].as.var, vars[k])) {
                        gfu_diag_issue(DIAG_FATAL, isel_source, location, "Match claus(es) contain duplicate variable names.");
                    }
                }
                vars[var_index++] = match->operands[j].as.var;
            }
        }
    }

    isel_parser_expect(parser, ISEL_TK_EMITS, "'emit'");

    pattern->emit_index = isel_emit_allocated;
    while (!isel_parser_is_at_end(parser) && parser->tk.kind != ISEL_TK_MATCH) {
        isel_parse_emit(parser, vars, pattern->var_count);
    }

    pattern->emit_count = isel_emit_allocated - pattern->emit_index;
    if (pattern->emit_count == 0) {
        gfu_diag_issue(DIAG_FATAL, isel_source, parser->tk.location, "Expected an emit clause.");
    }
}

/* ===== Lexer Shit ===== */

static const char* isel_token_kind_names[] = {
    [ISEL_TK_EOF] = "EOF",
#define X(Id) [ISEL_TK_##Id] = #Id,
    ISEL_TOKEN_KINDS(X)
#undef X
};

static void isel_token_dump(isel_token token) {
    if (token.kind >= ISEL_TK_PRINTABLE_BEGIN && token.kind <= ISEL_TK_PRINTABLE_END) {
        fprintf(
            stderr,
            "%s(%d,%d):  %c",
            isel_source.name,
            token.line,
            token.column,
            token.kind
        );
    } else {
        fprintf(
            stderr,
            "%s(%d,%d): [%s]  %.*s",
            isel_source.name,
            token.line,
            token.column,
            isel_token_kind_names[token.kind],
            (int)(token.end - token.begin),
            token.begin
        );

        if (token.kind == ISEL_TK_VAR) {
            fprintf(stderr, "  '%s'", token.as.var);
        }
    }

    fputc('\n', stderr);
}

static void isel_etok_error_callback(void* userdata, const char* source_name, const char* source_text, const char* where, uint64_t line, uint64_t column, const char* format, ...) {
    va_list v;
    va_start(v, format);
    gfu_diag_issue_v(DIAG_ERROR, isel_source, (uint32_t)(where - source_text), format, v);
    va_end(v);
}

static bool isel_etok_comment_consumer(etok_lexer* lexer) {
    if (lexer->ch != ';') return false;

    while (!etok_lexer_is_at_end(lexer) && lexer->ch != '\n') {
        etok_lexer_advance(lexer);
    }

    return true;
}

static void isel_lexer_init(etok_lexer* lexer) {
    etok_lexer_init(lexer, "gfu-iselgen/isel.txt", isel_source.text, isel_source.text + isel_source.length);
    lexer->error_callback = isel_etok_error_callback;
    lexer->comment_consumer = isel_etok_comment_consumer;
}

static gfu_uword isel_lexer_location(etok_lexer* lexer) {
    return (gfu_uword)(lexer->source_current - lexer->source_begin);
}

static struct {
    isel_token_kind keyword;
    const char* image;
} isel_keywords[] = {
    {ISEL_TK_MATCH, "match"},
    {ISEL_TK_EMITS, "emits"},
    {ISEL_TK_REG, "reg"},
    {ISEL_TK_IMM, "imm"},
    {ISEL_TK_COP0, "cop0"},
    {ISEL_TK_STR, "str"},
    {ISEL_TK_BYTES, "bytes"},
    {ISEL_TK_UPPER, "upper"},
    {ISEL_TK_LOWER, "lower"},
    {ISEL_TK_ADDR, "addr"},
    {ISEL_TK_OFFS, "offs"},
    {0},
};

static struct {
    gfu_opcode opcode;
    const char* image;
} isel_opcodes[] = {
#define X(Id, Value) { GFU_OP_##Id, #Id },
    GFU_OP(X)
#undef X
    {0},
};

static struct {
    gfu_opfn function;
    const char* image;
} isel_functions[] = {
#define X(Id, Value) { (int)GFU_OPFN_##Id, #Id },
    GFU_OPFN(X)
#undef X
    {0},
};

static struct {
    gfu_c0fn cop0_function;
    const char* image;
} isel_cop0_functions[] = {
#define X(Id, Value) { (int)GFU_C0FN_##Id, #Id },
    GFU_C0FN(X)
#undef X
    {0},
};

static struct {
    gfu_gpr _register;
    const char* image;
} isel_registers[] = {
#define GPR(Id, Image) { GFU_GPR_##Id, "" Image "" },
#include <gamefu/opcodes/x/registers.h>
    {0},
};

static isel_token isel_lexer_read(etok_lexer* lexer) {
    if (lexer == NULL) return (isel_token) {0};

    /* Prime the first character. */
    if (lexer->ch == 0 && lexer->stride == 0 && lexer->source_current == lexer->source_begin) {
        etok_lexer_advance(lexer);
    }

    etok_lexer_skip_white_space(lexer);
    isel_token token = {
        .kind = ISEL_TK_INVALID,
        .location = isel_lexer_location(lexer),
        .begin = lexer->source_current,
        .line = (gfu_uword)lexer->line,
        .column = (gfu_uword)lexer->column,
    };

    if (etok_lexer_is_at_end(lexer)) {
        token.kind = ISEL_TK_EOF;
        token.end = token.begin;
        return token;
    }

    int ch = lexer->ch;
    switch (ch) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            int64_t immediate = lexer->ch - '0';
            etok_lexer_advance(lexer);
            while (!etok_lexer_is_at_end(lexer) && lexer->ch >= '0' && lexer->ch <= '9') {
                immediate = (10 * immediate) + (lexer->ch - '0');
                etok_lexer_advance(lexer);
            }

            token.kind = ISEL_TK_IMMEDIATE;
            token.as.immediate = (gfu_uword)immediate;
        } break;

        case '%': {
            etok_lexer_advance(lexer);

            const char* name = lexer->source_current;
            while (!etok_lexer_is_at_end(lexer) && ((lexer->ch >= 'a' && lexer->ch <= 'z') || (lexer->ch >= '0' && lexer->ch <= '9'))) {
                etok_lexer_advance(lexer);
            }

            size_t length = (size_t)(lexer->source_current - name);
            if (length == 0) {
                gfu_diag_issue(DIAG_ERROR, isel_source, token.location, "Expected a lower-case letter to start a variable name.");
            } else if (length > 7) {
                gfu_diag_issue(DIAG_ERROR, isel_source, token.location, "Mnemonic names must be 7 characters or fewer.");
            }

            token.kind = ISEL_TK_MNEMONIC;

            int mnemonic_index = -1;
            for (int i = 0; i < mnemonic_count; i++) {
                size_t mnemlength = strlen(defined_mnemonics[i].mnemonic);
                if (length != mnemlength) continue;
                if (0 == strncmp(defined_mnemonics[i].mnemonic, name, length)) {
                    mnemonic_index = i;
                    break;
                }
            }

            if (mnemonic_index < 0) {
                mnemonic_index = mnemonic_count++;
                struct mnemonic* m = &defined_mnemonics[mnemonic_index];
                strncpy(m->mnemonic, name, length);
                strncpy(m->constant, name, length);
                for (size_t i = 0; i < length; i++) {
                    m->constant[i] = toupper(m->constant[i]);
                }
            }

            token.as.mnemonic = mnemonic_index;
        } break;

        case '$': {
            etok_lexer_advance(lexer);

            const char* name = lexer->source_current;
            while (!etok_lexer_is_at_end(lexer) && lexer->ch >= 'a' && lexer->ch <= 'z') {
                etok_lexer_advance(lexer);
            }

            size_t length = (size_t)(lexer->source_current - name);
            if (length == 0) {
                gfu_diag_issue(DIAG_ERROR, isel_source, token.location, "Expected a lower-case letter to start a variable name.");
            }

            token.kind = ISEL_TK_VAR;
            char* varname = nullptr;

            for (char* strings = isel_string_data; strings < isel_string_data + isel_string_allocated; ) {
                size_t existing_length = strlen(strings);
                if (existing_length == length && 0 == strncmp(strings, name, length)) {
                    varname = strings;
                    break;
                }

                strings += existing_length + 1;
            }

            if (varname == nullptr) {
                varname = &isel_string_data[isel_string_allocated];
                isel_string_allocated += (gfu_uword)length + 1;
                memcpy(varname, name, length);
                varname[length] = 0;
            }

            token.as.var = varname;
        } break;

        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z': {
            const char* name = token.begin;
            while (!etok_lexer_is_at_end(lexer) && lexer->ch >= 'A' && lexer->ch <= 'Z') {
                etok_lexer_advance(lexer);
            }

            size_t length = (size_t)(lexer->source_current - name);

            int function = -1;
            for (int i = 0; isel_functions[i].image != nullptr; i++) {
                if (length == strlen(isel_functions[i].image) && 0 == strncmp(name, isel_functions[i].image, length)) {
                    function = isel_functions[i].function;
                    break;
                }
            }

            if (function != -1) {
                token.kind = ISEL_TK_FUNCTION;
                token.as.function = function;
                break;
            }

            int cop0_function = -1;
            for (int i = 0; isel_cop0_functions[i].image != nullptr; i++) {
                if (length == strlen(isel_cop0_functions[i].image) && 0 == strncmp(name, isel_cop0_functions[i].image, length)) {
                    cop0_function = isel_cop0_functions[i].cop0_function;
                    break;
                }
            }

            if (cop0_function != -1) {
                token.kind = ISEL_TK_COP0_FUNCTION;
                token.as.function = cop0_function;
                break;
            }

            for (int i = 0; isel_opcodes[i].image != nullptr; i++) {
                if (length == strlen(isel_opcodes[i].image) && 0 == strncmp(name, isel_opcodes[i].image, length)) {
                    token.as.opcode = isel_opcodes[i].opcode;
                    break;
                }
            }

            if (token.as.opcode != GFU_OP_SPECIAL) {
                token.kind = ISEL_TK_OPCODE;
                break;
            }

            gfu_diag_issue(DIAG_ERROR, isel_source, token.location, "Invalid ISEL opcode word.");
        } break;

        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z': {
            const char* name = token.begin;
            while (!etok_lexer_is_at_end(lexer) && ((lexer->ch >= 'a' && lexer->ch <= 'z') || (lexer->ch >= '0' && lexer->ch <= '9'))) {
                etok_lexer_advance(lexer);
            }

            size_t length = (size_t)(lexer->source_current - name);

            isel_token_kind keyword = ISEL_TK_EOF;
            for (int i = 0; isel_keywords[i].image != nullptr; i++) {
                if (length == strlen(isel_keywords[i].image) && 0 == strncmp(name, isel_keywords[i].image, length)) {
                    keyword = isel_keywords[i].keyword;
                    break;
                }
            }

            if (keyword != ISEL_TK_EOF) {
                token.kind = keyword;
                break;
            }

            token.as._register = 255;
            for (int i = 0; isel_registers[i].image != nullptr; i++) {
                if (length == strlen(isel_registers[i].image) && 0 == strncmp(name, isel_registers[i].image, length)) {
                    token.as._register = isel_registers[i]._register;
                    break;
                }
            }

            if (token.as._register != 255)  {
                token.kind = ISEL_TK_REGISTER;
                break;
            }

            gfu_diag_issue(DIAG_ERROR, isel_source, token.location, "Invalid ISEL reserved word. Did you mean '$%.*s'?", (int)length, name);
        } break;

        default: {
            if (ch >= ISEL_TK_PRINTABLE_BEGIN && ch <= ISEL_TK_PRINTABLE_END) {
                token.kind = ch;
                etok_lexer_advance(lexer);
            } else {
                /* Somewhat catch-all for generic character errors. */
                if (ch < 32) {
                    etok_lexer_issue_errorf(lexer, token.begin, token.line, token.column, "Invalid or unexpected character in source text (0x%02X).", ch);
                } else if (ch < 128) {
                    etok_lexer_issue_errorf(lexer, token.begin, token.line, token.column, "Invalid or unexpected character in source text '%c'.", (char)ch);
                } else {
                    etok_lexer_issue_errorf(lexer, token.begin, token.line, token.column, "Invalid or unexpected character in source text (0x%08X).", ch);
                }

                token.kind = ISEL_TK_INVALID_CHARACTER;
                etok_lexer_advance(lexer);
            }
        } break;
    }

    gfu_assert(token.kind != ISEL_TK_INVALID, "Did not properly populate token info");
    token.end = lexer->source_current;
    return token;
}

#include "../gfu-common/diagnostic.c"
#include "../gfu-common/source.c"
