/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include "internal.h"

#include <gamefu/iselgen/data.h>
#include "isel_tables.c"

static inline bool gfuas_is_ident_start(gfu_word ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_' || ch == '.');
}

static inline bool gfuas_is_ident_continue(gfu_word ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '_' || ch == '.');
}

static void gfuas_skip_trivia(gfuas_state* state) {
    while (!gfuas_lexer_at_end(state)) {
        gfu_word ch = state->lex_character;
        switch (ch) {
            default: return;

            case ' ': case '\t': case '\r': {
                gfuas_next_character(state);
            } break;

            case ';': {
                while (ch != '\0' && ch != '\n') {
                    gfuas_next_character(state);
                    ch = state->lex_character;
                }
            } break;
        }
    }
}

static inline int gfuas_get_hex_digit_value(int ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return 10 + (ch - 'a');
    if (ch >= 'A' && ch <= 'F') return 10 + (ch - 'A');
    return -1;
}

static inline bool gfuas_is_hex_digit(int ch) {
    return gfuas_get_hex_digit_value(ch) >= 0;
}

gfuas_token gfuas_read_token(gfuas_state* state) {
    gfuas_skip_trivia(state);
    gfuas_token token = {
        .kind = GFUAS_TK_INVALID,
        .location = state->lex_position,
    };

    if (gfuas_lexer_at_end(state)) {
        token.kind = GFUAS_TK_EOF;
        return token;
    }

    static struct {
        gfuas_directive directive;
        const char* image;
    } directives[] = {
#define DIR(Id, Image) { GFUAS_DIR_##Id, Image },
#include <gamefu/as/x/directives.h>
        {0},
    };

    static struct {
        gfuas_mnemonic mnemonic;
        const char* image;
    } mnemonics[] = {
#define MNEM(Id, Name) { GFUAS_MNEM_##Id, Name },
#include <gamefu/as/x/mnemonics.h>
        {0},
    };

    static struct {
        gfu_gpr gpr;
        const char* image;
    } gprs[] = {
#define GPR(Id, Image) { GFU_GPR_##Id, Image },
#include <gamefu/opcodes/x/registers.h>
        {0},
    };

    gfu_word ch = state->lex_character;
    switch (ch) {
        case '\n': case '\0': {
            token.kind = GFUAS_TK_ENDL;
            gfuas_next_character(state);
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
            gfuas_next_character(state);
            token.kind = ch == '.' ? GFUAS_TK_LABEL_LOCAL : GFUAS_TK_LABEL_GLOBAL;

            const char* ident = state->source.text + state->lex_position;
            gfu_uword ident_length = 0;

            while (ch = state->lex_character, gfuas_is_ident_continue(ch)) {
                gfuas_next_character(state);
                ident_length++;
            }

            for (int i = 0; mnemonics[i].image != nullptr; i++) {
                size_t mnemonic_length = strlen(mnemonics[i].image);
                if (ident_length != mnemonic_length) continue;
                if (0 == strncmp(ident, mnemonics[i].image, ident_length)) {
                    token.kind = GFUAS_TK_MNEMONIC;
                    token.as.mnemonic = mnemonics[i].mnemonic;
                    goto super_break;
                }
            }

            for (int i = 0; gprs[i].image != nullptr; i++) {
                size_t gpr_length = strlen(gprs[i].image);
                if (ident_length != gpr_length) continue;
                if (0 == strncmp(ident, gprs[i].image, ident_length)) {
                    token.kind = GFUAS_TK_GPR;
                    token.as.gpr = gprs[i].gpr;
                    goto super_break;
                }
            }
        } break;

        case '%': {
            gfuas_next_character(state);
            token.kind = GFUAS_TK_DIRECTIVE;

            const char* ident = state->source.text + state->lex_position;
            gfu_uword ident_length = 0;

            while (ch = state->lex_character, gfuas_is_ident_continue(ch)) {
                gfuas_next_character(state);
                ident_length++;
            }

            if (ident_length == 0) {
                diag_issue(DIAG_ERROR, state->source, token.location, "Expected a directive name.");
                goto super_break;
            }

            for (int i = 0; directives[i].image != nullptr; i++) {
                if (0 == strncmp(ident, directives[i].image, ident_length)) {
                    token.as.directive = directives[i].directive;
                    goto super_break;
                }
            }

            diag_issue(DIAG_ERROR, state->source, token.location, "Invalid directive.");
            goto super_break;
        } break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            gfu_double immediate = state->lex_character - '0';
            gfuas_next_character(state);
            token.kind = GFUAS_TK_IMMEDIATE;

            while (!gfuas_lexer_at_end(state) && state->lex_character >= '0' && state->lex_character <= '9') {
                immediate = (10 * immediate) + (state->lex_character - '0');
                gfuas_next_character(state);
            }

            token.as.immediate = (gfu_uword)immediate;
        } break;

        case '#': {
            gfu_double immediate = 0;
            gfuas_next_character(state);
            token.kind = GFUAS_TK_IMMEDIATE;

            token.kind = GFUAS_TK_IMMEDIATE;
            if (!gfuas_is_hex_digit(state->lex_character)) {
                diag_issue(DIAG_ERROR, state->source, state->lex_position, "Expected a hex digit.");
                goto super_break;
            }

            while (!gfuas_lexer_at_end(state) && gfuas_is_hex_digit(state->lex_character)) {
                immediate = (16 * immediate) + gfuas_get_hex_digit_value(state->lex_character);
                gfuas_next_character(state);
            }

            token.as.immediate = immediate;
        } break;

        case '"': {
            gfuas_next_character(state);
            token.kind = GFUAS_TK_BYTE_STRING;

            gfu_uword max_length = 0;
            while (
                state->lex_position + max_length < (gfu_uword) state->source.length &&
                state->source.text[max_length] != '\n' &&
                state->source.text[max_length] != '"'
            ) {
                max_length++;
            }

            char* byte_string = gfu_arena_alloc(&state->arena, max_length);
            gfu_uword length = 0;

            while (!gfuas_lexer_at_end(state) && state->lex_character != '\n' && state->lex_character != '"') {
                if (state->lex_character == '\\') {
                    gfuas_next_character(state);
                    int digits[2];
                    for (int i = 0; i < 2; i++) {
                        if (gfuas_lexer_at_end(state) || state->lex_character == '\n' || state->lex_character == '"' || (digits[i] = gfuas_get_hex_digit_value(state->lex_character), digits[i] < 0)) {
                            diag_issue(DIAG_ERROR, state->source, state->lex_position, "Expected hex digit in escape sequence.");
                            digits[i] = 0;
                        }
                        gfuas_next_character(state);
                    }
                    byte_string[length++] = (char)((digits[0] * 16) + digits[1]);
                } else {
                    byte_string[length++] = state->lex_character;
                    gfuas_next_character(state);
                }
            }

            if (state->lex_character != '"') {
                diag_issue(DIAG_ERROR, state->source, token.location, "Unterminated byte-string constant.");
                goto super_break;
            }

            gfuas_next_character(state);

            token.as.byte_string.data = byte_string;
            token.as.byte_string.length = length;
        } break;

        default: {
            if (ch >= 33 && ch <= 127) {
                token.kind = ch;
                gfuas_next_character(state);
            } else {
                /* Somewhat catch-all for generic character errors. */
                if (ch < 32) {
                    diag_issue(DIAG_ERROR, state->source, token.location, "Invalid or unexpected character in source text (0x%02X).", ch);
                } else if (ch < 128) {
                    diag_issue(DIAG_ERROR, state->source, token.location, "Invalid or unexpected character in source text '%c'.", (char)ch);
                } else {
                    diag_issue(DIAG_ERROR, state->source, token.location, "Invalid or unexpected character in source text (0x%08X).", ch);
                }

                token.kind = GFUAS_TK_INVALID;
                gfuas_next_character(state);
            }
        } break;
    }

super_break:;
    assertn(token.kind != GFUAS_TK_INVALID);
    return token;
}
