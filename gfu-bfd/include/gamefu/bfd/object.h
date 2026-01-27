#ifndef GAMEFU_BFD_OBJECT_H_
#define GAMEFU_BFD_OBJECT_H_

#include <gamefu/common.h>

#define GFUOBJ_MAGIC 0xF0FF6109
#define GFUOBJ_VERIFY_MAGIC(Magic) (((Magic) & 0xF0FFFFFF) == (GFUOBJ_MAGIC & 0xF0FFFFFF))
#define GFUOBJ_GET_VERSION(Magic)  (((Magic) & 0x0F000000) >> 24)
#define GFUOBJ_MAGIC_VERSION(Version) ((GFUOBJ_MAGIC) | (((Version) & 0x0F) << 24))

#define GFUOBJ_CURRENT_VERSION 2

#define GFUOBJ_ALIGN(Value) ((Value) + ((sizeof(gfu_uword) - ((Value) % sizeof(gfu_uword))) % sizeof(gfu_uword)))

#define GFUOBJ_RAM_SIZE_SMALL 0
#define GFUOBJ_RAM_SIZE_LARGE 1

/* No relocation needs to be applied. */
#define GFUOBJ_R_NONE 0x00
/* A single 16-bit absolute address relocation.
   `gfu_inst::imm.value`, likely right-shifted to be 18 bits (word aligned) address. */
#define GFUOBJ_R_16 0x01
/* A single 26-bit absolute address relocation.
   `gfu_inst::addr.value`, likely right-shifted to be 28 bits (word aligned) address. */
#define GFUOBJ_R_26 0x02
/* A single 32-bit absolute address relocation. */
#define GFUOBJ_R_32 0x03
/* A single 16-bit absolute address relocation.
   `gfu_inst::imm.value` treated as the high 16-bits of an address. */
#define GFUOBJ_R_HI16 0x04
/* A single 16-bit absolute address relocation.
   `gfu_inst::imm.value` treated as the low 16-bits of an address. */
#define GFUOBJ_R_LO16 0x05
/* PC-relative, 16-bit address relocation.
 * Relative addresses only need relocated when they refer to external symbols. */
#define GFUOBJ_R_PC16 0x06

#define GFUOBJ_NUL_SECTION_NAME ""
#define GFUOBJ_STRINGS_SECTION_NAME ".str"
#define GFUOBJ_RELOCATION_SECTION_NAME ".rel"
#define GFUOBJ_SYMBOL_SECTION_NAME ".sym"
#define GFUOBJ_DEFAULT_DATA_SECTION_NAME ".dat"
#define GFUOBJ_DEFAULT_TEXT_SECTION_NAME ".txt"
#define GFUOBJ_DEFAULT_BSS_SECTION_NAME ".bss"

#define GFUOBJ_NUL_SECTIDX 0
#define GFUOBJ_STRINGS_SECTIDX 1
#define GFUOBJ_RELOCATION_SECTIDX 2
#define GFUOBJ_SYMBOL_SECTIDX 3

typedef gfu_uword gfuobj_addr;
typedef gfu_word  gfuobj_offs;
typedef gfu_uhalf gfuobj_sectidx;
typedef gfu_uword gfuobj_symidx;
typedef gfu_uword gfuobj_relidx;

#define GFUOBJ_ADDR_INVALID 0xFFFFFFFF
#define GFUOBJ_SECTIDX_INVALID 0xFFFF
#define GFUOBJ_SYMIDX_INVALID 0xFFFFFFFF
#define GFUOBJ_RELIDX_INVALID 0xFFFFFFFF

/*

Object File Layout: (Every struct/section/whatever will be word aligned and padded to the next 4 bytes if necessary)
  Header (4 words)
    Contains section count
  Section Table (N Section Headers, described by header)
  Raw Section Data (Remaining bytes in the ROM, described by entries in the section table)

Remarks:
- Should have at least four sections (.nul, .str, .rel, .sym)

*/

typedef union gfuobj_flags {
    gfu_uword raw[1];
    struct {
        /* 0 if small RAM size, 1 if large RAM size. */
        gfu_uword ram_size : 1;
        gfu_uword padding0 : 7;
        /* The number of sections in this object file. */
        gfu_uword section_count : 24;
    } bits;
} gfuobj_flags;

