#ifndef GAMEFU_FUASM_H_
#define GAMEFU_FUASM_H_

#include <kos.h>
#include <choir/core.h>
#include <gamefu/elf.h>

typedef struct fuasm_translation_unit {
    choir_context_ref context;
    choir_source_ref source;
} fuasm_translation_unit;

void fuasm_assemble(fuasm_translation_unit* unit);

#endif /* GAMEFU_FUASM_H_ */
