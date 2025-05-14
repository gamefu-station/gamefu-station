#include <gamefu/fuasm.h>
#include "lexer_internal.h"

typedef struct fuasm_section_info {
    kos_string_view name;
    int64_t size;
} fuasm_section_info;

typedef struct fuasm_section_infos {
    KOS_DYNAMIC_ARRAY_FIELDS(fuasm_section_info);
} fuasm_section_infos;

typedef struct fuasm_symbol_addr {
    kos_string_view name;
    int64_t addr;
    bool is_global : 1;
    bool is_extern : 1;
} fuasm_symbol_addr;

typedef struct fuasm_symbol_addrs {
    KOS_DYNAMIC_ARRAY_FIELDS(fuasm_symbol_addr);
} fuasm_symbol_addrs;

typedef struct fuasm_assembler {
    fuasm_translation_unit* unit;

    fuasm_tokens tokens;
    ssize_t token_position;

    fuasm_section_infos sections;
    fuasm_symbol_addrs symbols;
} fuasm_assembler;

static int mnemonic_instruction_count(fuasm_token_kind kind);

static bool is_at_end(fuasm_assembler* asm);
static void advance(fuasm_assembler* asm);
static fuasm_token current(fuasm_assembler* asm);
static bool is_at(fuasm_assembler* asm, fuasm_token_kind kind);
static bool is_at_mnemonic(fuasm_assembler* asm);
static fuasm_token expect(fuasm_assembler* asm, fuasm_token_kind kind, const char* desc);

static void precalculate_symbol_addresses(fuasm_assembler* asm);

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

    asm.token_position = 0;
    for (ssize_t i = 0; i < asm.sections.count; i++) {
        fprintf(stderr, "Section '"KOS_STR_FMT"' is %zd bytes long\n", KOS_STR_ARG(asm.sections.data[i].name), asm.sections.data[i].size);
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

static fuasm_token expect(fuasm_assembler* asm, fuasm_token_kind kind, const char* desc) {
    if (is_at_end(asm)) {
        ssize_t source_length;
        kos_discard choir_source_text_get(asm->unit->source, &source_length);

        choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, source_length, "Expected %s.", desc);
        exit(1);
    }

    fuasm_token token = current(asm);
    if (token.kind == kind) return token;

    choir_diag_issue_source_bytes(asm->unit->context, CHOIR_ERROR, asm->unit->source, token.begin, "Expected %s.", desc);
    exit(1);
}

static void precalculate_symbol_addresses(fuasm_assembler* asm) {
    ssize_t current_section_index = -1;
    while (!is_at_end(asm)) {
        fprintf(stderr, "at %s\n", fuasm_token_kind_name_get(current(asm).kind));

        bool is_at_label_def = is_at(asm, FUASM_TK_GLOBAL_LABEL) || is_at(asm, FUASM_TK_LOCAL_LABEL);
        if (is_at_label_def && asm->sections.count == 0) {
            current_section_index = 0;
            kos_da_push(&asm->sections, ((fuasm_section_info){ .name = KOS_SV_CONST("text") }));
        }

        fuasm_section_info* section = &asm->sections.data[current_section_index];

        if (is_at_label_def) {
            kos_string_view label_name = current(asm).text_value;
            fuasm_symbol_addr symbol = {
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

        section->size += inst_count;
        advance(asm);

    next_instruction:;
        while (!is_at_end(asm)) {
            bool is_stmt_end = is_at(asm, FUASM_TK_STMT_END);
            advance(asm);
            if (is_stmt_end) break;
        }
    }
}
