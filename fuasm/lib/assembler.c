#include <gamefu/arch.h>
#include <gamefu/elf.h>
#include <gamefu/fuasm.h>
#include "lexer_internal.h"

typedef struct fuasm_instructions {
    KOS_DYNAMIC_ARRAY_FIELDS(u32);
} fuasm_instructions;

typedef struct fuasm_section_info {
    kos_string_view name;
    i64 size;
    fuasm_instructions instructions;
} fuasm_section_info;

typedef struct fuasm_section_infos {
    KOS_DYNAMIC_ARRAY_FIELDS(fuasm_section_info);
} fuasm_section_infos;

typedef struct fuasm_symbol_addr {
    kos_string_view name;
    i64 addr;

    ssize_t section : 8;
    bool is_global : 1;
    bool is_extern : 1;
} fuasm_symbol_addr;

typedef struct fuasm_symbol_addrs {
    KOS_DYNAMIC_ARRAY_FIELDS(fuasm_symbol_addr);
} fuasm_symbol_addrs;

typedef enum fuasm_operand_kind {
    FUASM_OPERAND_REG,
    FUASM_OPERAND_IMM,
} fuasm_operand_kind;

typedef struct fuasm_operand {
    fuasm_operand_kind kind;
    ssize_t begin, end;
    union {
        gfu_register reg;
        u32 imm;
    };
} fuasm_operand;

typedef struct fuasm_statement {
    fuasm_token label;
    fuasm_token mnemonic;
    ssize_t addr;
    fuasm_operand operands[3];
} fuasm_statement;

typedef struct fuasm_assembler {
    fuasm_translation_unit* unit;

    fuasm_tokens tokens;
    ssize_t token_position;

    fuasm_section_infos sections;
    fuasm_symbol_addrs symbols;

    ssize_t current_section_index;
} fuasm_assembler;

static int mnemonic_instruction_count(fuasm_token_kind kind);

static bool is_at_end(fuasm_assembler* asm);
static void advance(fuasm_assembler* asm);
static fuasm_token current(fuasm_assembler* asm);
static fuasm_section_info* current_section(fuasm_assembler* asm);
static bool is_at(fuasm_assembler* asm, fuasm_token_kind kind);
static bool is_at_mnemonic(fuasm_assembler* asm);
static bool is_at_register(fuasm_assembler* asm);
static fuasm_token expect(fuasm_assembler* asm, fuasm_token_kind kind, const char* desc);
static void expect_comma(fuasm_assembler* asm);
static gfu_register expect_register(fuasm_assembler* asm);
static fuasm_token_kind expect_label(fuasm_assembler* asm, i64* out_label);

static void precalculate_symbol_addresses(fuasm_assembler* asm);
static void read_statement(fuasm_assembler* asm);

void fuasm_assemble(fuasm_translation_unit* unit) {
    fuasm_assembler asm = {
        .unit = unit,
    };

    fuasm_lexer lexer = {
        .context = unit->context,
        .source = unit->source,
    };

    fuasm_token token = fuasm_read_token(&lexer);
    while (token.kind != FUASM_TK_END_OF_FILE) {
        kos_da_push(&asm.tokens, token);
        token = fuasm_read_token(&lexer);
    }

    precalculate_symbol_addresses(&asm);
    asm.token_position = 0; // reset token count for second full parse

    /*
    for (ssize_t i = 0; i < asm.sections.count; i++) {
        fprintf(stderr, "Section '"KOS_STR_FMT"' is %zd bytes long\n", KOS_STR_ARG(asm.sections.data[i].name), asm.sections.data[i].size);
    }
    */

    while (!is_at_end(&asm)) {
        ssize_t start_token_position = asm.token_position;
        read_statement(&asm);

        if (asm.token_position == start_token_position) {
            choir_diag_issue_source_bytes(unit->context, CHOIR_FATAL, unit->source, current(&asm).begin, "Failed to consume a token.");
            exit(1);
        }
    }

    for (ssize_t i = 0; i < asm.sections.count; i++) {
        fprintf(stderr, "Section '"KOS_STR_FMT"'\n", KOS_STR_ARG(asm.sections.data[i].name));
        kos_hexdump(kos_cast(const char*) asm.sections.data[i].instructions.data, 4 * asm.sections.data[i].instructions.count);
    }
}

static int mnemonic_instruction_count(fuasm_token_kind kind) {
    switch (kind) {
        default: return 1;
#define TK(Id) case FUASM_TK_##Id: return 0;
#define TK_MNEMONIC(Id, ...)
#include <gamefu/fuasm/tokens.h>
        // TODO(local): Identify mnemonics which require more than 1 instruction.
    }
}

static bool is_at_end(fuasm_assembler* asm) {
    return asm->token_position >= asm->tokens.count;
}

