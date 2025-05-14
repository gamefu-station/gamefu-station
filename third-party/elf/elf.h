#ifndef ELF_SINGLE_H_
#define ELF_SINGLE_H_

#include <stdint.h>

#define ELF_MAG0 (0x7F)
#define ELF_MAG1 ('E')
#define ELF_MAG2 ('L')
#define ELF_MAG3 ('F')

#define ELF_SEGMENT_TYPES(X) \
    X(NULL, 0) \
    X(LOAD, 1) \
    X(DYNAMIC, 2) \
    X(INTERP, 3) \
    X(NOTE, 4) \
    X(SHLIB, 5) \
    X(PHDR, 6) \
    X(TLS, 7) \
    X(LOOS, 0x60000000) \
    X(HIOS, 0x6FFFFFFF) \
    X(LOPROC, 0x70000000) \
    X(HIPROC, 0x7FFFFFFF)

typedef enum elf_segment_type {
#define X(id, val) ELF_SEG_##id = (val),
ELF_SEGMENT_TYPES(X)
#undef X
} elf_segment_type;

#define ELF_SEGMENT_FLAGS(X) \
    X(X, 0x01) \
    X(W, 0x02) \
    X(R, 0x04)

typedef enum elf_segment_flag {
#define X(id, val) ELF_SEGFLAG_##id = (val),
ELF_SEGMENT_FLAGS(X)
#undef X
} elf_segment_flag;

#define ELF_SECTION_TYPES(X) \
    X(NULL, 0) \
    X(PROGBITS, 1) \
    X(SYMTAB, 2) \
    X(STRTAB, 3) \
    X(RELA, 4) \
    X(HASH, 5) \
    X(DYNAMIC, 6) \
    X(NOTE, 7) \
    X(NOBITS, 8) \
    X(REL, 9) \
    X(SHLIB, 10) \
    X(DYNSYM, 11) \
    X(NUM, 12) \
    X(LOPROC, 0x70000000) \
    X(HIPROC, 0x7FFFFFFF) \
    X(LOUSER, 0x80000000) \
    X(HIUSER, 0xFFFFFFFF)

typedef enum elf_section_type {
#define X(id, val) ELF_SECT_##id = (int)(val),
ELF_SECTION_TYPES(X)
#undef X
} elf_section_type;

#define ELF_SECTION_FLAGS(X) \
    X(WRITE, 0x1) \
    X(ALLOC, 0x2) \
    X(EXECINSTR, 0x4) \
    X(MERGE, 0x10) \
    X(STRINGS, 0x20) \
    X(INFO_LINK, 0x40) \
    X(LINK_ORDER, 0x80) \
    X(OS_NONCONFORMING, 0x100) \
    X(GROUP, 0x200) \
    X(TLS, 0x400) \
    X(RELA_LIVEPATCH, 0x00100000) \
    X(RO_AFTER_INIT, 0x00200000) \
    X(ORDERED, 0x04000000) \
    X(EXCLUDE, 0x08000000) \
    X(MASKOS, 0x0FF00000) \
    X(MASKPROC, 0xF0000000)

typedef enum elf_section_flag {
#define X(id, val) ELF_SECTFLAG_##id = (int)(val),
ELF_SECTION_FLAGS(X)
#undef X
} elf_section_flag;

#define ELF_SPECIAL_SECTION_INDEXES(X) \
    X(UNDEF, 0) \
    X(LOPROC, 0xFF00) \
    X(HIPROC, 0xFF1F) \
    X(LIVEPATCH, 0xFF20) \
    X(ABS, 0xFFF1) \
    X(COMMON, 0xFFF2) \
    X(HIRESERVE, 0xFFFF)

typedef enum elf_special_section_index {
#define X(id, val) ELF_SHN_##id = (val),
ELF_SPECIAL_SECTION_INDEXES(X)
#undef X
} elf_special_section_index;

