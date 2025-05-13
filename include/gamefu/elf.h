#ifndef GAMEFU_ELF_H_
#define GAMEFU_ELF_H_

#include <kos.h>

#define GFU_ELF_MAG0 0x7f
#define GFU_ELF_MAG1 0x45
#define GFU_ELF_MAG2 0x4C
#define GFU_ELF_MAG3 0x46

#define GFU_ELF_CLASS_32BIT 1
#define GFU_ELF_CLASS_64BIT 2

#define GFU_ELF_LITTLE_ENDIAN 1
#define GFU_ELF_BIT_ENDIAN 2

/// 1, the current version of ELF.
#define GFU_ELF_VERSION 1

/// Arbitrary GameFU Station ABI number.
#define GFU_ELF_ABI_GFU 0x22
/// Arbitrary GameFU Station ABI version number.
#define GFU_ELF_ABI_GFU_VERSION 0x23

/// Unknown file.
#define GFU_ELF_TYPE_NONE 0x0000
/// Relocatable file.
#define GFU_ELF_TYPE_REL  0x0001
/// Executable file.
#define GFU_ELF_TYPE_EXE  0x0002
/// Shared object file.
#define GFU_ELF_TYPE_DYN  0x0003
/// Core dump file.
#define GFU_ELF_TYPE_CORE 0x0004

#define GFU_ELF_MACHINE_MIPS 0x08

/// 1, the current version of ELF.
/// also ???
#define GFU_ELF_VERSION2 1

/// Program header table entry unused.
#define GFU_ELF_PT_NULL 0x00
/// Loadable segment.
#define GFU_ELF_PT_LOAD 0x01
/// Dynamic linking information.
#define GFU_ELF_PT_DYNAMIC 0x02
/// Interpreter information.
#define GFU_ELF_PT_INTERP 0x03
/// Auxiliary information.
#define GFU_ELF_PT_NOTE 0x04
/// Reserved?
#define GFU_ELF_PT_SHLIB 0x05
/// Segment containing program header table itself.
#define GFU_ELF_PT_PHDR 0x06
///	Thread-Local Storage template.
#define GFU_ELF_PT_TLS 0x07

/// Executable segment.
#define GFU_ELF_PF_X 0x01
/// Writeable segment.
#define GFU_ELF_PF_W 0x02
/// Readable segment.
#define GFU_ELF_PF_R 0x03

/// Section header table entry unused.
#define GFU_ELF_SHT_NULL 0x0
/// Program data.
#define GFU_ELF_SHT_PROGBITS 0x1
/// Symbol table.
#define GFU_ELF_SHT_SYMTAB 0x2
/// String table.
#define GFU_ELF_SHT_STRTAB 0x3
/// Relocation entries with addends.
#define GFU_ELF_SHT_RELA 0x4
/// Symbol hash table.
#define GFU_ELF_SHT_HASH 0x5
/// Dynamic linking information.
#define GFU_ELF_SHT_DYNAMIC 0x6
/// Notes.
#define GFU_ELF_SHT_NOTE 0x7
/// Program space with no data (bss).
#define GFU_ELF_SHT_NOBITS 0x8
/// Relocation entries, no addends.
#define GFU_ELF_SHT_REL 0x9
/// Reserved.
#define GFU_ELF_SHT_SHLIB 0x0A
/// Dynamic linker symbol table.
#define GFU_ELF_SHT_DYNSYM 0x0B
/// Array of constructors.
#define GFU_ELF_SHT_INIT_ARRAY 0x0E
/// Array of destructors.
#define GFU_ELF_SHT_FINI_ARRAY 0x0F
/// Array of pre-constructors.
#define GFU_ELF_SHT_PREINIT_ARRAY 0x10
/// Section group.
#define GFU_ELF_SHT_GROUP 0x11
/// Extended section indices.
#define GFU_ELF_SHT_SYMTAB_SHNDX 0x12
/// Number of defined types.
#define GFU_ELF_SHT_NUM 0x13

