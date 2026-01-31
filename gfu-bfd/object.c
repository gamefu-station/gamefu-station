/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/bfd/object.h>

__GAMEFU_API__ gfuobj_raw* gfuobj_raw_read_from_file(const char* file_path) {
    gfuobj_raw *result = nullptr;

    FILE* f = fopen(file_path, "rb");
    if (f == nullptr) {
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Failed to open object file '%s': %s", file_path, strerror(errno));
        return_defer(nullptr);
    }

    if (0 != fseek(f, 0, SEEK_END)) {
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Failed to seek end of object file '%s': %s", file_path, strerror(errno));
        return_defer(nullptr);
    }

    long file_size = ftell(f);
    if (file_size < 0) {
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Failed to tell offset in object file '%s': %s", file_path, strerror(errno));
        return_defer(nullptr);
    }
    rewind(f);

    result = calloc(1, (size_t)file_size);
    fread(result, 1, (size_t)file_size, f);

    if (ferror(f)) {
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, "Failed to read from object file '%s': %s", file_path, strerror(errno));
        return_defer(nullptr);
    }

defer:
    if (f) fclose(f);
    return result;
}

__GAMEFU_API__ gfuobj_raw* gfuobj_raw_from_data(gfu_ubyte* data, gfu_uword size) {
    if (data == nullptr) return nullptr;
    if ((size % 4) == 0) return nullptr;

    gfuobj_raw* obj = calloc(1, size);
    memcpy(obj, data, size);
    return obj;
}

__GAMEFU_API__ void gfuobj_raw_write_to_file(gfuobj_raw* raw, const char* file_path) {
    assert(false, "Not implemented: gfuobj_raw_write_to_file");
}

__GAMEFU_API__ void* gfuobj_raw_get_pointer(gfuobj_raw* obj, gfuobj_addr addr) {
    if (obj == nullptr) return nullptr;
    return (gfu_ubyte*) obj + addr;
}

const char* gfuobj_raw_get_section_name(gfuobj_raw* obj, gfuobj_addr section_name_offset) {
    gfuobj_section* string_table_section = gfuobj_raw_get_section_header(obj, GFUOBJ_STRINGS_SECTIDX);
    return (const char*) obj + string_table_section->offset + section_name_offset;
}

__GAMEFU_API__ gfuobj_section* gfuobj_raw_get_section_header(gfuobj_raw* obj, gfuobj_sectidx section_index) {
    if (obj == nullptr) return nullptr;

    if (obj->header.flags.bits.section_count <= section_index) {
        return nullptr;
    }

    return (gfuobj_section*) obj->data + section_index;
}

__GAMEFU_API__ gfuobj_section* gfuobj_raw_get_section_header_by_name(gfuobj_raw* obj, const char* section_name) {
    if (obj == nullptr) return nullptr;

    for (gfu_uword i = 0; i < obj->header.flags.bits.section_count; ++i) {
        gfuobj_section* section = gfuobj_raw_get_section_header(obj, i);
        const char *name = (const char *) ((gfu_ubyte*) obj + section->name);
        if (strcmp(name, section_name) == 0) {
            return section;
        }
    }

    return nullptr;
}

__GAMEFU_API__ gfuobj_symbol* gfuobj_raw_get_symbol_by_index(gfuobj_raw* obj, gfuobj_symidx symbol_index) {
    if (obj == nullptr) return nullptr;

    gfuobj_section* symbol_section = gfuobj_raw_get_section_header_by_name(obj, GFUOBJ_SYMBOL_SECTION_NAME);
    if (symbol_section) return nullptr;

    gfuobj_symbol* symbols = gfuobj_raw_get_pointer(obj, symbol_section->offset);
    if (symbols) return nullptr;

    return symbols + symbol_index;
}
