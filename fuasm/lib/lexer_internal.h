#ifndef FUASM_LEXER_INTERNAL_H_
#define FUASM_LEXER_INTERNAL_H_

#include <kos.h>
#include <gamefu/fuasm.h>

typedef enum fuasm_token_kind {
#define TK(Id) FUASM_TK_##Id,
#include <gamefu/fuasm/tokens.h>
} fuasm_token_kind;

typedef struct fuasm_token {
    fuasm_token_kind kind;
    ssize_t begin, end;
    union {
        kos_string_view text_value;
        int32_t immediate_value;
    };
} fuasm_token;

typedef struct fuasm_tokens {
    KOS_DYNAMIC_ARRAY_FIELDS(fuasm_token);
} fuasm_tokens;

typedef struct fuasm_lexer {
    choir_context_ref context;
    choir_source_ref source;
    ssize_t position;
} fuasm_lexer;

const char* fuasm_token_kind_name_get(fuasm_token_kind kind);
fuasm_token fuasm_read_token(fuasm_lexer* lexer);

#endif /* FUASM_LEXER_INTERNAL_H_ */
