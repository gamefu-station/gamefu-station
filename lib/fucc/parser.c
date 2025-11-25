#include <gamefu/prologue.h>

#include <fucc/context.h>
#include <fucc/parser.h>
#include <fucc/asm_builder.h>

void fucc_parser_init(fucc_parser_t* p, fucc_context_t* context) {
    *p = (fucc_parser_t) {
        .context = context,
    };
}

void fucc_parser_deinit(fucc_parser_t* p) {
    fucc_asm_builder_deinit(&p->asm_builder);
    fucc_lexer_deinit(&p->lexer);
}

static bool fucc_parser_is_at_type(fucc_parser_t* p) {
    switch (p->token.kind) {
        default: return false;

        case FUCC_TK_KW_VOID:
        case FUCC_TK_KW_BOOL:
        case FUCC_TK_KW_BYTE:
        case FUCC_TK_KW_UBYTE:
        case FUCC_TK_KW_HALF:
        case FUCC_TK_KW_UHALF:
        case FUCC_TK_KW_WORD:
        case FUCC_TK_KW_UWORD:
            return true;

        case FUCC_TK_IDENT: {
            const char* ident = p->token.as.ident;
            gfu_assert(ident != nullptr, "Where is the identifier text?");
            fucc_decl_type_t** decl_types = p->context->decl_types;

            for (gfu_uword_t i = 0, dtcount = p->context->decl_type_count; i < dtcount; i++) {
                fucc_decl_type_t* dtype = decl_types[i];
                gfu_assert(dtype != nullptr, "Where is the decl_type?");
                gfu_assert(dtype->declared_name != nullptr, "Where is the decl_type's name?");
                if (0 == strcmp(ident, dtype->declared_name)) {
                    return true;
                }
            }

            return false;
        }
    }
}

static fucc_decl_type_t* fucc_alloc_decl_type(fucc_context_t* c, fucc_source_t* source, gfu_uword_t location, const char* declared_name) {
    fucc_decl_type_t* result = gfu_arena_alloc(&c->decl_type_arena, sizeof *result);
    *result = (fucc_decl_type_t) {
        .declared_source = source,
        .declared_location = location,
        .declared_name = declared_name,
    };

    if (c->decl_type_count >= c->decl_type_capacity) {
        c->decl_type_capacity *= 2;
        c->decl_types = realloc(c->decl_types, (size_t)c->decl_type_capacity * (sizeof *c->decl_types));
    }

    c->decl_types[c->decl_type_count++] = result;
    return result;
}

static bool fucc_parser_is_at_end(fucc_parser_t* p) {
    return p->token.kind == FUCC_TK_EOF;
}

static void fucc_parser_advance(fucc_parser_t* p) {
    if (p->token.kind == FUCC_TK_EOF) return;

    if (p->token.kind == '(' || p->token.kind == '[' || p->token.kind == '{') {
        p->bracket_nesting++;
    } else if (p->token.kind == ')' || p->token.kind == ']' || p->token.kind == '}') {
        gfu_assert(p->bracket_nesting > 0, "Bad bracket nesting slipped thru.");
        p->bracket_nesting--;
    }

    p->token = fucc_read_token(&p->lexer);
    p->location = p->token.location;
}

static bool fucc_parser_try(fucc_parser_t* p, fucc_token_kind_t kind) {
    if (p->token.kind != kind) return false;
    fucc_parser_advance(p);
    return true;
}

static bool fucc_parser_expect(fucc_parser_t* p, fucc_token_kind_t kind, const char* what) {
    if (fucc_parser_try(p, kind)) return true;
    fucc_diag_issue(p->context, FUCC_ERROR, p->source, p->location, "Expected %s.", what);
    return false;
}

static const char* fucc_parser_expect_ident(fucc_parser_t* p) {
    const char* ident = p->token.as.ident;
    if (!fucc_parser_expect(p, FUCC_TK_IDENT, "an identifier")) return nullptr;
    return ident;
}

static const char* fucc_parser_expect_label(fucc_parser_t* p) {
    const char* label = p->token.as.label;
    if (!fucc_parser_expect(p, FUCC_TK_LABEL, "an assembler label")) return nullptr;
    return label;
}