static_assert(sizeof(gfuobj_flags) == 1 * sizeof(gfu_uword), "GameFU Object flags expected to be 1 32-bit word.");

typedef struct gfuobj_header {
    gfu_uword magic;
    gfuobj_flags flags;
    /* The virtual address where the program entry is located.
     * This value minus the text section or segment's base address
     * should be the index into that section or segment. */
    gfuobj_addr entry_address;
    /* The total size of the ROM file in bytes. */
    gfu_uword rom_size;
} gfuobj_header;

static_assert(sizeof(gfuobj_header) == 4 * sizeof(gfu_uword), "GameFU Object header expected to be 4 32-bit words.");

typedef enum gfuobj_section_class {
    GFUOBJ_CLASS_META,
    GFUOBJ_CLASS_DATA,
    GFUOBJ_CLASS_TEXT,
    GFUOBJ_CLASS_BSS,
} gfuobj_section_class;

typedef struct gfuobj_section {
    /* The name of this section.
     * String data like this is stored in the string table.
     * This pointer points directly into that table from the loaded ROM data. */
    gfuobj_addr name;
    /* The absolute location in the ROM data where this section begins. */
    gfuobj_addr offset;
    /* The total size of this section in bytes. */
    gfu_uword size;
    /* Class of the section. Defines whether the section is readable, writeable and/or executable */
    gfuobj_section_class class;
    /* Where in virtual memory this section would like to be placed.
       If multiple sections overlap in virtual memory, then linking will fail. */
    gfuobj_addr virtual_address;
} gfuobj_section;

static_assert(sizeof(gfuobj_section) == 5 * sizeof(gfu_uword), "GameFU Section header expected to be 5 32-bit words.");

typedef struct gfuobj_symbol {
    /* The name of this symbol.
     * String data like this is stored in the string table.
     * This pointer points directly into that table from the loaded ROM data. */
    gfuobj_addr name;
    union {
        gfu_uhalf raw;
    } info;
    /* Index of the section in which the symbol is defined. */
    gfuobj_sectidx section;
    /* The absolute location in the ROM data where this symbol is defined. */
    gfuobj_addr offset;
    /* The size of all data related to this symbol. */
    gfu_uword size;
} gfuobj_symbol;

static_assert(sizeof(gfuobj_symbol) == 4 * sizeof(gfu_uword), "GameFU Symbol entry expected to be 4 32-bit words.");

typedef struct gfuobj_relocation {
    /* The absolute location in the ROM data to apply the relocation to. */
    gfuobj_addr offset;
    union {
        gfu_uword raw;
        struct {
            /* The type of relocation to apply. */
            gfu_uword type : 8;
            /* The index of a referenced symbol, if any. */
            gfu_uword symbol_index : 24;
        } bits;
    } info;
} gfuobj_relocation;

static_assert(sizeof(gfuobj_relocation) == 2 * sizeof(gfu_uword), "GameFU Relocation entry expected to be 2 32-bit words.");

typedef struct gfuobj_raw {
    gfuobj_header header;
    gfu_ubyte data[];
} gfuobj_raw;

GAMEFU_API gfuobj_raw* gfuobj_raw_read_from_file(const char* file_path);
GAMEFU_API gfuobj_raw* gfuobj_raw_from_data(gfu_ubyte* data, gfu_uword size);
GAMEFU_API void gfuobj_raw_write_to_file(gfuobj_raw* raw, const char* file_path);

GAMEFU_API const char* gfuobj_raw_get_section_name(gfuobj_raw* obj, gfuobj_addr section_name_offset);

