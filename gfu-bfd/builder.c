#include <gamefu/bfd/object.h>

static gfuobj_addr gfuobj_get_string_index(gfuobj_builder* builder, const char* string) {
    assertn(builder != nullptr);
    assertn(GFUOBJ_STRINGS_SECTIDX < builder->count);

    size_t string_length = strlen(string);

    gfuobj_byte_builder* strdat_builder = &builder->items[GFUOBJ_STRINGS_SECTIDX].data;
    char* strdat = (char*) strdat_builder->items;
    char* strdat_end = strdat + strdat_builder->count;

    for (char* search = strdat; search < strdat_end; ) {
        gfu_uword search_length = strlen(search);
        if (0 == strcmp(search, string)) return (gfuobj_addr) (search - strdat);
        search += search_length + 1;
    }

    // unreachable;
    fprintf(stderr, "missing string: \"%s\"\n", string);
    assert(false, "Should only be looking up strings which were already interned.");
}

GAMEFU_API gfuobj_raw* gfuobj_builder_to_raw(gfuobj_builder* builder) {
    gfu_uword raw_size = sizeof(gfuobj_header) + (sizeof(gfuobj_section) * builder->count);
    assertn(raw_size == GFUOBJ_ALIGN(raw_size));

    gfu_uword section_offsets[builder->count];
    // NOTE(local): calculating the size of this is not as easy as just summing section builder lengths, because symbols need to translate their string point to an index.
    // We manually re-calculate the size of the symbol section here, and other sections might need the same treatment eventually.
    for (gfu_uword i = 0; i < builder->count; raw_size = GFUOBJ_ALIGN(raw_size), i++) {
        section_offsets[i] = raw_size;
        if (i == GFUOBJ_SYMBOL_SECTIDX) {
            raw_size += sizeof(gfuobj_symbol) * (builder->items[i].data.count / sizeof(gfuobj_symbol_builder));
        } else {
            raw_size += builder->items[i].data.count;
        }
    }

    assertn(raw_size == GFUOBJ_ALIGN(raw_size));
    gfuobj_raw* raw = calloc(1, raw_size);

    gfuobj_header* header = (void*) raw;
    header->magic = GFUOBJ_MAGIC_VERSION(GFUOBJ_CURRENT_VERSION);
    memcpy(&header->flags, &builder->flags, sizeof(gfuobj_flags));
    header->entry_address = builder->entry_address;
    header->rom_size = raw_size;

    gfu_uword current_offset = sizeof(gfuobj_header) + (sizeof(gfuobj_section) * builder->count);
    for (gfu_uword i = 0; i < builder->count; current_offset = GFUOBJ_ALIGN(current_offset), i++) {
        gfuobj_section_builder* section = &builder->items[i];

        gfuobj_section* section_header = (void*) ((char*) raw + sizeof(gfuobj_header) + (i * sizeof(gfuobj_section)));
        *section_header = (gfuobj_section) {
            .name = gfuobj_get_string_index(builder, section->name),
            .offset = current_offset,
            .size = section->data.count,
            .class = section->class,
            .virtual_address = section->virtual_address,
        };

        void* section_ptr = ((char*) raw) + section_offsets[i];

        if (i == GFUOBJ_SYMBOL_SECTIDX) {
            gfuobj_symbol* data_ptr = section_ptr;
            for (gfu_uword j = 0, j_count = section->data.count / sizeof(gfuobj_symbol_builder); j < j_count; j++) {
                gfuobj_symbol_builder* symbol_builder = &(((gfuobj_symbol_builder*) section->data.items)[j]);
                data_ptr[j] = (gfuobj_symbol) {
                    .name = gfuobj_get_string_index(builder, symbol_builder->name),
                    .info.raw = symbol_builder->info.raw,
                    .section = symbol_builder->section,
                    .offset = symbol_builder->offset,
                    .size = symbol_builder->size,
                };
            }
        } else {
            memcpy(section_ptr, section->data.items, section->data.count);
        }

        current_offset += section->data.count;
    }

    return raw;
}