void fucc_parse_cc_unit(fucc_parser_t* p) {
    p->source = p->lexer.source;
    p->token = fucc_read_token(&p->lexer);
    p->location = p->token.location;

    // TODO(echoe): Parse includes decls & their sources.

    while (!fucc_parser_is_at_end(p)) {
        fucc_parse_cc_top_level(p);
        gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    }

    fucc_diag_flush(p->context);
}

static void fucc_parse_cc_function_type_decl(fucc_parser_t* p);
static void fucc_parse_cc_struct_type_decl(fucc_parser_t* p);
static void fucc_parse_cc_union_type_decl(fucc_parser_t* p);
static void fucc_parse_cc_enum_type_decl(fucc_parser_t* p);

static void fucc_parse_cc_section_decl(fucc_parser_t* p);
static void fucc_parse_cc_asm_decl(fucc_parser_t* p);
static void fucc_parse_cc_typed_decl(fucc_parser_t* p);

static void fucc_parse_cc_stmt(fucc_parser_t* p);
static void fucc_parse_cc_asm_stmt(fucc_parser_t* p);

static void fucc_parse_cc_expr(fucc_parser_t* p);

static fucc_type_ref_t fucc_parse_cc_type(fucc_parser_t* p);

static fucc_evaluated_constant_t fucc_parse_cc_constant_expression(fucc_parser_t* p);
// static bool fucc_parse_cc_constant_boolean_expression(fucc_parser_t* p);
static gfu_uword_t fucc_parse_cc_constant_integer_expression(fucc_parser_t* p);

static void fucc_parse_as_statement(fucc_parser_t* p);

static void fucc_parser_skip_to_top_level_end(fucc_parser_t* p) {
    while (!fucc_parser_is_at_end(p)) {
        fucc_token_kind_t kind = p->token.kind;
        fucc_parser_advance(p);
        if ((kind == ';' || kind == '}') && p->bracket_nesting == 0) {
            if (kind == '}') fucc_parser_try(p, ';');
            break;
        }
    }

    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
}

void fucc_parse_cc_top_level(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;
    switch (p->token.kind) {
        default: {
            if (fucc_parser_is_at_type(p)) {
                fucc_parse_cc_typed_decl(p);
                break;
            }

            fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "Unexpected token at top level.");
            fucc_parser_skip_to_top_level_end(p);
        } break;

        case FUCC_TK_KW_FUNCTION: fucc_parse_cc_function_type_decl(p); break;
        case FUCC_TK_KW_STRUCT: fucc_parse_cc_struct_type_decl(p); break;
        case FUCC_TK_KW_UNION: fucc_parse_cc_union_type_decl(p); break;
        case FUCC_TK_KW_ENUM: fucc_parse_cc_enum_type_decl(p); break;

        case FUCC_TK_KW_SECTION: fucc_parse_cc_section_decl(p); break;
        case FUCC_TK_KW_ASM: fucc_parse_cc_asm_decl(p); break;
    }
}

static void fucc_parse_cc_function_type_decl(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;

    fucc_parser_advance(p);

    fucc_type_ref_t return_type;
    if (fucc_parser_is_at_type(p)) {
        return_type = fucc_parse_cc_type(p);
        gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    } else {
        fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "Expected a type.");
        fucc_parser_skip_to_top_level_end(p);
        return;
    }

    gfu_uword_t declared_location = p->location;
    const char* declared_name = fucc_parser_expect_ident(p);
    if (declared_name == nullptr) {
        fucc_parser_skip_to_top_level_end(p);
        return;
    }

    fucc_decl_type_t* function_type = fucc_alloc_decl_type(p->context, p->source, declared_location, declared_name);
    function_type->kind = FUCC_DECL_TYPE_FUNCTION;

    if (!fucc_parser_expect(p, '(', "'('")) {
        fucc_parser_skip_to_top_level_end(p);
        return;
    }

    gfu_assert(p->bracket_nesting == 1, "Should be in parameters.");
    // TODO(echoe): Parse function type decl parameters.

    gfu_assert(p->bracket_nesting == 1, "Should be in parameters.");
    if (!fucc_parser_expect(p, ')', "')'")) {
        fucc_parser_skip_to_top_level_end(p);
        return;
    }

    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    if (!fucc_parser_expect(p, ';', "';'")) {
        fucc_parser_skip_to_top_level_end(p);
        return;
    }
}

