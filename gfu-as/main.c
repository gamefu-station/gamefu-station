/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/common.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <io.h>
#  define isatty _isatty
#  define fileno _fileno
#elif defined(__linux__)
#  include <unistd.h>
#endif

#include <gamefu/bfd/object.h>

#include <gamefu/as.h>

int main(int argc, char** argv) {
    return gfuas_driver_main(argc, argv);
}

#define VERSION "0.2.0"

typedef enum gfuas_token_kind {
    GFUAS_TK_INVALID = 0x00FFFFFF,
    GFUAS_TK_EOF = 0,

    _GFUAS_TK_MULTIBYTE_BEGIN = 127,

#define TK(Id) GFUAS_TK_##Id,
#include <gamefu/as/x/tokens.h>
} gfuas_token_kind;

typedef struct gfuas_token {
    gfuas_token_kind kind;
    gfu_uword location;
    union {
        gfuas_directive directive;
        gfuas_mnemonic mnemonic;
        gfu_gpr gpr;
        gfu_c0r c0r;
        gfu_uword immediate;
        const char* label;
        struct {
            const char* data;
            gfu_uword length;
        } byte_string;
    } as;
} gfuas_token;

typedef struct gfuas_state {
    const char* program;
    bool show_help : 1;
    bool show_version : 1;
    bool verbose : 1;
    source source;
    const char* output;

    gfu_arena arena;
    gfuas_builder asmb;
    gfuobj_builder objb;

    gfu_uword lex_position;
    gfu_word lex_character;
    gfu_word lex_stride;

    gfuas_token token, next_token;

    const char* entry_name;
    bool is_entry_set;
} gfuas_state;

#define SHIFT (argc == 0 ? nullptr : (argc--, *(argv++)))

static bool gfuas_options_parse(int argc, char** argv, gfuas_state* state) {
    bool result = false;

    state->program = SHIFT;

    for (int i = 0; i < argc; i++) {
        const char* option = argv[i];
        if (0 == strcmp("--help", option)) {
            state->show_help = true;
            return true;
        } else if (0 == strcmp("--version", option)) {
            state->show_version = true;
            return true;
        }
    }

    while (argc > 0) {
        const char* opt = SHIFT;
        assertn(opt != nullptr);

        if (0 == strcmp(opt, "-as1")) {
            continue;
        } else if (0 == strcmp(opt, "--verbose") || 0 == strcmp(opt, "-v")) {
            state->verbose = true;
        } else if (0 == strcmp(opt, "-o")) {
            const char* path = SHIFT;
            if (path == nullptr) {
                diag_issue(DIAG_ERROR, NOSOURCE, "Option '-o' requires an argument.");
                goto fail;
            }
        } else {
            if (state->source.name != nullptr) {
                diag_issue(DIAG_ERROR, NOSOURCE, "Only one source file may be assembled at a time.");
                goto fail;
            }
            if (!load_source_from_file(opt, &state->source)) {
                goto fail;
            }
        }
    }

    result = true;
fail:;
    diag_flush();
    return result;
}

#undef SHIFT

static void gfuas_show_help(const char* program) {
    fprintf(
        stderr,
        "%s [options...] <file>\n"
        "Options:\n"
        "  --help          Show this help text and exit.\n"
        "  --version       Show version info and exit.\n"
        "  -v, --verbose   Enable verbose output.\n"
        "\n"
        "  <file>          The single input GameFU ASM source file.\n"
        "  -o <file>, --output <file>\n"
        "                  Output ROM to <file>.\n",
        program
    );
}

static void gfuas_show_version(void) {
    fprintf(
        stderr,
        "GameFU Assembler version " VERSION "\n"
        "This tool is part of the GameFU Station fantasy console project toolchain.\n"
    );
}

static inline void gfuas_print_verbose(gfuas_state* state, const char* format, ...) {
    if (!state->verbose) return;
    va_list v;
    va_start(v, format);
    vfprintf(stderr, format, v);
    va_end(v);
    fprintf(stderr, "\n");
}

static inline void gfuas_next_character(gfuas_state* state);
static gfuas_token gfuas_read_token(gfuas_state* state);