GAMEFU_API void gfuobj_builder_init(gfuobj_builder* builder) {
    *builder = (gfuobj_builder) {0};

    gfuobj_sectidx nul_idx = gfuobj_builder_add_section(builder, GFUOBJ_NUL_SECTION_NAME);
    assertn(nul_idx == GFUOBJ_NUL_SECTIDX);

    gfuobj_sectidx str_idx = gfuobj_builder_add_section(builder, GFUOBJ_STRINGS_SECTION_NAME);
    assertn(str_idx == GFUOBJ_STRINGS_SECTIDX);

    gfuobj_section_builder* string_section = gfuobj_builder_get_section(builder, GFUOBJ_STRINGS_SECTIDX);
    gfu_da_push(&string_section->data, 0);

    (void) gfuobj_builder_intern_string(builder, GFUOBJ_NUL_SECTION_NAME);
    (void) gfuobj_builder_intern_string(builder, GFUOBJ_STRINGS_SECTION_NAME);

    gfuobj_sectidx rel_idx = gfuobj_builder_add_section(builder, GFUOBJ_RELOCATION_SECTION_NAME);
    assertn(rel_idx == GFUOBJ_RELOCATION_SECTIDX);

    gfuobj_sectidx sym_idx = gfuobj_builder_add_section(builder, GFUOBJ_SYMBOL_SECTION_NAME);
    assertn(sym_idx == GFUOBJ_SYMBOL_SECTIDX);
}

GAMEFU_API void gfuobj_builder_deinit(gfuobj_builder* builder) {
    for (gfu_uword i = 0; i < builder->count; i++) {
        gfu_da_free(&builder->items[i].data);
    }

    gfu_da_free(builder);
    *builder = (gfuobj_builder) {0};
}

GAMEFU_API gfuobj_addr gfuobj_builder_intern_string(gfuobj_builder* builder, const char* string) {
    if (builder == nullptr || GFUOBJ_STRINGS_SECTIDX >= builder->count) {
        return GFUOBJ_ADDR_INVALID;
    }

    size_t string_length = strlen(string);

    gfuobj_byte_builder* strdat_builder = &builder->items[GFUOBJ_STRINGS_SECTIDX].data;
    char* strdat = (char*) strdat_builder->items;
    char* strdat_end = strdat + strdat_builder->count;

    for (char* search = strdat; search < strdat_end; ) {
        gfu_uword search_length = strlen(search);
        if (0 == strcmp(search, string)) return (gfuobj_addr) (search - strdat);
        search += search_length + 1;
    }

    gfu_uword string_addr = strdat_builder->count;
    gfu_da_reserve(strdat_builder, strdat_builder->count + string_length + 1);

    // + 1 because this will be NUL terminated, ensure the terminator is there.
    memcpy((char*) strdat_builder->items + strdat_builder->count, string, string_length + 1);
    strdat_builder->count += string_length + 1;

    return string_addr;
}

GAMEFU_API gfuobj_sectidx gfuobj_builder_add_section(gfuobj_builder* builder, const char* section_name) {
    if (builder == nullptr || section_name == nullptr) {
        return GFUOBJ_SECTIDX_INVALID;
    }

    for (gfu_uword i = 0; i < builder->count; i++) {
        if (0 == strcmp(section_name, builder->items[i].name)) {
            return (gfuobj_sectidx) i;
        }
    }

    (void) gfuobj_builder_intern_string(builder, section_name);

    gfuobj_section_builder sect_builder = {
        .name = section_name,
    };

    gfuobj_sectidx idx = builder->count;
    gfu_da_push(builder, sect_builder);

    builder->flags.bits.section_count = builder->count;

    return idx;
}

GAMEFU_API gfuobj_section_builder* gfuobj_builder_get_section(gfuobj_builder* builder, gfuobj_sectidx section_index) {
    if (builder == nullptr || section_index == GFUOBJ_SECTIDX_INVALID || section_index >= builder->count) {
        return nullptr;
    }

    return &builder->items[section_index];
}