static void fucc_parse_cc_struct_type_decl(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;
    fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "TODO: Parse struct type decl.");
    fucc_parser_advance(p);
    fucc_parser_skip_to_top_level_end(p);
}

static void fucc_parse_cc_union_type_decl(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;
    fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "TODO: Parse union type decl.");
    fucc_parser_advance(p);
    fucc_parser_skip_to_top_level_end(p);
}

static void fucc_parse_cc_enum_type_decl(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;
    fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "TODO: Parse enum type decl.");
    fucc_parser_advance(p);
    fucc_parser_skip_to_top_level_end(p);
}

static void fucc_parse_cc_section_decl(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;
    fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "TODO: Parse section decl.");
    fucc_parser_advance(p);
    fucc_parser_skip_to_top_level_end(p);
}

static void fucc_parse_cc_asm_decl(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;

    fucc_parser_advance(p);

    gfu_uword_t declared_location = p->location;
    const char* declared_name = fucc_parser_expect_label(p);
    if (declared_name == nullptr) {
        fucc_parser_skip_to_top_level_end(p);
        return;
    }

    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    if (!fucc_parser_expect(p, '{', "'{'")) {
        fucc_parser_skip_to_top_level_end(p);
        return;
    }

    fucc_asm_builder_t* b = &p->asm_builder;
    fucc_asm_builder_enter_section(b, GFUOBJ_CLASS_TEXT, nullptr, 0xFFFFFFFF, 0xFFFFFFFF);

    while (!fucc_parser_is_at_end(p) && !fucc_parser_try(p, '}')) {
        gfu_assert(p->bracket_nesting == 1, "Should be in nested assembler statements block.");
        fucc_parse_as_statement(p);
    }

    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");

    fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "TODO: Parse asm decl.");
    fucc_parser_skip_to_top_level_end(p);
}

static void fucc_parse_cc_typed_decl(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;

    fucc_type_ref_t type = fucc_parse_cc_type(p);
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");

    gfu_uword_t declared_location = p->location;
    const char* declared_name = fucc_parser_expect_ident(p);
    if (declared_name == nullptr) {
        fucc_parser_skip_to_top_level_end(p);
        return;
    }

    fucc_asm_builder_t* b = &p->asm_builder;

    if (fucc_parser_try(p, ';')) {
        fucc_asm_builder_enter_section(b, GFUOBJ_CLASS_DATA, nullptr, 0xFFFFFFFF, 0xFFFFFFFF);
        fucc_asm_builder_define_label(b, declared_name, FUCC_ASMB_MANGLE);
        fucc_build_bytes(b, fucc_type_ref_sizeof(type));
    } else if (fucc_parser_try(p, '=')) {
        fucc_asm_builder_enter_section(b, GFUOBJ_CLASS_DATA, nullptr, 0xFFFFFFFF, 0xFFFFFFFF);
        fucc_asm_builder_define_label(b, declared_name, FUCC_ASMB_MANGLE);
        fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "Global initializer lists are currently not supported.");
        fucc_parser_skip_to_top_level_end(p);
    } else {
        if (!fucc_parser_expect(p, '(', "'('")) {
            fucc_parser_skip_to_top_level_end(p);
            return;
        }

        fucc_asm_builder_enter_section(b, GFUOBJ_CLASS_TEXT, nullptr, 0xFFFFFFFF, 0xFFFFFFFF);
        fucc_asm_builder_define_label(b, declared_name, FUCC_ASMB_MANGLE);
    }
}