static void advance(fuasm_assembler* asm) {
    if (is_at_end(asm)) return;
    asm->token_position++;
}

static fuasm_token current(fuasm_assembler* asm) {
    choir_assert(asm->unit->context, !is_at_end(asm), "you fucked up");
    return asm->tokens.data[asm->token_position];
}

static fuasm_section_info* current_section(fuasm_assembler* asm) {
    choir_assert(asm->unit->context, asm->current_section_index >= 0 && asm->current_section_index < asm->sections.count, "you fucked up");
    return &asm->sections.data[asm->current_section_index];
}

static bool is_at(fuasm_assembler* asm, fuasm_token_kind kind) {
    if (is_at_end(asm)) return false;
    return current(asm).kind == kind;
}

static bool is_at_mnemonic(fuasm_assembler* asm) {
    if (is_at_end(asm)) return false;
    switch (current(asm).kind) {
        default: return false;
#define TK_MNEMONIC(Id, ...) case FUASM_TK_MN_##Id: return true;
#include <gamefu/fuasm/tokens.h>
    }
}

static bool is_at_register(fuasm_assembler* asm) {
    if (is_at_end(asm)) return false;
    switch (current(asm).kind) {
        default: return false;
#define TK_REGISTER(Id, ...) case FUASM_TK_REG_##Id: return true;
#include <gamefu/fuasm/tokens.h>
    }
}

static fuasm_token expect(fuasm_assembler* asm, fuasm_token_kind kind, const char* desc) {
    if (is_at_end(asm)) {
        ssize_t source_length;
        kos_discard choir_source_text_get(asm->unit->source, &source_length);

        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, source_length, "Expected %s.", desc);
        exit(1);
    }

    fuasm_token token = current(asm);
    if (token.kind == kind) {
        advance(asm);
        return token;
    }

    choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, token.begin, "Expected %s.", desc);
    exit(1);
}

static void expect_comma(fuasm_assembler* asm) {
    kos_discard expect(asm, FUASM_TK_COMMA, "','");
}

static gfu_register expect_register(fuasm_assembler* asm) {
    if (is_at_end(asm)) {
        ssize_t source_length;
        kos_discard choir_source_text_get(asm->unit->source, &source_length);

        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, source_length, "Expected a register.");
        exit(1);
    }

    if (!is_at_register(asm)) {
        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, current(asm).begin, "Expected a register.");
        exit(1);
    }

    fuasm_token token = current(asm);
    advance(asm);

    switch (token.kind) {
        default: {
            choir_diag_issue_source_bytes(asm->unit->context, CHOIR_FATAL, asm->unit->source, token.begin, "Invalid register token kind %s.", fuasm_token_kind_name_get(token.kind));
            return -1;
        }

        case FUASM_TK_REG_R0:  case FUASM_TK_REG_ZERO: return GFU_REG_R0;
        case FUASM_TK_REG_R1:  case FUASM_TK_REG_AT:   return GFU_REG_R1;
        case FUASM_TK_REG_R2:  case FUASM_TK_REG_V0:   return GFU_REG_R2;
        case FUASM_TK_REG_R3:  case FUASM_TK_REG_V1:   return GFU_REG_R3;
        case FUASM_TK_REG_R4:  case FUASM_TK_REG_A0:   return GFU_REG_R4;
        case FUASM_TK_REG_R5:  case FUASM_TK_REG_A1:   return GFU_REG_R5;
        case FUASM_TK_REG_R6:  case FUASM_TK_REG_A2:   return GFU_REG_R6;
        case FUASM_TK_REG_R7:  case FUASM_TK_REG_A3:   return GFU_REG_R7;
        case FUASM_TK_REG_R8:  case FUASM_TK_REG_T0:   return GFU_REG_R8;
        case FUASM_TK_REG_R9:  case FUASM_TK_REG_T1:   return GFU_REG_R9;
        case FUASM_TK_REG_R10: case FUASM_TK_REG_T2:   return GFU_REG_R10;
        case FUASM_TK_REG_R11: case FUASM_TK_REG_T3:   return GFU_REG_R11;
        case FUASM_TK_REG_R12: case FUASM_TK_REG_T4:   return GFU_REG_R12;
        case FUASM_TK_REG_R13: case FUASM_TK_REG_T5:   return GFU_REG_R13;
        case FUASM_TK_REG_R14: case FUASM_TK_REG_T6:   return GFU_REG_R14;
        case FUASM_TK_REG_R15: case FUASM_TK_REG_T7:   return GFU_REG_R15;
        case FUASM_TK_REG_R16: case FUASM_TK_REG_S0:   return GFU_REG_R16;
        case FUASM_TK_REG_R17: case FUASM_TK_REG_S1:   return GFU_REG_R17;
        case FUASM_TK_REG_R18: case FUASM_TK_REG_S2:   return GFU_REG_R18;
        case FUASM_TK_REG_R19: case FUASM_TK_REG_S3:   return GFU_REG_R19;
        case FUASM_TK_REG_R20: case FUASM_TK_REG_S4:   return GFU_REG_R20;
        case FUASM_TK_REG_R21: case FUASM_TK_REG_S5:   return GFU_REG_R21;
        case FUASM_TK_REG_R22: case FUASM_TK_REG_S6:   return GFU_REG_R22;
        case FUASM_TK_REG_R23: case FUASM_TK_REG_S7:   return GFU_REG_R23;
        case FUASM_TK_REG_R24: case FUASM_TK_REG_T8:   return GFU_REG_R24;
        case FUASM_TK_REG_R25: case FUASM_TK_REG_T9:   return GFU_REG_R25;
        case FUASM_TK_REG_R26: case FUASM_TK_REG_K0:   return GFU_REG_R26;
        case FUASM_TK_REG_R27: case FUASM_TK_REG_K1:   return GFU_REG_R27;
        case FUASM_TK_REG_R28: case FUASM_TK_REG_GP:   return GFU_REG_R28;
        case FUASM_TK_REG_R29: case FUASM_TK_REG_SP:   return GFU_REG_R29;
        case FUASM_TK_REG_R30: case FUASM_TK_REG_FP:   return GFU_REG_R30;
        case FUASM_TK_REG_R31: case FUASM_TK_REG_RA:   return GFU_REG_R31;
    }
}

