/* echoetok.h

*/

#ifndef ECHOETOK_H_
#define ECHOETOK_H_

#include <stdbool.h>
#include <stdint.h>

#define ETOK_CHAR_INVALID (-1)

enum {
    ETOK_EOF = 0,

    ETOK_CONTROL_BEGIN = 0x01,
    ETOK_CONTROL_END = 0x1F,

    ETOK_PRINTABLE_BEGIN = 0x21,
    ETOK_PRINTABLE_END = 0x7E,

    ETOK_INVALID_CHARACTER = 0x7F,

    ETOK_MULTIBYTE_BEGIN = 0x100,
    ETOK_MULTIBYTE_END = 0x7FFFFFFF,

    ETOK_IDENT = ETOK_MULTIBYTE_BEGIN,
    ETOK_INTEGER,
    ETOK_DOUBLE_QUOTE_STRING,
    ETOK_SINGLE_QUOTE_STRING,
};

typedef struct etok_lexer etok_lexer;

typedef bool (*etok_char_predicate)(void* userdata, int ch);
typedef int (*etok_char_decoder)(void* userdata, const char* data, size_t data_count, int* stride);
typedef void (*etok_error_callback)(void* userdata, const char* source_name, const char* source_text, const char* where, uint64_t line, uint64_t column, const char* format, ...);
typedef bool (*etok_comment_consumer)(etok_lexer* lexer);

typedef struct etok_token {
    int kind;
    const char* source_name;
    const char* begin, *end;
    uint64_t line, column;
} etok_token;

struct etok_lexer {
    void* userdata;

    const char* source_name;
    const char* source_begin;
    const char* source_current;
    const char* source_end;

    int ch, stride;
    uint64_t line, column;

    etok_char_decoder decoder;

    etok_char_predicate white_space_predicate;
    etok_char_predicate decimal_digit_predicate;
    etok_char_predicate ident_begin_predicate;
    etok_char_predicate ident_continue_predicate;

    etok_error_callback error_callback;

    etok_comment_consumer comment_consumer;
    char string_escape_ch;
};

void etok_lexer_init(etok_lexer* lexer, const char* source_name, const char* source_text, const char* source_text_end);
etok_token etok_lexer_read(etok_lexer* lexer);
bool etok_lexer_is_at_end(const etok_lexer* lexer);
void etok_lexer_advance(etok_lexer* lexer);
int etok_lexer_peek(const etok_lexer* lexer, int ahead);

#define etok_lexer_issue_error(Lexer, Where, Line, Column, Message) do { \
        if ((Lexer)->error_callback == NULL) { \
            etok_print_error_to_stderr((Lexer)->userdata, (Lexer)->source_name, (Lexer)->source_begin, (Where), (Line), (Column), "" Message ""); \
        } else { \
            (Lexer)->error_callback((Lexer)->userdata, (Lexer)->source_name, (Lexer)->source_begin, (Where), (Line), (Column), "" Message ""); \
        } \
    } while (0)

#define etok_lexer_issue_errorf(Lexer, Where, Line, Column, Format, ...) do { \
        if ((Lexer)->error_callback == NULL) { \
            etok_print_error_to_stderr((Lexer)->userdata, (Lexer)->source_name, (Lexer)->source_begin, (Where), (Line), (Column), "" Format "", __VA_ARGS__); \
        } else { \
            (Lexer)->error_callback((Lexer)->userdata, (Lexer)->source_name, (Lexer)->source_begin, (Where), (Line), (Column), "" Format "", __VA_ARGS__); \
        } \
    } while (0)

int etok_lexer_decode_char(const etok_lexer* lexer, size_t decode_offset, int* stride);
bool etok_lexer_is_white_space(const etok_lexer* lexer, int ch);
bool etok_lexer_is_decimal_digit(const etok_lexer* lexer, int ch);
bool etok_lexer_is_ident_begin(const etok_lexer* lexer, int ch);
bool etok_lexer_is_ident_continue(const etok_lexer* lexer, int ch);

void etok_lexer_skip_white_space(etok_lexer* lexer);
bool etok_lexer_try_consume_comment(etok_lexer* lexer);

int etok_utf8_decoder(void* userdata, const char* data, size_t data_count, int* stride);

bool etok_is_white_space(void* userdata, int ch);
bool etok_is_decimal_digit(void* userdata, int ch);
bool etok_is_common_ident_begin(void* userdata, int ch);
bool etok_is_common_ident_continue(void* userdata, int ch);

bool etok_try_consume_common_comment(etok_lexer* lexer);