static void fucc_parse_cc_type_specifiers(fucc_parser_t* p, fucc_type_ref_t* type) {
    fucc_context_t* c = p->context;

    fucc_type_specifier_t kind = FUCC_SPEC_NONE;
    gfu_uword_t count = 0;

    gfu_uword_t specifier_location = p->location;

    switch (p->token.kind) {
        default: return;

        case '[': {
            kind = FUCC_SPEC_ARRAY;
            fucc_parser_advance(p);
            count = fucc_parse_cc_constant_integer_expression(p);
            fucc_parser_expect(p, ']', "']'");
        } break;

        case '*': {
            kind = FUCC_SPEC_POINTER;
            fucc_parser_advance(p);
        } break;
    }

    fucc_type_qualifier_t qualifiers = FUCC_QUAL_NONE;
    while (!fucc_parser_is_at_end(p)) {
        gfu_uword_t qual_location = p->token.location;
        if (fucc_parser_try(p, FUCC_TK_KW_CONST)) {
            if (0 != (qualifiers & FUCC_QUAL_CONST)) {
                fucc_diag_issue(c, FUCC_ERROR, p->source, qual_location, "Duplicate 'const' type qualifier.");
            }
            qualifiers |= FUCC_QUAL_CONST;
        } else if (fucc_parser_try(p, FUCC_TK_KW_VOLATILE)) {
            if (0 != (qualifiers & FUCC_QUAL_VOLATILE)) {
                fucc_diag_issue(c, FUCC_ERROR, p->source, qual_location, "Duplicate 'volatile' type qualifier.");
            }
            qualifiers |= FUCC_QUAL_VOLATILE;
        } else break;
    }

    gfu_assert(kind != FUCC_SPEC_NONE, "Should have decided a specifier kind.");

    if (type->specifier_count < FUCC_MAX_TYPE_SPECIFIERS) {
        type->specifiers[type->specifier_count].kind = kind;
        type->specifiers[type->specifier_count].qualifiers = qualifiers;
        type->specifiers[type->specifier_count].count = count;
        type->specifier_count++;
    } else {
        // only show the error once.
        if (type->specifier_count == FUCC_MAX_TYPE_SPECIFIERS) {
            fucc_diag_issue(c, FUCC_ERROR, p->source, specifier_location, "Internal type specifier limit (%u) reached.", FUCC_MAX_TYPE_SPECIFIERS);
            type->specifier_count++;
        }
    }

    fucc_parse_cc_type_specifiers(p, type);
}