static fuasm_token_kind expect_label(fuasm_assembler* asm, i64* out_label) {
    if (is_at_end(asm)) {
        ssize_t source_length;
        kos_discard choir_source_text_get(asm->unit->source, &source_length);

        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, source_length, "Expected a label.");
        exit(1);
    }

    if (!is_at(asm, FUASM_TK_GLOBAL_LABEL) && !is_at(asm, FUASM_TK_LOCAL_LABEL)) {
        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, current(asm).begin, "Expected a label.");
        exit(1);
    }

    fuasm_token token = current(asm);
    advance(asm);

    bool found = false;
    for (ssize_t i = 0; !found && i < asm->symbols.count; i++) {
        if (kos_sv_equals(asm->symbols.data[i].name, token.text_value)) {
            found = true;
            if (asm->symbols.data[i].is_global) {
                if (out_label) *out_label = kos_cast(i64) i;
            } else {
                if (out_label) *out_label = kos_cast(i64) asm->symbols.data[i].addr;
            }
        }
    }

    if (!found) {
        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, current(asm).begin, "Label '"KOS_STR_FMT"' is not defined.", KOS_STR_ARG(token.text_value));
        exit(1);
    }

    return token.kind;
}

static void precalculate_symbol_addresses(fuasm_assembler* asm) {
    ssize_t current_section_index = -1;
    while (!is_at_end(asm)) {
        bool is_at_label_def = is_at(asm, FUASM_TK_GLOBAL_LABEL) || is_at(asm, FUASM_TK_LOCAL_LABEL);
        if (is_at_label_def && asm->sections.count == 0) {
            current_section_index = 0;
            kos_da_push(&asm->sections, ((fuasm_section_info){ .name = KOS_SV_CONST("text") }));
        }

        fuasm_section_info* section = &asm->sections.data[current_section_index];

        if (is_at_label_def) {
            kos_string_view label_name = current(asm).text_value;
            fuasm_symbol_addr symbol = {
                .section = current_section_index,
                .name = label_name,
                .addr = section->size,
                .is_global = current(asm).kind == FUASM_TK_GLOBAL_LABEL,
            };

            // TODO(local): Check if a symbol has been defined before.
            // TODO(local): For local symbols, only check for a redefinition in the current "scope" (since the previous global symbol).

            kos_da_push(&asm->symbols, symbol);
            advance(asm); // the label def
        }

        if (is_at(asm, FUASM_TK_STMT_END)) {
            advance(asm);
            continue;
        }

        fuasm_token ct = current(asm);
        if (!is_at_mnemonic(asm)) {
            choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, ct.begin, "Expected a mnemonic.");
            exit(1);
        }

        if (ct.kind == FUASM_TK_MN_SECTION) {
            advance(asm);
            ct = expect(asm, FUASM_TK_GLOBAL_LABEL, "a section name");

            // TODO(local): Error if an invalid section name is provided.

            bool redefined_section = false;
            for (ssize_t i = 0; i < asm->sections.count; i++) {
                if (kos_sv_equals(ct.text_value, asm->sections.data[i].name)) {
                    redefined_section = true;
                    break;
                }
            }

            if (redefined_section) {
                choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, ct.end, "Redefinition of section '"KOS_STR_FMT"'.", KOS_STR_ARG(ct.text_value));
                exit(1);
            }

            current_section_index = asm->sections.count;
            kos_da_push(&asm->sections, ((fuasm_section_info){ .name = ct.text_value }));

            goto next_instruction;
        }

        int inst_count = mnemonic_instruction_count(ct.kind);
        if (inst_count == 0 || current_section_index < 0) {
            goto next_instruction;
        }

        if (asm->sections.count == 0) {
            current_section_index = 0;
            kos_da_push(&asm->sections, ((fuasm_section_info){ .name = KOS_SV_CONST("text") }));
            section = &asm->sections.data[current_section_index];
        }

        section->size += inst_count * 4;
        advance(asm);

    next_instruction:;
        while (!is_at_end(asm)) {
            bool is_stmt_end = is_at(asm, FUASM_TK_STMT_END);
            advance(asm);
            if (is_stmt_end) break;
        }
    }
}