void etok_print_error_to_stderr(void* userdata, const char* source_name, const char* source_text, const char* where, uint64_t line, uint64_t column, const char* format, ...);

#endif /* ECHOETOK_H_ */


#if !defined(ECHOETOK_IMPLEMENTATION_INCLUDED) && (defined(ECHOETOK_IMPL) || defined(ECHOETOK_IMPLEMENTATION))
#define ECHOETOK_IMPLEMENTATION_INCLUDED
#undef ECHOETOK_IMPL
#undef ECHOETOK_IMPLEMENTATION

#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void etok_lexer_init(etok_lexer* lexer, const char* source_name, const char* source_text, const char* source_text_end) {
    if (source_text_end == NULL) {
        source_text_end = source_text + strlen(source_text);
    }

    *lexer = (etok_lexer) {
        .source_name = source_name,
        .source_begin = source_text,
        .source_current = source_text,
        .source_end = source_text_end,
        .ch = 0,
        .stride = 0,
        .line = 1,
        .column = 1,
        .decoder = etok_utf8_decoder,
        .white_space_predicate = etok_is_white_space,
        .decimal_digit_predicate = etok_is_decimal_digit,
        .ident_begin_predicate = etok_is_common_ident_begin,
        .ident_continue_predicate = etok_is_common_ident_continue,
        .error_callback = etok_print_error_to_stderr,
        .comment_consumer = etok_try_consume_common_comment,
        .string_escape_ch = '\\',
    };
}

etok_token etok_lexer_read(etok_lexer* lexer) {
    if (lexer == NULL) return (etok_token) {0};

    /* Prime the first character. */
    if (lexer->ch == 0 && lexer->stride == 0 && lexer->source_current == lexer->source_begin) {
        etok_lexer_advance(lexer);
    }

    etok_lexer_skip_white_space(lexer);
    etok_token token = {
        .source_name = lexer->source_name,
        .begin = lexer->source_current,
        .line = lexer->line,
        .column = lexer->column,
    };

    if (etok_lexer_is_at_end(lexer)) {
        token.end = token.begin;
        return token;
    }

    int ch = lexer->ch;
    switch (ch) {
        case '"':
        case '\'': {
            token.kind = ch == '"' ? ETOK_DOUBLE_QUOTE_STRING : ETOK_SINGLE_QUOTE_STRING;

            int delim = ch;
            etok_lexer_advance(lexer);

            while (!etok_lexer_is_at_end(lexer) && lexer->ch != '\n' && lexer->ch != delim) {
                if (lexer->string_escape_ch > 0 && lexer->ch == lexer->string_escape_ch && etok_lexer_peek(lexer, 1) == delim) {
                    etok_lexer_advance(lexer);
                }
                etok_lexer_advance(lexer);
            }

            if (lexer->ch != delim) {
                etok_lexer_issue_error(lexer, token.begin, token.line, token.column, "Unterminated string constant.");
            } else etok_lexer_advance(lexer);
        } break;

        default: {
            if (etok_lexer_is_ident_begin(lexer, ch)) {
                token.kind = ETOK_IDENT;
                do {
                    etok_lexer_advance(lexer);
                } while (!etok_lexer_is_at_end(lexer) && etok_lexer_is_ident_continue(lexer, lexer->ch));
            } else if (etok_lexer_is_decimal_digit(lexer, ch)) {
                token.kind = ETOK_INTEGER;
                do {
                    etok_lexer_advance(lexer);
                } while (!etok_lexer_is_at_end(lexer) && etok_lexer_is_decimal_digit(lexer, lexer->ch));
            } else if (ch >= ETOK_PRINTABLE_BEGIN && ch <= ETOK_PRINTABLE_END) {
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

                token.kind = ETOK_INVALID_CHARACTER;
                etok_lexer_advance(lexer);
            }
        } break;
    }

    token.end = lexer->source_current;
    return token;
}

bool etok_lexer_is_at_end(const etok_lexer* lexer) {
    return lexer == NULL || lexer->source_current == NULL || lexer->source_current >= lexer->source_end;
}