static fucc_type_ref_t fucc_parse_cc_type(fucc_parser_t* p) {
    fucc_context_t* c = p->context;
    fucc_type_ref_t result = {
        .source = p->source,
        .location = p->location,
    };

    switch (p->token.kind) {
        default: {
            fucc_diag_issue(c, FUCC_FATAL, p->source, p->location, "Should not have attempted to parse a type here.");
        } break;

        case FUCC_TK_KW_VOID: {
            result.kind = FUCC_TYPE_VOID;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_KW_BOOL: {
            result.kind = FUCC_TYPE_BOOL;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_KW_BYTE: {
            result.kind = FUCC_TYPE_BYTE;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_KW_UBYTE: {
            result.kind = FUCC_TYPE_UBYTE;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_KW_HALF: {
            result.kind = FUCC_TYPE_HALF;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_KW_UHALF: {
            result.kind = FUCC_TYPE_UHALF;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_KW_WORD: {
            result.kind = FUCC_TYPE_WORD;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_KW_UWORD: {
            result.kind = FUCC_TYPE_UWORD;
            fucc_parser_advance(p);
        } break;

        case FUCC_TK_IDENT: {
            const char* ident = p->token.as.ident;
            fucc_decl_type_t** decl_types = p->context->decl_types;

            fucc_decl_type_t* decl_type = nullptr;
            for (gfu_uword_t i = 0, dtcount = p->context->decl_type_count; i < dtcount; i++) {
                fucc_decl_type_t* dtype = decl_types[i];
                if (0 == strcmp(ident, dtype->declared_name)) {
                    decl_type = dtype;
                    break;
                }
            }

            gfu_assert(decl_type != nullptr, "Should have validated this is a type name before calling.");
            fucc_parser_advance(p);

            result.kind = FUCC_TYPE_DECL;
            result.decl_type = decl_type;
        } break;
    }

    while (!fucc_parser_is_at_end(p)) {
        gfu_uword_t qual_location = p->token.location;
        if (fucc_parser_try(p, FUCC_TK_KW_CONST)) {
            if (0 != (result.qualifiers & FUCC_QUAL_CONST)) {
                fucc_diag_issue(c, FUCC_ERROR, p->source, qual_location, "Duplicate 'const' type qualifier.");
            }
            result.qualifiers |= FUCC_QUAL_CONST;
        } else if (fucc_parser_try(p, FUCC_TK_KW_VOLATILE)) {
            if (0 != (result.qualifiers & FUCC_QUAL_VOLATILE)) {
                fucc_diag_issue(c, FUCC_ERROR, p->source, qual_location, "Duplicate 'volatile' type qualifier.");
            }
            result.qualifiers |= FUCC_QUAL_VOLATILE;
        } else break;
    }

    gfu_assert(result.location < p->location, "Did not consume a type token.");
    fucc_parse_cc_type_specifiers(p, &result);
    return result;
}

static fucc_evaluated_constant_t fucc_parse_cc_constant_expression(fucc_parser_t* p) {
    fucc_evaluated_constant_t result = {0};

    switch (p->token.kind) {
        default: return result;

        case FUCC_TK_INTEGER: {
            result.kind = FUCC_EVAL_INTEGER;
            result.as.integer = p->token.as.integer;
            fucc_parser_advance(p);
        } break;
    }

    return result;
}

static gfu_uword_t fucc_parse_cc_constant_integer_expression(fucc_parser_t* p) {
    fucc_evaluated_constant_t result = fucc_parse_cc_constant_expression(p);
    if (result.kind != FUCC_EVAL_INTEGER) {
        fucc_diag_issue(p->context, FUCC_ERROR, p->source, result.location, "Expected an integer expression.");
        return 0;
    } else return result.as.integer;
}

gfu_uword_t fucc_type_ref_sizeof(fucc_type_ref_t type) {
    gfu_uword_t result_size;
    switch (type.kind) {
        case FUCC_TYPE_NONE: result_size = 0; break;
        case FUCC_TYPE_VOID: result_size = 0; break;
        case FUCC_TYPE_BOOL: result_size = 1; break;
        case FUCC_TYPE_BYTE: result_size = 1; break;
        case FUCC_TYPE_UBYTE: result_size = 1; break;
        case FUCC_TYPE_HALF: result_size = 2; break;
        case FUCC_TYPE_UHALF: result_size = 2; break;
        case FUCC_TYPE_WORD: result_size = 4; break;
        case FUCC_TYPE_UWORD: result_size = 4; break;
        case FUCC_TYPE_DECL: result_size = fucc_decl_type_sizeof(type.decl_type); break;
    }

    for (gfu_ubyte_t i = 0; i < type.specifier_count; i++) {
        switch (type.specifiers[i].kind) {
            default: gfu_assertf(false, "Unimplemented type specifier case %u\n.", type.specifiers[i].kind); break;
            case FUCC_SPEC_ARRAY: result_size *= type.specifiers[i].count; break;
            case FUCC_SPEC_POINTER: result_size = 4; break;
        }
    }

    return result_size;
}

gfu_uword_t fucc_decl_type_sizeof(fucc_decl_type_t* type) {
    gfu_assert(type != nullptr, "Where is the type?");
    switch (type->kind) {
        case FUCC_DECL_TYPE_FUNCTION: return 4;

        case FUCC_DECL_TYPE_STRUCT:
        case FUCC_DECL_TYPE_UNION: {
            gfu_uword_t size = 0;
            gfu_uword_t align = 1;

            gfu_assert(size != 0, "Implementation failure for struct/union type size.");
            return size + ((align - (size % align)) % align);
        }

        case FUCC_DECL_TYPE_ENUM: return 4;
    }
}

static void fucc_parse_as_statement(fucc_parser_t* p) {
    gfu_assert(p->bracket_nesting == 0, "Should be at top level.");
    fucc_context_t* c = p->context;
    fucc_asm_builder_t* b = &p->asm_builder;

    switch (p->token.kind) {
        case '\n': break;
    }

    if (p->token.kind != '\n') {
        fucc_diag_issue(c, FUCC_ERROR, p->source, p->location, "Extra tokens at the end of assembler statement.");
        while (!fucc_parser_is_at_end(p) && p->token.kind != '\n') {
            fucc_parser_advance(p);
        }
    }

    (void)fucc_parser_try(p, '\n');
}