GAMEFU_API void* gfuobj_raw_get_pointer(gfuobj_raw* obj, gfuobj_addr addr);
GAMEFU_API gfuobj_section* gfuobj_raw_get_section_header(gfuobj_raw* obj, gfuobj_sectidx section_index);
GAMEFU_API gfuobj_section* gfuobj_raw_get_section_header_by_name(gfuobj_raw* obj, const char* section_name);
GAMEFU_API gfuobj_symbol* gfuobj_raw_get_symbol_by_index(gfuobj_raw* obj, gfuobj_symidx symbol_index);
GAMEFU_API gfuobj_symbol* gfuobj_raw_get_symbol_by_name_addr(gfuobj_raw* obj, gfuobj_addr symbol_name_addr);
GAMEFU_API gfuobj_symbol* gfuobj_raw_get_symbol_by_name(gfuobj_raw* obj, const char* symbol_name);
GAMEFU_API gfuobj_relocation* gfuobj_raw_get_relocation(gfuobj_raw* obj, gfuobj_relidx relocation_index);

typedef struct gfuobj_byte_builder {
    GFU_DA_FIELDS(gfu_ubyte);
} gfuobj_byte_builder;

typedef struct gfuobj_section_builder {
    const char* name;
    gfuobj_section_class class;
    gfuobj_addr virtual_address;
    gfuobj_byte_builder data;
} gfuobj_section_builder;

typedef struct gfuobj_symbol_builder {
    /* The name of this symbol. */
    const char* name;
    union {
        gfu_uhalf raw;
    } info;
    /* Index of the section in which the symbol is defined. */
    gfuobj_sectidx section;
    /* The absolute location in the ROM data where this symbol is defined. */
    gfuobj_addr offset;
    /* The size of all data related to this symbol. */
    gfu_uword size;
} gfuobj_symbol_builder;

typedef struct gfuobj_relocation_builder {
    /* The absolute location in the ROM data to apply the relocation to. */
    gfuobj_addr offset;
    union {
        gfu_uword raw;
        struct {
            /* The type of relocation to apply. */
            gfu_uword type : 8;
            /* The index of a referenced symbol, if any. */
            gfu_uword symbol_index : 24;
        } bits;
    } info;
} gfuobj_relocation_builder;

typedef struct gfuobj_builder {
    gfuobj_flags flags;
    /* The virtual address where the program entry is located.
     * This value minus the text section or segment's base address
     * should be the index into that section or segment. */
    gfuobj_addr entry_address;
    GFU_DA_FIELDS(gfuobj_section_builder);
} gfuobj_builder;

GAMEFU_API void gfuobj_builder_init(gfuobj_builder* builder);
GAMEFU_API gfuobj_raw* gfuobj_builder_to_raw(gfuobj_builder* builder);
GAMEFU_API void gfuobj_builder_deinit(gfuobj_builder* builder);

/// Returns the relative address from the start of this String section data, which is the index of the string in the table.
GAMEFU_API gfuobj_addr gfuobj_builder_intern_string(gfuobj_builder* builder, const char* string);
GAMEFU_API gfuobj_sectidx gfuobj_builder_add_section(gfuobj_builder* builder, const char* section_name);
/// Returned pointer is subject to invalidation; do not store long-term, only store the index and look-up its builder on demand.
GAMEFU_API gfuobj_section_builder* gfuobj_builder_get_section(gfuobj_builder* builder, gfuobj_sectidx section_index);
/// Returned pointer is subject to invalidation; do not store long-term, only store the index and look-up its builder on demand.
GAMEFU_API gfuobj_symbol_builder* gfuobj_builder_get_symbol(gfuobj_builder* builder, gfuobj_symidx symbol_index);
/// Returned pointer is subject to invalidation; do not store long-term, only store the index and look-up its builder on demand.
GAMEFU_API gfuobj_relocation_builder* gfuobj_builder_get_relocation(gfuobj_builder* builder, gfuobj_relidx relocation_index);

/// Returns the relative address from the start of this byte builder, in case that's useful.
GAMEFU_API gfuobj_addr gfuobj_byte_builder_push_word(gfuobj_byte_builder* byte_builder, gfu_uword word);

GAMEFU_API gfuobj_symidx gfuobj_builder_push_symbol(gfuobj_builder* builder, const char* symbol_name);
GAMEFU_API gfuobj_relidx gfuobj_builder_push_relocation(gfuobj_builder* builder);

#endif /* GAMEFU_BFD_OBJECT_H_ */
