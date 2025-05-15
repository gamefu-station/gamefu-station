#define KOS_IMPL
#include <kos.h>
#define GFU_ELF_IMPL
#include <gamefu/elf.h>

#include <choir/core.h>
#include <gamefu/fuasm.h>

static int emit_elf(fuasm_translation_unit* unit, const char* output_path);

int main(int argc, char** argv) {
    int result = 0;
    choir_context_ref context = nullptr;
    choir_source_ref source = nullptr;

    context = choir_context_create();

    if (argc < 2) {
        choir_diag_issue(context, CHOIR_ERROR, "No input file provided.");
        kos_return_defer(1);
    }

    if (argc < 3) {
        choir_diag_issue(context, CHOIR_ERROR, "No output file provided.");
        kos_return_defer(1);
    }

    const char* input_path = argv[1];
    const char* output_path = argv[2];

    source = choir_source_read_from_file(context, input_path, 0);
    if (source == nullptr) {
        //choir_diag_issue(context, CHOIR_ERROR, "No source file provided.");
        kos_return_defer(1);
    }

    fuasm_translation_unit unit = {
        .context = context,
        .source = source,
    };

    fuasm_assemble(&unit);
    emit_elf(&unit, output_path);

defer:;
    choir_context_destroy(context);
    return result;
}

static int emit_elf(fuasm_translation_unit* unit, const char* output_path) {
    int host_endian; {
        uint16_t endian_check = 0x0201;
        host_endian = *((uint8_t*)&endian_check);
    }

    elf32_header header = {
        .magic[0] = ELF_MAG0,
        .magic[1] = ELF_MAG1,
        .magic[2] = ELF_MAG2,
        .magic[3] = ELF_MAG3,
        .class = ELF_CLASS_ELF32,
        .endianness = host_endian,
        .version = 1,
        .abi = ELF_OSABI_GAMEFU,
        .abi_version = 1,
        .type = ELF_FILE_REL,
        .machine = ELF_MACHINE_MIPS,
        .version2 = 1,
        .entry = 0,
        .ph_offset = 0,
        .sh_offset = sizeof(elf32_header),
        .flags = 0,
        .header_size = sizeof(elf32_header),
        .ph_entry_size = 0,
        .ph_count = 0,
        .sh_entry_size = sizeof(elf32_section_header),
        .sh_count = 3,
        .sh_names_index = 1,
    };

    elf32_section_header header_null = {0};

    const char shnames_data[] = "\0.shstrtab\0.text";
    elf32_word shnames_size = sizeof(shnames_data);
    elf32_section_header header_shnames = {
        .name_index = 1,
        .type = ELF_SECT_STRTAB,
        .flags = 0,
        .virtual_address = 0,
        .offset = sizeof(elf32_header) + (header.sh_entry_size * header.sh_count),
        .size = shnames_size,
        .link = 0,
        .info = 0,
        .align = 1,
        .entry_size = 0,
    };

    const char* text_data = unit->section_text.data;
    elf32_word text_size = kos_cast(elf32_word) unit->section_text.size;
    elf32_section_header header_text = {
        .name_index = 11,
        .type = ELF_SECT_PROGBITS,
        .flags = ELF_SECTFLAG_EXECINSTR,
        .virtual_address = 0,
        .offset = kos_cast(elf32_word) kos_align_to(kos_cast(i64) (header_shnames.offset + header_shnames.size), 4) + (header.sh_entry_size * header.sh_count),
        .size = text_size,
        .link = 0,
        .info = 0,
        .align = 4,
        .entry_size = 0,
    };

    int result = 0;
    FILE* f = fopen(output_path, "wb");

    u64 padding = 0;
    fwrite(&header, sizeof(elf32_header), 1, f);
    fwrite(&header_null, sizeof(elf32_section_header), 1, f);
    fwrite(&header_shnames, sizeof(elf32_section_header), 1, f);
    fwrite(&header_text, sizeof(elf32_section_header), 1, f);
    fwrite(shnames_data, sizeof(shnames_data), 1, f);
    fwrite(&padding, (size_t) kos_align_padding(sizeof(shnames_data), 4), 1, f);
    fwrite(text_data, kos_cast(size_t) text_size, 1, f);

defer:;
    if (f != nullptr) fclose(f);
    return result;
}