#define ELF_FILE_TYPES(X) \
    X(NONE, 0) \
    X(REL, 1) \
    X(EXEC, 2) \
    X(DYN, 3) \
    X(CORE, 4) \
    X(LOPROC, 0xFF00) \
    X(HIPROC, 0xFFFF)

typedef enum elf_file_type {
#define X(id, val) ELF_FILE_##id = (val),
ELF_FILE_TYPES(X)
#undef X
} elf_file_type;

#define ELF_MACHINES(X) \
    X(NONE, 0) \
    X(M32, 1) \
    X(SPARC, 2) \
    X(386, 3) \
    X(68K, 4) \
    X(88K, 5) \
    X(486, 6) \
    X(860, 7) \
    X(MIPS, 8) \
    X(PARISC, 15) \
    X(SPARC32PLUS, 18) \
    X(PPC, 20) \
    X(PPC64, 21) \
    X(SPU, 23) \
    X(ARM, 40) \
    X(SH, 42) \
    X(SPARCV9, 43) \
    X(H8_300, 46) \
    X(IA_64, 50) \
    X(X86_64, 62) \
    X(S390, 22) \
    X(CRIS, 76) \
    X(M32R, 88) \
    X(MN10300, 89) \
    X(OPENRISC, 92) \
    X(ARCOMPACT, 93) \
    X(XTENSA, 94) \
    X(BLACKFIN, 106) \
    X(UNICORE, 110) \
    X(ALTERA_NIOS2, 113) \
    X(TI_C6000, 140) \
    X(HEXAGON, 164) \
    X(NDS32, 167) \
    X(AARCH64, 183) \
    X(TILEPRO, 188) \
    X(MICROBLAZE, 189) \
    X(TILEGX, 191) \
    X(ARCV2, 195) \
    X(RISCV, 243) \
    X(BPF, 247) \
    X(CSKY, 252) \
    X(LOONGARCH, 258) \
    X(FRV, 0x5441)

typedef enum elf_machine {
#define X(id, val) ELF_MACHINE_##id = (val),
ELF_MACHINES(X)
#undef X
} elf_machine;

#define ELF_SYMBOL_BINDINGS(X) \
    X(LOCAL, 0) \
    X(GLOBAL, 1) \
    X(WEAK, 2)

typedef enum elf_symbol_binding {
#define X(id, val) ELF_SYMBIND_##id = (val),
ELF_SYMBOL_BINDINGS(X)
#undef X
} elf_symbol_binding;

#define ELF_SYMBOL_TYPES(X) \
    X(NOTYPE, 0) \
    X(OBJECT, 1) \
    X(FUNC, 2) \
    X(SECTION, 3) \
    X(FILE, 4) \
    X(COMMON, 5) \
    X(TLS, 6)

typedef enum elf_symbol_type {
#define X(id, val) ELF_SYMTYPE_##id = (val),
ELF_SYMBOL_TYPES(X)
#undef X
} elf_symbol_type;

#define ELF_SYMBOL_CLASSES(X) \
    X(NONE, 0) \
    X(ELF32, 1) \
    X(ELF64, 2)

typedef enum elf_class {
#define X(id, val) ELF_CLASS_##id = (val),
ELF_SYMBOL_CLASSES(X)
#undef X
} elf_class;

#define ELF_SYMBOL_DATAS(X) \
    X(NONE, 0) \
    X(LE, 1) \
    X(BE, 2)

typedef enum elf_data {
#define X(id, val) ELF_DATA_##id = (val),
ELF_SYMBOL_DATAS(X)
#undef X
} elf_data;

#define ELF_VERSIONS(X) \
    X(NONE, 0) \
    X(CURRENT, 1)

typedef enum elf_version {
#define X(id, val) ELF_VERSION_##id = (val),
ELF_VERSIONS(X)
#undef X
} elf_version;

#define ELF_OSABIS(X) \
    X(NONE, 0) \
    X(GAMEFU, 69)

typedef enum elf_osabi {
#define X(id, val) ELF_OSABI_##id = (val),
ELF_OSABIS(X)
#undef X
} elf_osabi;