GAMEFU_API gfuobj_symbol_builder* gfuobj_builder_get_symbol(gfuobj_builder* builder, gfuobj_symidx symbol_index) {
    if (builder == nullptr || builder->count <= GFUOBJ_SYMBOL_SECTIDX || symbol_index == GFUOBJ_SYMIDX_INVALID) {
        return nullptr;
    }

    gfuobj_section_builder* symbol_section = gfuobj_builder_get_section(builder, GFUOBJ_SYMBOL_SECTIDX);
    assertn(symbol_section != nullptr);

    if (symbol_section->data.capacity < symbol_index * sizeof(gfuobj_symbol_builder)) {
        return nullptr;
    }

    return &((gfuobj_symbol_builder*) symbol_section->data.items)[symbol_index];
}

GAMEFU_API gfuobj_relocation_builder* gfuobj_builder_get_relocation(gfuobj_builder* builder, gfuobj_relidx relocation_index) {
    if (builder == nullptr || builder->count <= GFUOBJ_RELOCATION_SECTIDX || relocation_index == GFUOBJ_RELIDX_INVALID) {
        return nullptr;
    }

    gfuobj_section_builder* relocation_section = gfuobj_builder_get_section(builder, GFUOBJ_RELOCATION_SECTIDX);
    assertn(relocation_section != nullptr);

    if (relocation_section->data.capacity < relocation_index * sizeof(gfuobj_relocation_builder)) {
        return nullptr;
    }

    return &((gfuobj_relocation_builder*) relocation_section->data.items)[relocation_index];

}

GAMEFU_API gfuobj_addr gfuobj_byte_builder_push_word(gfuobj_byte_builder* byte_builder, gfu_uword word) {
    if (byte_builder == nullptr) {
        return GFUOBJ_ADDR_INVALID;
    }

    gfuobj_addr address = byte_builder->count;

    gfu_da_reserve(byte_builder, byte_builder->count + sizeof(word));
    ((gfu_uword*)byte_builder->items)[address / sizeof(word)] = word;

    byte_builder->count += sizeof(word);
    return address;
}

GAMEFU_API gfuobj_symidx gfuobj_builder_push_symbol(gfuobj_builder* builder, const char* symbol_name) {
    if (builder == nullptr) {
        return GFUOBJ_SYMIDX_INVALID;
    }

    gfuobj_section_builder* section = gfuobj_builder_get_section(builder, GFUOBJ_SYMBOL_SECTIDX);
    if (section == nullptr) {
        return GFUOBJ_SYMIDX_INVALID;
    }

    gfuobj_byte_builder* byte_builder = &section->data;
    gfuobj_symidx index = byte_builder->count / sizeof(gfuobj_symbol_builder);

    gfu_da_reserve(byte_builder, byte_builder->count + sizeof(gfuobj_symbol_builder));

    (void) gfuobj_builder_intern_string(builder, symbol_name);
    ((gfuobj_symbol_builder*)byte_builder->items)[index] = (gfuobj_symbol_builder) {
        .name = symbol_name,
    };

    byte_builder->count += sizeof(gfuobj_symbol_builder);
    return index;
}

GAMEFU_API gfuobj_relidx gfuobj_builder_push_relocation(gfuobj_builder* builder) {
    if (builder == nullptr) {
        return GFUOBJ_SYMIDX_INVALID;
    }

    gfuobj_section_builder* section = gfuobj_builder_get_section(builder, GFUOBJ_SYMBOL_SECTIDX);
    if (section == nullptr) {
        return GFUOBJ_SYMIDX_INVALID;
    }

    gfuobj_byte_builder* byte_builder = &section->data;
    gfuobj_relidx index = byte_builder->count / sizeof(gfuobj_relocation);

    gfu_da_reserve(byte_builder, byte_builder->count + sizeof(gfuobj_relocation));
    ((gfuobj_relocation*)byte_builder->items)[index] = (gfuobj_relocation) {0};

    byte_builder->count += sizeof(gfuobj_relocation);
    return index;
}