static gfuobj_raw* gfuas_assemble_internal(gfuas_state* state);
static gfuobj_raw* gfuas_assemble_ir_internal(gfuas_state* state);

GAMEFU_API int gfuas_driver_main(int argc, char** argv) {
    int result = 1;
    FILE* f = nullptr;

    diag_color_output(isatty(fileno(stderr)));

    gfuas_state state = {
        .entry_name = "_start",
    };

    if (!gfuas_options_parse(argc, argv, &state)) {
        goto fail;
    }

    if (state.show_help) {
        gfuas_show_help(state.program);
        goto success;
    }

    if (state.show_version) {
        gfuas_show_version();
        goto success;
    }

    if (state.source.name == nullptr) {
        gfuas_show_help(state.program);
        fprintf(stderr, "\n");
        diag_issue(DIAG_ERROR, NOSOURCE, "No source file provided.");
        goto fail;
    }

    gfu_arena_init(&state.arena, 32 * 1024);

    const char* output_name = state.output;
    if (output_name == nullptr) {
        const char* base_source_name = state.source.name;
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

        char* new_output_name = gfu_arena_alloc(&state.arena, (last_dot - source_name) + 5);
        sprintf(new_output_name, "%.*s.gfu", (int)(last_dot - source_name), source_name);
        output_name = new_output_name;
    }

    gfuas_print_verbose(&state, "Generating '%s'", output_name);

    gfuobj_raw* rom_data = gfuas_assemble_internal(&state);
    if (rom_data == nullptr) goto fail;

    errno = 0;
    f = fopen(output_name, "wb");
    if (f == nullptr) {
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to open output file '%s': %s", output_name, strerror(errno));
        goto fail;
    }

    errno = 0;
    fwrite(rom_data, (size_t) rom_data->header.rom_size, 1, f);
    free(rom_data);
    if (ferror(f)) {
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to write to output file '%s': %s", output_name, strerror(errno));
        goto fail;
    }

    // if we make it here, we succeeded; set the exit code appropriately.
success:;
    result = 0;
fail:;
    diag_flush();
    if (f != nullptr) fclose(f);
    free((void*) state.source.text);
    gfu_arena_deinit(&state.arena);
    return result;
}

GAMEFU_API int gfuas_driver_fuzz(const char* text, size_t length) {
    return 0;
}

GAMEFU_API gfuobj_raw* gfuas_source_assemble(source source) {
    gfuas_state state = {0};
    state.source = source;
    gfu_arena_init(&state.arena, 32 * 1024);
    gfuobj_raw* rom_data = gfuas_assemble_internal(&state);
    gfu_arena_deinit(&state.arena);
    return rom_data;
}

GAMEFU_API gfuobj_raw* gfuas_builder_assemble(gfuas_builder* builder) {
    gfuas_state state = {0};
    state.source = (source) { .name = "<internal>", .text = "\n", .length = 1 };
    state.asmb = *builder;
    gfu_arena_init(&state.arena, 32 * 1024);
    gfuobj_raw* rom_data = gfuas_assemble_ir_internal(&state);
    gfu_arena_deinit(&state.arena);
    return rom_data;
}

#include <gamefu/iselgen/data.h>
#include "isel_tables.c"

static gfuobj_raw* gfuas_assemble_ir_internal(gfuas_state* state) {
    return nullptr;
}

static gfuobj_raw* gfuas_assemble_internal(gfuas_state* state) {
    state->lex_position = 0;
    gfuas_next_character(state);

    return gfuas_assemble_ir_internal(state);
}

static inline bool gfuas_lexer_at_end(gfuas_state* state) {
    return state->lex_position >= (gfu_uword) state->source.length;
}

static inline void gfuas_next_character(gfuas_state* state) {
    state->lex_position += state->lex_stride;
    if (gfuas_lexer_at_end(state)) {
        state->lex_character = 0;
        return;
    }

    state->lex_character = state->source.text[state->lex_position];
    state->lex_stride = 1;
}

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

static gfuas_token gfuas_read_token(gfuas_state* state) {
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