#define ELF32_ST_BIND(x) ((x) >> 4)
#define ELF32_ST_TYPE(x) ((x) & 0x0F)
#define ELF32_ST_INFO(b, t) (((b) << 4) | ((t) & 0x0F))

typedef uint8_t  elf32_byte;
typedef uint16_t elf32_half;
typedef uint32_t elf32_addr;
typedef uint32_t elf32_offs;
typedef uint32_t elf32_word;

typedef struct elf32_header {
    elf32_byte magic[4];
    elf32_byte class;
    elf32_byte endianness;
    elf32_byte version;
    elf32_byte abi;
    elf32_byte abi_version;
    elf32_byte _padding[7];
    elf32_half type;
    elf32_half machine;
    elf32_word version2;
    elf32_addr entry;
    elf32_offs ph_offset;
    elf32_offs sh_offset;
    elf32_word flags;
    elf32_half header_size;
    elf32_half ph_entry_size;
    elf32_half ph_count;
    elf32_half sh_entry_size;
    elf32_half sh_count;
    elf32_half sh_names_index;
} elf32_header;

typedef struct elf32_segment_header {
    elf32_word type;
    elf32_offs offset;
    elf32_addr virtual_address;
    elf32_addr physical_address;
    elf32_word file_size;
    elf32_word memory_size;
    elf32_word flags;
    elf32_word align;
} elf32_segment_header;

typedef struct elf32_section_header {
    elf32_word name_index;
    elf32_word type;
    elf32_word flags;
    elf32_addr virtual_address;
    elf32_offs offset;
    elf32_word size;
    elf32_word link;
    elf32_word info;
    elf32_word align;
    elf32_word entry_size;
} elf32_section_header;

typedef struct elf32_relocation {
    elf32_addr offset;
    elf32_word info;
} elf32_relocation;

typedef struct elf32_symbol {
    elf32_word name;
    elf32_addr value;
    elf32_word size;
    elf32_byte info;
    elf32_byte other;
    elf32_half shndx;
} elf32_symbol;

typedef struct elf32_note {
    elf32_word name_size;
    elf32_word content_size;
    elf32_word content_type;
} elf32_note;

typedef struct elf32_raw {
    elf32_header header;
    elf32_segment_header* segments;
    elf32_section_header* sections;

    elf32_word size;
    elf32_word is_owned;
    elf32_byte* data;

    elf32_word error_byte_offset;
    const char* error_message;
} elf32_raw;

const char* elf_segment_type_name(elf_segment_type v);
const char* elf_segment_flag_name(elf_segment_flag v);
const char* elf_section_type_name(elf_section_type v);
const char* elf_section_flag_name(elf_section_flag v);
const char* elf_special_section_index_name(elf_special_section_index v);
const char* elf_file_type_name(elf_file_type v);
const char* elf_machine_name(elf_machine v);
const char* elf_symbol_binding_name(elf_symbol_binding v);
const char* elf_symbol_type_name(elf_symbol_type v);
const char* elf_class_name(elf_class v);
const char* elf_data_name(elf_data v);
const char* elf_version_name(elf_version v);
const char* elf_osabi_name(elf_osabi v);

void elf32_raw_free(elf32_raw* raw);
void elf32_raw_dump(elf32_raw* raw);

elf32_raw elf32_read_raw_from_file(const char* file_path);
elf32_raw elf32_read_raw_from_bytes(char* data, elf32_word size);

#endif /* ELF_SINGLE_H_ */