/// Writable.
#define GFU_ELF_SHF_WRITE 0x1
/// Occupies memory during execution.
#define GFU_ELF_SHF_ALLOC 0x2
/// Executable.
#define GFU_ELF_SHF_EXECINSTR 0x4
/// Might be merged.
#define GFU_ELF_SHF_MERGE 0x10
/// Contains null-terminated strings.
#define GFU_ELF_SHF_STRINGS 0x20
/// 'sh_info' contains SHT index.
#define GFU_ELF_SHF_INFO_LINK 0x40
/// Preserve order after combining.
#define GFU_ELF_SHF_LINK_ORDER 0x80
/// Non-standard OS specific handling required.
#define GFU_ELF_SHF_OS_NONCONFORMING 0x100
/// Section is member of a group.
#define GFU_ELF_SHF_GROUP 0x200
/// Section hold thread-local data.
#define GFU_ELF_SHF_TLS 0x400
/// OS-specific.
#define GFU_ELF_SHF_MASKOS 0x0FF00000
/// Processor-specific.
#define GFU_ELF_SHF_MASKPROC 0xF0000000

typedef struct gfu_elf_header {
    /// 0x7F followed by ELF(45 4c 46) in ASCII; these four bytes constitute the magic number.
    uint8_t magic[4];
    /// This byte is set to either 1 or 2 to signify 32- or 64-bit format, respectively.
    uint8_t class;
    /// This byte is set to either 1 or 2 to signify little or big endianness, respectively. This affects interpretation of multi-byte fields starting with offset 0x10.
    uint8_t endianness;
    /// Set to 1 for the original and current version of ELF.
    uint8_t version;
    /// Identifies the target operating system ABI. We will be using 0x13 probably? That or something greater.
    uint8_t abi;
    /// Further specifies the ABI version. Its interpretation depends on the target ABI. We will use a funny number, like 69 I guess.
    uint8_t abi_version;
    /// Reserved padding bytes. Currently unused. Should be filled with zeros and ignored when read.
    uint8_t _padding[7];
    /// Identifies object file type.
    uint16_t type;
    /// Specifies target instruction set architecture. We will use MIPS, 0x08.
    uint16_t machine;
    /// Set to 1 for the original version of ELF.
    uint32_t version2;
    /// This is the memory address of the entry point from where the process starts executing. If the file doesn't have an associated entry point, then this holds zero.
    uint32_t entry;
    /// Points to the start of the program header table. It usually follows the file header immediately following this one, making the offset 0x34 for an executable.
    uint32_t ph_offset;
    /// Points to the start of the section header table.
    uint32_t sh_offset;
    /// Interpretation of this field depends on the target architecture.
    uint32_t flags;
    /// Contains the size of this header, 52 bytes.
    uint16_t header_size;
    /// Contains the size of a program header table entry. This will be 0x20.
    uint16_t ph_entry_size;
    /// Contains the number of entries in the program header table.
    uint16_t ph_count;
    /// Contains the size of a section header table entry. This will be 0x28.
    uint16_t sh_entry_size;
    /// Contains the number of entries in the section header table.
    uint16_t sh_count;
    /// Contains index of the section header table entry that contains the section names.
    uint16_t sh_names_index;
} gfu_elf_header;

typedef struct gfu_elf_ph_entry {
    /// Identifies the type of the segment.
    uint32_t type;
    /// Offset of the segment in the file image.
    uint32_t offset;
    /// Virtual address of the segment in memory.
    uint32_t virtual_address;
    /// On systems where the physical address is relevant, reserved for segment's physical address.
    uint32_t physical_address;
    /// Size in bytes of the segment in the file image. May be 0.
    uint32_t file_size;
    /// Size in bytes of the segment in memory. May be 0.
    uint32_t memory_size;
    /// Segment-dependent flags.
    uint32_t flags;
    /// 0 and 1 specify no alignment. Otherwise should be a positive, integral power of 2, with virtual_address equating physical_address modulus align.
    uint32_t align;
} gfu_elf_ph_entry;

