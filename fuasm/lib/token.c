#include "lexer_internal.h"

const char* fuasm_token_kind_name_get(fuasm_token_kind kind) {
    switch (kind) {
        default: return "<unknown-fuasm-token>";
#define TK(Id) case FUASM_TK_##Id: return #Id;
#include <gamefu/fuasm/tokens.h>
    }
}
