#include <gamefu/prologue.h>

#include <fucc/asm_builder.h>

void fucc_asm_builder_init(fucc_asm_builder_t* b, fucc_context_t* c) {
    *b = (fucc_asm_builder_t) {
        .context = c,
    };
}

void fucc_asm_builder_deinit(fucc_asm_builder_t* b) {
    *b = (fucc_asm_builder_t) {0};
}

void fucc_asm_builder_enter_section(fucc_asm_builder_t* b, gfuobj_section_class_t section_class, const char* section_name, gfu_uword_t section_address, gfu_uword_t section_size) {
}

void fucc_asm_builder_define_label(fucc_asm_builder_t* b, const char* entity_name, fucc_asm_builder_mangling_t mangling) {
}

void fucc_build_bytes(fucc_asm_builder_t* b, gfu_uword_t count) {
}