static void read_statement(fuasm_assembler* asm) {
    if (is_at_end(asm)) return;
    if (is_at(asm, FUASM_TK_STMT_END)) {
        advance(asm);
        return;
    }

    fuasm_section_info* section = current_section(asm);
    i64 addr = section->instructions.count * 4;

    if (is_at(asm, FUASM_TK_GLOBAL_LABEL) || is_at(asm, FUASM_TK_LOCAL_LABEL)) {
        // we've already defined this symbols's section and address, just skip it for the parsing.
        advance(asm);
    }

    if (is_at(asm, FUASM_TK_STMT_END)) {
        advance(asm);
        return;
    }

    if (!is_at_mnemonic(asm)) {
        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, current(asm).begin, "Expected a mnemonic.");
        exit(1);
        // goto next_instruction_no_check;
    }

    switch (current(asm).kind) {
        default: {
            choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, current(asm).begin, "Unimplemented mnemonic.");
            exit(1);
        } break; // goto next_instruction_no_check;

        case FUASM_TK_MN_ADD: {
            advance(asm);
            gfu_register rd = expect_register(asm); expect_comma(asm);
            gfu_register rs = expect_register(asm); expect_comma(asm);
            if (is_at(asm, FUASM_TK_IMMEDIATE)) {
                u32 imm = current(asm).immediate_value; advance(asm);
                kos_da_push(&section->instructions, GFU_INST_ADDI(rd, rs, imm));
            } else {
                gfu_register rt = expect_register(asm);
                kos_da_push(&section->instructions, GFU_INST_ADD(rd, rs, rt));
            }
        } break;

        case FUASM_TK_MN_B: {
            advance(asm);
            i64 label;
            if (expect_label(asm, &label) == FUASM_TK_LOCAL_LABEL) {
                i32 offs = kos_cast(i32) (label - (addr + 4));
                kos_da_push(&section->instructions, GFU_INST_B(offs));
            } else {
                choir_diag_issue_source_bytes(asm->unit->context, CHOIR_FATAL, asm->unit->source, current(asm).begin, "Relocations are not yet implemented.");
                exit(1);
            }
        } break;

        case FUASM_TK_MN_NOP: {
            advance(asm);
            kos_da_push(&section->instructions, GFU_INST_NOP());
        } break;

        case FUASM_TK_MN_OR: {
            advance(asm);
            gfu_register rd = expect_register(asm); expect_comma(asm);
            gfu_register rs = expect_register(asm); expect_comma(asm);
            if (is_at(asm, FUASM_TK_IMMEDIATE)) {
                u32 imm = current(asm).immediate_value; advance(asm);
                kos_da_push(&section->instructions, GFU_INST_ORI(rd, rs, imm));
            } else {
                gfu_register rt = expect_register(asm);
                kos_da_push(&section->instructions, GFU_INST_OR(rd, rs, rt));
            }
        } break;

        case FUASM_TK_MN_MOVE: {
            advance(asm);
            gfu_register rd = expect_register(asm); expect_comma(asm);
            u32 imm = expect(asm, FUASM_TK_IMMEDIATE, "an immediate").immediate_value;
            kos_da_push(&section->instructions, GFU_INST_ORI(rd, GFU_REG_R0, imm));
        } break;
    }

next_instruction:;
    if (!is_at_end(asm) && !is_at(asm, FUASM_TK_STMT_END)) {
        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, current(asm).begin, "Extra tokens at the end of a statement.");
        exit(1);
    }

next_instruction_no_check:;
    while (!is_at_end(asm)) {
        bool is_stmt_end = is_at(asm, FUASM_TK_STMT_END);
        advance(asm);
        if (is_stmt_end) break;
    }
}
