/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/common.h>
#include <gamefu/bfd/object.h>
#include <gamefu/hx.h>

#include <stdio.h>

typedef struct {
    bool show_help;
    bool show_header;
    bool show_section_headers;
    bool show_section_data;
    const char* obj_file_path;
} readobj_options;

static const char *readobj_class_to_cstr(gfuobj_section_class class);
static void readobj_print_help(const char* program_name);
static bool readobj_parse_flags(int argc, char** argv, readobj_options* options);

int main(int argc, char** argv) {
    int result = 0;
    gfuobj_raw *obj = nullptr;

    readobj_options options = {0};
    if (!readobj_parse_flags(argc, argv, &options)) {
        readobj_print_help(argv[0]);
        return_defer(1);
    }

    if (options.show_help) {
        readobj_print_help(argv[0]);
        return_defer(0);
    }

    const char* obj_path = options.obj_file_path;
    obj = gfuobj_raw_read_from_file(obj_path);
    if (obj == nullptr) return_defer(1);

    if (!GFUOBJ_VERIFY_MAGIC(obj->header.magic)) {
        diag_issue(DIAG_ERROR, NOSOURCE, "Provided file is not a GameFU object.");
        return_defer(1);
    }

    if (options.show_header) {
        gfuobj_header* header = &obj->header;
        printf("GameFU Object Header:\n");
        printf("  Magic:         ");
        for (gfu_uword i = 0; i < sizeof(header->magic); ++i) {
            printf(" %02X", ((unsigned char*)&header->magic)[i]);
        }
        printf("\n");
        printf("  Version:        %u\n", GFUOBJ_GET_VERSION(obj->header.magic));
        printf("  Section count:  %u\n", header->flags.bits.section_count);
        printf("  RAM size:       %s\n", (header->flags.bits.ram_size) ? "Large" : "Small");
        printf("  Entry address:  0x%08X\n", header->entry_address);
        printf("  ROM size:       %u bytes\n", header->rom_size);
    }

    if (options.show_section_headers) {
        gfu_uword size_column_len = 0;
        for (gfu_uword i = 0; i < obj->header.flags.bits.section_count; ++i) {
            gfuobj_section* section = gfuobj_raw_get_section_header(obj, i);
            gfu_uword size = snprintf(nullptr, 0, "%u", section->size);
            if (size > size_column_len) size_column_len = size;
        }

        printf("GameFU Object Section Headers:\n");
        printf("  Num:   Offset  %*s  Class  Name\n", size_column_len, "Size");
        for (gfu_uword i = 0; i < obj->header.flags.bits.section_count; ++i) {
            gfuobj_section* section = gfuobj_raw_get_section_header(obj, i);
            const char *class_name = readobj_class_to_cstr(section->class);
            const char *section_name = gfuobj_raw_get_section_name(obj, section->name);
            printf("%5u: %08X  %*u   %s  %s\n", i, section->offset, size_column_len, section->size, class_name, section_name);
        }
    }

    if (options.show_section_data) {
        printf("GameFU Object Section Data:\n");
        for (gfu_uword i = 0; i < obj->header.flags.bits.section_count; ++i) {
            gfuobj_section* section = gfuobj_raw_get_section_header(obj, i);
            const char *section_name = gfuobj_raw_get_section_name(obj, section->name);
            printf("  Section %u: %s\n", i, section_name);

            if (section->size <= 0) {
                printf("Empty\n");
                continue;
            }

            hx_opts opts = hx_default_opts();
            opts.visual_offset = section->offset;
            hx_dump_opt(stdout, gfuobj_raw_get_pointer(obj, section->offset), section->size, opts);
        }
    }

defer:;
    free(obj);
    diag_flush();
    return result;
}

static const char *readobj_class_to_cstr(gfuobj_section_class class) {
    switch (class) {
    case GFUOBJ_CLASS_META: return "META";
    case GFUOBJ_CLASS_DATA: return "DATA";
    case GFUOBJ_CLASS_TEXT: return "TEXT";
    case GFUOBJ_CLASS_BSS:  return "BSS ";
    default: assert(false, "Non-exhaustive handling of gfuobj_section_class");
    }
}

static bool readobj_parse_flags(int argc, char** argv, readobj_options* options) {
    bool result = true;

    for (int i = 1; i < argc; i++) {
        const char* option = argv[i];

        if (*option != '-') {
            if (options->obj_file_path != nullptr) {
                diag_issue(DIAG_ERROR, NOSOURCE, "Cannot provide more than one object file.");
                return_defer(false);
            }
            options->obj_file_path = option;
            continue;
        }

        if (strcmp(option, "--help") == 0 || strcmp(option, "-h") == 0) {
            options->show_help = true;
        } else if (strcmp(option, "-H") == 0) {
            options->show_header = true;
        } else if (strcmp(option, "-s") == 0) {
            options->show_section_headers = true;
        } else if (strcmp(option, "-d") == 0) {
            options->show_section_data = true;
        } else {
            diag_issue(DIAG_ERROR, NOSOURCE, "Unknown flag: '%s'.", option);
            return_defer(false);
        }
    }

    if (options->obj_file_path == nullptr) {
        diag_issue(DIAG_ERROR, NOSOURCE, "No object file provided.");
        return_defer(false);
    }

defer:
    diag_flush();
    return result;
}

static void readobj_print_help(const char* program_name) {
    fprintf(
        stderr,
        "readobj -- Display information about a GFU object file.\n"
        "Usage: %s [options...] <file>\n"
        "Options:\n"
        "  -h, --help      Display this help message\n"
        "  -H              Display the header of the object file\n"
        "  -s              Display the section headers of the object file\n"
        "  -d              Display the section data of the object file\n",
        program_name
    );
}