typedef struct gfu_elf_sh_entry {
    /// An offset to a string in the .shstrtab section (index gfu_elf_header.sh_names_index) that represents the name of this section.
    uint32_t name_index;
    /// Identifies the type of this header.
    uint32_t type;
    /// Identifies the attributes of the section.
    uint32_t flags;
    /// Virtual address of the section in memory, for sections that are loaded.
    uint32_t virtual_address;
    /// Offset of the section in the file image.
    uint32_t offset;
    /// Size in bytes of the section. May be 0.
    uint32_t size;
    /// Contains the section index of an associated section. This field is used for several purposes, depending on the type of section.
    uint32_t link;
    /// Contains extra information about the section. This field is used for several purposes, depending on the type of section.
    uint32_t info;
    /// Contains the required alignment of the section. This field must be a power of two.
    uint32_t align;
    /// Contains the size, in bytes, of each entry, for sections that contain fixed-size entries. Otherwise, this field contains zero.
    uint32_t entry_size;
} gfu_elf_sh_entry;

static_assert(sizeof(gfu_elf_header) == 52);
static_assert(sizeof(gfu_elf_ph_entry) == 0x20);
static_assert(sizeof(gfu_elf_sh_entry) == 0x28);

typedef struct gfu_elf_raw {
    gfu_elf_header header;
    gfu_elf_ph_entry* segments;
    gfu_elf_sh_entry* sections;
    uint32_t data_size;
    char* file_data;
} gfu_elf_raw;

typedef struct gfu_elf_segment {
    uint32_t offset;

    uint32_t type;
    uint32_t virtual_address;
    uint32_t physical_address;
    uint32_t memory_size;
    uint32_t flags;
    uint32_t align;

    uint32_t data_size;
    char* data;
} gfu_elf_segment;

typedef struct gfu_elf_section {
    uint32_t offset;

    const char* name;
    uint32_t type;
    uint32_t flags;
    uint32_t virtual_address;
    uint32_t link;
    uint32_t info;
    uint32_t align;

    uint32_t data_size;
    char* data;
} gfu_elf_section;

typedef struct gfu_elf {
    gfu_elf_raw raw;

    uint8_t endianness;
    uint32_t flags;
    uint32_t entry;

    uint32_t segment_count;
    gfu_elf_segment* segments;

    uint32_t section_count;
    gfu_elf_section* sections;
} gfu_elf;

bool gfu_elf_from_file(gfu_elf* elf, FILE* f);
bool gfu_elf_from_bytes(gfu_elf* elf, char* data, size_t size);
void gfu_elf_free(gfu_elf* elf);
void gfu_elf_debug_print(gfu_elf* elf);

#endif /* GAMEFU_ELF_H_ */


#if defined(GFU_ELF_IMPL) || defined(GFU_ELF_IMPLEMENTATION)
#undef GFU_ELF_IMPL
#undef GFU_ELF_IMPLEMENTATION

bool gfu_elf_from_file(gfu_elf* elf, FILE* f) {
    if (0 != fseek(f, 0, SEEK_END)) {
        return false;
    }

    int64_t file_size = ftell(f);
    if (file_size < 0) {
        return false;
    }

    if (0 != fseek(f, 0, SEEK_SET)) {
        return false;
    }

    char* data = calloc(1, (size_t)file_size);
    size_t read_count = fread(data, (size_t)file_size, 1, f);

    if (ferror(f)) {
        return false;
    }

    return gfu_elf_from_bytes(elf, data, (size_t)file_size);
}

