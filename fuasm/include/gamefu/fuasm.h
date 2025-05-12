#ifndef GAMEFU_FUASM_H_
#define GAMEFU_FUASM_H_

#include <kos.h>
#include <choir/core.h>

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

typedef struct fuasm_lexer {
    choir_context_ref context;
    choir_source_ref source;
    ssize_t position;
} fuasm_lexer;

typedef struct fuasm_translation_unit {
    choir_context_ref context;
    choir_source_ref source;
} fuasm_translation_unit;

const char* fuasm_token_kind_name_get(fuasm_token_kind kind);
fuasm_token fuasm_read_token(fuasm_lexer* lexer);
void guasm_assemble();

#endif /* GAMEFU_FUASM_H_ */
