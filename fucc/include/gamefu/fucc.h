#ifndef FUCC_H_
#define FUCC_H_

#include <kos.h>
#include <choir/core.h>

typedef enum fucc_token_kind {
#define TK(id, ...) FUCC_TK_##id,
#include <gamefu/fucc/tokens.h>
} fucc_token_kind;

typedef struct fucc_token {
    fucc_token_kind kind;
} fucc_token;

typedef struct fucc_translation_unit {
    choir_context_ref context;
    choir_source_ref source;
} fucc_translation_unit;

#endif /* FUCC_H_ */
