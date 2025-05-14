#include <gamefu/fuasm.h>
#include "lexer_internal.h"

static struct {
    kos_string_view image;
    fuasm_token_kind kind;
} keywords[] = {
#define TK_REGISTER(Id, Spelling) {KOS_SV_CONST(Spelling), FUASM_TK_REG_##Id},
#define TK_MNEMONIC(Id, Spelling) {KOS_SV_CONST(Spelling), FUASM_TK_MN_##Id},
#include <gamefu/fuasm/tokens.h>
    {0}, // sentinel terminator
};

static bool is_space(char c) {
    return c == ' ' || c == '\t';
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_idstart(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '.' || c == '_';
}

static bool is_idcont(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

static char current_raw(fuasm_lexer* lexer) {
    return choir_source_text_get(lexer->source, nullptr)[lexer->position];
}

static bool at_eof(fuasm_lexer* lexer) {
    ssize_t source_length;
    kos_discard choir_source_text_get(lexer->source, &source_length);
    return lexer->position >= source_length || current_raw(lexer) == 0;
}

static char current(fuasm_lexer* lexer) {
    char c = choir_source_text_get(lexer->source, nullptr)[lexer->position];

    ssize_t source_length;
    const char* source_text = choir_source_text_get(lexer->source, &source_length);
    if (c == '\\' && lexer->position + 1 < source_length) {
        char cnext = source_text[lexer->position + 1];
        if (cnext == '\n' || cnext == '\r') {
            return ' ';
        }
    } else if (c == '\r') {
        return '\n';
    }

    return c;
}

static void advance(fuasm_lexer* lexer) {
    if (at_eof(lexer)) {
        return;
    }

    char c = current_raw(lexer);
    lexer->position += 1;

    if (c == '\n' || c == '\r') {
        if ((c == '\n' && current_raw(lexer) == '\r') || (c == '\r' && current_raw(lexer) == '\n')) {
            lexer->position += 1;
        }
    } else if (c == '\\' && (current_raw(lexer) == '\n' || current_raw(lexer) == '\r')) {
        c = current_raw(lexer);
        lexer->position += 1;
        if ((c == '\n' && current_raw(lexer) == '\r') || (c == '\r' && current_raw(lexer) == '\n')) {
            lexer->position += 1;
        }
    }
}

static void skip_white_space(fuasm_lexer* lexer) {
    char c;
    while (c = current(lexer), is_space(c) || c == ';') {
        advance(lexer);
        if (c == ';') {
            while (c = current(lexer), c != '\n') {
                advance(lexer);
            }
        }
    }
}

fuasm_token fuasm_read_token(fuasm_lexer* lexer) {
    skip_white_space(lexer);

    fuasm_token result = {0};
    result.begin = lexer->position;

    if (at_eof(lexer)) {
        result.kind = FUASM_TK_END_OF_FILE;
        result.end = lexer->position;
        return result;
    }

    char c = current(lexer);
    switch (c) {
        case '\n': {
            result.kind = FUASM_TK_STMT_END;
            advance(lexer);
        } break;

        case ',': {
            result.kind = FUASM_TK_COMMA;
            advance(lexer);
        } break;

        default: {
            if (is_idstart(c)) {
                bool is_local_label = c == '.';

                advance(lexer);
                while (c = current(lexer), is_idcont(c)) {
                    advance(lexer);
                }

                result.text_value = kos_sv(choir_source_text_get(lexer->source, nullptr) + result.begin, lexer->position - result.begin);
                if (current(lexer) == ':') {
                    advance(lexer);
                    result.kind = is_local_label ? FUASM_TK_LOCAL_LABEL : FUASM_TK_GLOBAL_LABEL;
                } else {
                    for (isize i = 0; 0 != keywords[i].kind; i++) {
                        if (kos_sv_equals(keywords[i].image, result.text_value)) {
                            result.kind = keywords[i].kind;
                            break;
                        }
                    }

                    if (result.kind == FUASM_TK_INVALID) {
                        // if we didn't find a valid keyword to transform this identifier into, then it's a label reference
                        result.kind = is_local_label ? FUASM_TK_LOCAL_LABEL : FUASM_TK_GLOBAL_LABEL;
                    }
                }
            } else if (is_digit(c)) {
                int64_t imm_value = c - '0';

                advance(lexer);
                while (c = current(lexer), is_digit(c)) {
                    imm_value = imm_value * 10 + (c - '0');
                    advance(lexer);
                }

                if ((imm_value & 0xFFFFFFFFL) != imm_value) {
                    choir_diag_issue_source_bytes(lexer->context, CHOIR_ERROR, lexer->source, result.begin, "Integer cannot be represented within 32 bits.");
                }

                result.kind = FUASM_TK_IMMEDIATE;
                result.immediate_value = (int32_t)(imm_value & 0xFFFFFFFFL);
            } else {
                if (c > 32 && c <= 127)
                    choir_diag_issue_source_bytes(lexer->context, CHOIR_ERROR, lexer->source, result.begin, "Unexpected character '%c'.", c);
                else choir_diag_issue_source_bytes(lexer->context, CHOIR_ERROR, lexer->source, result.begin, "Unexpected character 0x%02X.", (int)(unsigned char)c);
            }
        } break;
    }

    result.end = lexer->position;
    return result;
}
