#ifndef GAMEFU_OBJECT_H_
#define GAMEFU_OBJECT_H_

#include <gamefu/prologue.h>
#include <gamefu/system.h>

#define GFUOBJ_ADDRSPACE_BIOS 0x01
#define GFUOBJ_ADDRSPACE_USER 0x02

#define GFUOBJ_RAM_SIZE_SMALL 0x3F
#define GFUOBJ_RAM_SIZE_LARGE 0xFF

typedef struct gfuobj_header {
    gfu_uword_t magic;
    gfu_ubyte_t version;
    gfu_ubyte_t address_space;
    gfu_ubyte_t ram_size;
    gfu_ubyte_t reserved1;
    gfu_uword_t entry_address;
    gfu_uword_t rom_size;
} gfuobj_header_t;

//

#define GFUOBJ_MAGIC 0xF0FF6109
#define GFUOBJ_VERIFY_MAGIC(Magic) (((Magic) & 0xF0FFFFFF) == (GFUOBJ_MAGIC & 0xF0FFFFFF))
#define GFUOBJ_GET_VERSION(Magic)  (((Magic) & 0x0F000000) >> 24)

#define GFUOBJ2_RAM_SIZE_SMALL 0
#define GFUOBJ2_RAM_SIZE_LARGE 1

/* No relocation needs to be applied. */
#define GFUOBJ_R_NONE 0x00
/* A single 16-bit absolute address relocation. */
#define GFUOBJ_R_16 0x01
#define GFUOBJ_R_26 0x02
#define GFUOBJ_R_32 0x03
#define GFUOBJ_R_HI16 0x04
#define GFUOBJ_R_LO16 0x05
/* PC-relative, 16-bit address relocation.
 * Relative addresses only need relocated when they refer to external symbols. */
#define GFUOBJ_R_PC16 0x06

typedef gfu_uword_t gfuobj_addr_t;
typedef gfu_word_t  gfuobj_offs_t;
typedef gfu_uhalf_t gfuobj_sectidx_t;
typedef gfu_uword_t gfuobj_symidx_t;

typedef union gfuobj_flags {
    gfu_uword_t raw[1];
    struct {
        /* 0 if small RAM size, 1 if large RAM size. */
        gfu_uword_t ram_size : 1;
        gfu_uword_t padding0 : 7;
        /* The number of sections in this object file. */
        gfu_uword_t section_count : 24;
    } bits;
} gfuobj_flags_t;

gfu_static_assert(sizeof(gfuobj_flags_t) == 1 * sizeof(gfu_uword_t), "GameFU Object flags expected to be 1 32-bit word.");

typedef struct gfuobj2_header {
    gfu_uword_t magic;
    gfuobj_flags_t flags;
    /* The virtual address where the program entry is located.
     * This value minus the text section or segment's base address
     * should be the index into that section or segment. */
    gfuobj_addr_t entry_address;
    /* The total size of the ROM file in bytes. */
    gfu_uword_t rom_size;
} gfuobj2_header_t;

gfu_static_assert(sizeof(gfuobj_header_t) == 4 * sizeof(gfu_uword_t), "GameFU Object header expected to be 4 32-bit words.");

typedef enum gfuobj_section_class {
    GFUOBJ_CLASS_DATA,
    GFUOBJ_CLASS_TEXT,
    GFUOBJ_CLASS_BSS,
} gfuobj_section_class_t;

typedef struct gfuobj_section {
    /* The name of this section.
     * String data like this is stored in the string table.
     * This pointer points directly into that table from the loaded ROM data. */
    const char* name;
    /* The absolute location in the ROM data where this section begins. */
    gfuobj_addr_t offset;
    /* The total size of this section in bytes. */
    gfu_uword_t size;
    /* Class of the section. Defines whether the section is readable, writeable and/or executable */
    gfuobj_section_class_t class;
} gfuobj_section_t;

typedef struct gfuobj_rel {
    /* The absolute location in the ROM data to apply the relocation to. */
    gfuobj_addr_t offset;
    union {
        gfu_uword_t raw;
        struct {
            /* The type of relocation to apply. */
            gfu_uword_t type : 8;
            /* The index of a referenced symbol, if any. */
            gfu_uword_t symbol_index : 24;
        } bits;
    } info;
} gfuobj_rel_t;

#endif /* GAMEFU_OBJECT_H_ */