bool gfu_elf_from_bytes(gfu_elf* elf, char* data, size_t size) {
    char* data_start = data;
    const char* data_end = data + size;

    // TODO(local): define actual implementation for ELFTOHOST2 and ELFTOHOST4
#define ELFTOHOST2(Value) (Value)
#define ELFTOHOST4(Value) (Value)
#define READ1() (assert(data < data_end), data += 1, *(data - 1))
#define READ2() (assert(data < data_end), data += 2, ELFTOHOST2(*(uint16_t*)(data - 2)))
#define READ4() (assert(data < data_end), data += 4, ELFTOHOST4(*(uint32_t*)(data - 4)))

    *elf = (gfu_elf){0};
    elf->raw.data_size = (uint32_t)size;
    elf->raw.file_data = data;

    gfu_elf_header* header = &elf->raw.header;
    header->magic[0] = READ1();
    header->magic[1] = READ1();
    header->magic[2] = READ1();
    header->magic[3] = READ1();
    header->class = READ1();
    header->endianness = READ1();
    header->version = READ1();
    header->abi = READ1();
    header->abi_version = READ1();
    data += 7; // padding
    header->type = READ2();
    assert(header->type == GFU_ELF_TYPE_REL);
    header->machine = READ2();
    header->version2 = READ4();
    header->entry = READ4();
    header->ph_offset = READ4();
    header->sh_offset = READ4();
    header->flags = READ4();
    header->header_size = READ2();
    assert(header->header_size == sizeof(gfu_elf_header));
    header->ph_entry_size = READ2();
    assert(header->ph_entry_size == 0 || header->ph_entry_size == sizeof(gfu_elf_ph_entry));
    header->ph_count = READ2();
    header->sh_entry_size = READ2();
    assert(header->sh_entry_size == 0 || header->sh_entry_size == sizeof(gfu_elf_sh_entry));
    header->sh_count = READ2();
    header->sh_names_index = READ2();

    elf->raw.segments = calloc((size_t)header->ph_count, sizeof *elf->raw.segments);
    elf->raw.sections = calloc((size_t)header->sh_count, sizeof *elf->raw.sections);

    for (uint32_t i = 0; i < header->ph_count; i++) {
        data = data_start + header->ph_offset + (i * header->ph_entry_size);
        gfu_elf_ph_entry* entry = &elf->raw.segments[i];
        entry->type = READ4();
        entry->offset = READ4();
        entry->virtual_address = READ4();
        entry->physical_address = READ4();
        entry->file_size = READ4();
        entry->memory_size = READ4();
        entry->flags = READ4();
        entry->align = READ4();
    }

    for (uint32_t i = 0; i < header->sh_count; i++) {
        data = data_start + header->sh_offset + (i * header->sh_entry_size);
        gfu_elf_sh_entry* entry = &elf->raw.sections[i];
        entry->name_index = READ4();
        entry->type = READ4();
        entry->flags = READ4();
        entry->virtual_address = READ4();
        entry->offset = READ4();
        entry->size = READ4();
        entry->link = READ4();
        entry->info = READ4();
        entry->align = READ4();
        entry->entry_size = READ4();
    }

    // transform the raw

    elf->endianness = header->endianness;
    elf->flags = header->flags;
    elf->entry = header->entry;
    elf->segment_count = header->ph_count;
    elf->segments = calloc((size_t)elf->segment_count, sizeof *elf->segments);
    elf->section_count = header->sh_count;
    elf->sections = calloc((size_t)elf->section_count, sizeof *elf->sections);

    const char* name_table;
    {
        gfu_elf_sh_entry* name_entry = &elf->raw.sections[elf->raw.header.sh_names_index];
        name_table = data_start + name_entry->offset;
    }

    for (uint32_t i = 0; i < elf->segment_count; i++) {
        gfu_elf_ph_entry* entry = &elf->raw.segments[i];
        gfu_elf_segment* segment = &elf->segments[i];

        segment->offset = entry->offset;

        segment->data_size = entry->file_size;
        segment->data = data_start + entry->offset;
    }

    for (uint32_t i = 0; i < elf->section_count; i++) {
        gfu_elf_sh_entry* entry = &elf->raw.sections[i];
        gfu_elf_section* section = &elf->sections[i];

        section->name = &name_table[entry->name_index];

        section->data_size = entry->size;
        section->data = data_start + entry->offset;
    }

#undef READ4
#undef READ2
#undef READ1
#undef ELFTOHOST4
#undef ELFTOHOST2

    return true;
}