#if (defined(ELF_IMPL) || defined(ELF_IMPLEMENTATION)) && !defined(ELF_IMPLEMENTATION_DEFINED)
#define ELF_IMPLEMENTATION_DEFINED
#undef ELF_IMPL
#undef ELF_IMPLEMENTATION

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void elf_hexdump(const char* data, elf32_word size) {
    fprintf(stderr, "          00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F\n");
    for (elf32_word i = 0; i < (size + 15) / 16; i++) {
        fprintf(stderr, "%08X ", i * 16);

        int nleft = 49;
        for (elf32_word j = 0; j < 16; j++) {
            elf32_word index = i * 16 + j;
            if (index >= size) break;
            if (j == 0x08) {
                nleft--;
                fputc(' ', stderr);
            }
            nleft -= fprintf(stderr, " %02hhX", data[index]);
        }

        for (int i = 0; i < nleft; i++)
            fputc(' ', stderr);
        fprintf(stderr, "  |");

        nleft = 16;
        for (elf32_word j = 0; j < 16; j++) {
            elf32_word index = i * 16 + j;
            if (index >= size) break;
            char b = data[index];
            if (b < 32 || b >= 127) fputc('.', stderr);
            else fputc(b, stderr);
            nleft--;
        }

        for (int i = 0; i < nleft; i++)
            fputc(' ', stderr);
        fprintf(stderr, "|\n");
    }
}

const char* elf_segment_type_name(elf_segment_type v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_SEG_##id: return #id;
ELF_SEGMENT_TYPES(X)
#undef X
    }
}

const char* elf_segment_flag_name(elf_segment_flag v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_SEGFLAG_##id: return #id;
ELF_SEGMENT_FLAGS(X)
#undef X
    }
}

const char* elf_section_type_name(elf_section_type v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_SECT_##id: return #id;
ELF_SECTION_TYPES(X)
#undef X
    }
}

const char* elf_section_flag_name(elf_section_flag v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_SECTFLAG_##id: return #id;
ELF_SECTION_FLAGS(X)
#undef X
    }
}

const char* elf_special_section_index_name(elf_special_section_index v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_SHN_##id: return #id;
ELF_SPECIAL_SECTION_INDEXES(X)
#undef X
    }
}

const char* elf_file_type_name(elf_file_type v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_FILE_##id: return #id;
ELF_FILE_TYPES(X)
#undef X
    }
}

const char* elf_machine_name(elf_machine v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_MACHINE_##id: return #id;
ELF_MACHINES(X)
#undef X
    }
}

const char* elf_symbol_binding_name(elf_symbol_binding v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_SYMBIND_##id: return #id;
ELF_SYMBOL_BINDINGS(X)
#undef X
    }
}

const char* elf_symbol_type_name(elf_symbol_type v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_SYMTYPE_##id: return #id;
ELF_SYMBOL_TYPES(X)
#undef X
    }
}

const char* elf_class_name(elf_class v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_CLASS_##id: return #id;
ELF_SYMBOL_CLASSES(X)
#undef X
    }
}

const char* elf_data_name(elf_data v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_DATA_##id: return #id;
ELF_SYMBOL_DATAS(X)
#undef X
    }
}

const char* elf_version_name(elf_version v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_VERSION_##id: return #id;
ELF_VERSIONS(X)
#undef X
    }
}

const char* elf_osabi_name(elf_osabi v) {
    switch (v) {
        default: return "<unknown>";
#define X(id, val) case ELF_OSABI_##id: return #id;
ELF_OSABIS(X)
#undef X
    }
}

void elf32_raw_free(elf32_raw* raw) {
    free(raw->segments);
    free(raw->sections);
    if (raw->is_owned) free(raw->data);
    *raw = (elf32_raw){0};
}