void etok_lexer_advance(etok_lexer* lexer) {
    if (etok_lexer_is_at_end(lexer)) return;

    lexer->source_current += lexer->stride;

    size_t length = lexer->source_end - lexer->source_current;
    if (length == 0) return;

    int lastch = lexer->ch;
    int ch = lexer->ch = etok_lexer_decode_char(lexer, 0, &lexer->stride);
    if (ch == ETOK_CHAR_INVALID) {
        // TODO(local): Notify that an invalid character was returned or the stride is invalid.
        lexer->source_current = lexer->source_end;
        return;
    }

    if (lastch == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else if (lastch != 0) {
        lexer->column++;
    }
}

int etok_lexer_peek(const etok_lexer* lexer, int ahead) {
    if (ahead == 0) return lexer->ch;

    int ch = 0;
    size_t peek_offset = lexer->stride;

    for (int i = 1; i <= ahead; i++) {
        int stride = 0;
        ch = etok_lexer_decode_char(lexer, peek_offset, &stride);
        if (stride == 0) return 0;
        peek_offset += (size_t)stride;
    }

    return ch;
}

int etok_lexer_decode_char(const etok_lexer* lexer, size_t decode_offset, int* stride) {
    if (lexer == NULL || stride == NULL) goto return_error;

    const char* current = lexer->source_current + decode_offset;
    const char* end = lexer->source_end;
    if (current >= end) goto return_error;

    if (lexer->decoder == NULL) {
        *stride = 1;
        return *lexer->source_current;
    }

    size_t length = lexer->source_end - current;
    int ch = lexer->decoder(lexer->userdata, current, length, stride);
    if (ch == ETOK_CHAR_INVALID || *stride <= 0 || (size_t)(*stride) > length) {
        goto return_error;
    }

    return ch;

return_error:;
    if (stride != NULL) *stride = 0;
    return ETOK_CHAR_INVALID;
}

bool etok_lexer_is_white_space(const etok_lexer* lexer, int ch) {
    if (lexer == NULL) return false;
    if (lexer->white_space_predicate == NULL) {
        return etok_is_white_space(lexer->userdata, ch);
    }

    return lexer->white_space_predicate(lexer->userdata, ch);
}

bool etok_lexer_is_decimal_digit(const etok_lexer* lexer, int ch) {
    if (lexer == NULL) return false;
    if (lexer->decimal_digit_predicate == NULL) {
        return etok_is_decimal_digit(lexer->userdata, ch);
    }

    return lexer->decimal_digit_predicate(lexer->userdata, ch);
}

bool etok_lexer_is_ident_begin(const etok_lexer* lexer, int ch) {
    if (lexer == NULL) return false;
    if (lexer->ident_begin_predicate == NULL) {
        return etok_is_common_ident_begin(lexer->userdata, ch);
    }

    return lexer->ident_begin_predicate(lexer->userdata, ch);
}

bool etok_lexer_is_ident_continue(const etok_lexer* lexer, int ch) {
    if (lexer == NULL) return false;
    if (lexer->ident_continue_predicate == NULL) {
        return etok_is_common_ident_continue(lexer->userdata, ch);
    }

    return lexer->ident_continue_predicate(lexer->userdata, ch);
}

void etok_lexer_skip_white_space(etok_lexer* lexer) {
    while (!etok_lexer_is_at_end(lexer)) {
        if (etok_lexer_is_white_space(lexer, lexer->ch)) {
            etok_lexer_advance(lexer);
        } else if (etok_lexer_try_consume_comment(lexer)) {
            continue; // Explicitly continue on, we skipped a comment.
        } else break;
    }
}

bool etok_lexer_try_consume_comment(etok_lexer* lexer) {
    if (lexer == NULL || lexer->comment_consumer == NULL) {
        return false;
    }

    return lexer->comment_consumer(lexer);
}

int etok_utf8_decoder(void* userdata, const char* data, size_t data_count, int* stride) {
    // TODO(echoe): Actually decode UTF-8.
    *stride = 1;
    return *data;
}

bool etok_is_white_space(void* userdata, int ch) {
    return ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v';
}

bool etok_is_decimal_digit(void* userdata, int ch) {
    return ch >= '0' && ch <= '9';
}

bool etok_is_common_ident_begin(void* userdata, int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

bool etok_is_common_ident_continue(void* userdata, int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_';
}

bool etok_try_consume_common_comment(etok_lexer* lexer) {
    return false;
}

void etok_print_error_to_stderr(void* userdata, const char* source_name, const char* source_text, const char* where, uint64_t line, uint64_t column, const char* format, ...) {
    fprintf(stderr, "echoetok: error: %s(%"PRIi64",%"PRIi64"): ", source_name, line, column);
    va_list v;
    va_start(v, format);
    vfprintf(stderr, format, v);
    va_end(v);
    fputc('\n', stderr);
}

#endif /* ECHOETOK_IMPLEMENTATION_INCLUDED */