void gfu_elf_free(gfu_elf* elf) {
    free(elf->raw.file_data);
    free(elf->raw.segments);
    free(elf->raw.sections);
    free(elf->segments);
    free(elf->sections);
}

void gfu_elf_debug_print(gfu_elf* elf) {
    fprintf(stderr, "ELF Header:\n");

    fprintf(stderr, "  Magic:  ");
    for (int i = 0; i < 4; i++) {
        fprintf(stderr, " %02X", elf->raw.header.magic[i]);
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "Class:                             %u\n", elf->raw.header.class);
    fprintf(stderr, "Data:                              %u\n", elf->raw.header.endianness);
    fprintf(stderr, "Version:                           %u\n", elf->raw.header.version);
    fprintf(stderr, "OS/ABI:                            %u\n", elf->raw.header.abi);
    fprintf(stderr, "ABI Version:                       %u\n", elf->raw.header.abi_version);
    fprintf(stderr, "Type:                              %u\n", elf->raw.header.type);
    fprintf(stderr, "Machine:                           %u\n", elf->raw.header.machine);
    fprintf(stderr, "Version:                           0x%02X\n", elf->raw.header.version2);
    fprintf(stderr, "Entry point address:               0x%04X\n", elf->raw.header.entry);
    fprintf(stderr, "Start of program headers:          0x%08X (%u bytes into file)\n", elf->raw.header.ph_offset, elf->raw.header.ph_offset);
    fprintf(stderr, "Start of section headers:          0x%08X (%u bytes into file)\n", elf->raw.header.sh_offset, elf->raw.header.sh_offset);
    fprintf(stderr, "Flags:                             0x%08X\n", elf->raw.header.flags);
    fprintf(stderr, "Size of this header:               %u (bytes)\n", elf->raw.header.header_size);
    fprintf(stderr, "Size of program headers:           %u (bytes)\n", elf->raw.header.ph_entry_size);
    fprintf(stderr, "Number of program headers:         %u\n", elf->raw.header.ph_count);
    fprintf(stderr, "Size of section headers:           %u (bytes)\n", elf->raw.header.sh_entry_size);
    fprintf(stderr, "Number of section headers:         %u\n", elf->raw.header.sh_count);
    fprintf(stderr, "Section header string table index: %u\n", elf->raw.header.sh_names_index);

    fprintf(stderr, "\nSegment Headers:\n");

    fprintf(stderr, "\nSection Headers:\n");

    fprintf(stderr, "  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    const char* name_table = elf->sections[elf->raw.header.sh_names_index].data;
    for (uint32_t i = 0; i < elf->section_count; i++) {
        int nleft;
        fprintf(stderr, "  [%2u] ", i);
        nleft = fprintf(stderr, "%s", &name_table[elf->raw.sections[i].name_index]);
        for (int j = 0; j < 18 - nleft; j++) fputc(' ', stderr);
        nleft = fprintf(stderr, "%02X", elf->raw.sections[i].type);
        for (int j = 0; j < 16 - nleft; j++) fputc(' ', stderr);
        fprintf(stderr, "%08X ", elf->raw.sections[i].virtual_address);
        fprintf(stderr, "%06X ", elf->raw.sections[i].offset);
        fprintf(stderr, "%06X ", elf->raw.sections[i].size);
        fprintf(stderr, "%02X ", elf->raw.sections[i].entry_size);
        fprintf(stderr, "\n");
    }
}

#endif /* GFU_ELF_IMPL */