void elf32_raw_dump(elf32_raw* raw) {
    fprintf(stderr, "ELF Header:\n");

    fprintf(stderr, "  Magic:  ");
    for (int i = 0; i < 4; i++) {
        fprintf(stderr, " %02X", raw->header.magic[i]);
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "Class:                             %s\n", elf_class_name(raw->header.class));
    fprintf(stderr, "Data:                              2's complement, %s endian\n", raw->header.endianness == ELF_DATA_LE ? "little" : "big");
    fprintf(stderr, "Version:                           %u\n", raw->header.version);
    fprintf(stderr, "OS/ABI:                            %s\n", elf_osabi_name(raw->header.abi));
    fprintf(stderr, "ABI Version:                       %u\n", raw->header.abi_version);
    fprintf(stderr, "Type:                              %s\n", elf_file_type_name(raw->header.type));
    fprintf(stderr, "Machine:                           %s\n", elf_machine_name(raw->header.machine));
    fprintf(stderr, "Version:                           0x%X\n", raw->header.version2);
    fprintf(stderr, "Entry point address:               0x%X\n", raw->header.entry);
    fprintf(stderr, "Start of program headers:          %u (bytes into file)\n", raw->header.ph_offset);
    fprintf(stderr, "Start of section headers:          %u (bytes into file)\n", raw->header.sh_offset);
    fprintf(stderr, "Flags:                             0x%X\n", raw->header.flags);
    fprintf(stderr, "Size of this header:               %u (bytes)\n", raw->header.header_size);
    fprintf(stderr, "Size of program headers:           %u (bytes)\n", raw->header.ph_entry_size);
    fprintf(stderr, "Number of program headers:         %u\n", raw->header.ph_count);
    fprintf(stderr, "Size of section headers:           %u (bytes)\n", raw->header.sh_entry_size);
    fprintf(stderr, "Number of section headers:         %u\n", raw->header.sh_count);
    fprintf(stderr, "Section header string table index: %u\n", raw->header.sh_names_index);

    fprintf(stderr, "\nSegment Headers:\n");

    if (raw->header.sh_count != 0) {
        fprintf(stderr, "\nSection Headers:\n");

        fprintf(stderr, "  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
        const char* name_table = (const char*) raw->data + raw->sections[raw->header.sh_names_index].offset;
        for (elf32_word i = 0; i < raw->header.sh_count; i++) {
            int nleft;
    #define PAD(n, p) do { nleft = (p); for (int j = 0; j < (n) - nleft; j++) fputc(' ', stderr); } while (0)
            fprintf(stderr, "  [%2u] ", i);
            PAD(18, fprintf(stderr, "%s", &name_table[raw->sections[i].name_index]));
            PAD(16, fprintf(stderr, "%02X", raw->sections[i].type));
            fprintf(stderr, "%08X ", raw->sections[i].virtual_address);
            fprintf(stderr, "%06X ", raw->sections[i].offset);
            fprintf(stderr, "%06X ", raw->sections[i].size);
            fprintf(stderr, "%02X ", raw->sections[i].entry_size);
            fprintf(stderr, "    "); // TODO(local): flags
            fprintf(stderr, "%2u ", raw->sections[i].link);
            fprintf(stderr, "%3u ", raw->sections[i].info);
            fprintf(stderr, "%2u ", raw->sections[i].align);
            fprintf(stderr, "\n");
    #undef PAD
        }

        fprintf(stderr, "Key to Flags:\n");
        fprintf(stderr, "  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
        fprintf(stderr, "  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
        fprintf(stderr, "  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
        fprintf(stderr, "  D (mbind), p (processor specific)\n");
    }
}

elf32_raw elf32_read_raw_from_file(const char* file_path) {
    FILE* f = NULL;

#define RETURN_ERRMSG(s) do { if (f) fclose(f); return (elf32_raw){ .error_message = (s) }; } while (0)
    f = fopen(file_path, "rb");
    if (f == NULL) RETURN_ERRMSG(strerror(errno));
    if (0 != fseek(f, 0, SEEK_END)) RETURN_ERRMSG(strerror(errno));

    long file_size = ftell(f);
    if (file_size < 0) RETURN_ERRMSG(strerror(errno));
    if (0 != fseek(f, 0, SEEK_SET)) RETURN_ERRMSG(strerror(errno));

    char* data = malloc((size_t)file_size);
    if (data == NULL) RETURN_ERRMSG("Failed to allocate data buffer.");

    size_t read_count = fread(data, (size_t)file_size, 1, f);
    if (ferror(f)) RETURN_ERRMSG(strerror(errno));

    fclose(f);
    f = NULL;
#undef RETURN_ERRMSG

    elf32_raw result = elf32_read_raw_from_bytes(data, (elf32_word)read_count);
    result.is_owned = 1;
    return result;
}

elf32_raw elf32_read_raw_from_bytes(char* data, elf32_word size) {
    char* data_start = data;
    char* data_end = data + size;

    elf32_raw result = {
        .size = size,
        .data = (elf32_byte*)data,
    };

#define RETURN_ERRMSG(bo, s) do { result.error_byte_offset = (bo); result.error_message = (s); return result; } while (0)

    if (size < sizeof(elf32_header)) RETURN_ERRMSG(0, "Not enough bytes for the ELF header.");

#define READ1() (assert(data < data_end), data += 1, *(data - 1))

    elf32_header* header = &result.header;
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

    if (header->magic[0] != ELF_MAG0) RETURN_ERRMSG(0, "Invalid ELF magic.");
    if (header->magic[1] != ELF_MAG1) RETURN_ERRMSG(1, "Invalid ELF magic.");
    if (header->magic[2] != ELF_MAG2) RETURN_ERRMSG(2, "Invalid ELF magic.");
    if (header->magic[3] != ELF_MAG3) RETURN_ERRMSG(3, "Invalid ELF magic.");
    if (header->class != ELF_CLASS_ELF32) RETURN_ERRMSG(4, "ELF data is not ELF32.");

    int elf_endian = header->endianness;
    int host_endian; {
        uint16_t endian_check = 0x0201;
        host_endian = *((uint8_t*)&endian_check);
    }
    
#define ELFTOHOST2(Value) (host_endian == elf_endian) ? (Value) : ((((Value) >> 8) & 0xFF) | (((Value) & 0xFF) << 8))
#define ELFTOHOST4(Value) (host_endian == elf_endian) ? (Value) : ((((Value) >> 24) & 0xFF) | (((Value) >> 8) & 0xFF00) | (((Value) & 0xFF) << 24) | (((Value) & 0xFF00) << 8))
#define READ2() (assert(data < data_end), data += 2, ELFTOHOST2(*(uint16_t*)(data - 2)))
#define READ4() (assert(data < data_end), data += 4, ELFTOHOST4(*(uint32_t*)(data - 4)))

    header->type = READ2();
    header->machine = READ2();
    header->version2 = READ4();
    header->entry = READ4();
    header->ph_offset = READ4();
    header->sh_offset = READ4();
    header->flags = READ4();
    header->header_size = READ2();
    header->ph_entry_size = READ2();
    header->ph_count = READ2();
    header->sh_entry_size = READ2();
    header->sh_count = READ2();
    header->sh_names_index = READ2();
    
    assert(header->header_size == sizeof(elf32_header));
    assert(header->ph_entry_size == 0 || header->ph_entry_size == sizeof(elf32_segment_header));
    assert(header->sh_entry_size == 0 || header->sh_entry_size == sizeof(elf32_section_header));

    if (data_start + header->ph_offset + (header->ph_count * sizeof(elf32_segment_header)) > data_end)
        RETURN_ERRMSG(0, "Not enough bytes for the ELF program header table.");

    if (data_start + header->sh_offset + (header->sh_count * sizeof(elf32_section_header)) > data_end)
        RETURN_ERRMSG(0, "Not enough bytes for the ELF section header table.");

    for (elf32_word i = 0; i < header->ph_count; i++) {
        data = data_start + header->ph_offset + (i * sizeof(elf32_segment_header));
        elf32_segment_header* entry = &result.segments[i];
        entry->type = READ4();
        entry->offset = READ4();
        entry->virtual_address = READ4();
        entry->physical_address = READ4();
        entry->file_size = READ4();
        entry->memory_size = READ4();
        entry->flags = READ4();
        entry->align = READ4();
    }

    for (elf32_word i = 0; i < header->sh_count; i++) {
        data = data_start + header->sh_offset + (i * sizeof(elf32_section_header));
        elf32_section_header* entry = &result.sections[i];
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

#undef READ4
#undef READ2
#undef ELFTOHOST4
#undef ELFTOHOST2
#undef READ1
#undef RETURN_ERRMSG

    return result;
}

#endif /* ELF_IMPL */
