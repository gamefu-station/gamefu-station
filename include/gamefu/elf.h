#ifndef GAMEFU_ELF_H_
#define GAMEFU_ELF_H_

#include <elf.h>

#define GFU_ELF_SECTIONS(X) \
    X(NULL, 0, false, "", ELF_SECT_NULL, 0) \
    X(INIT, 1, true, ".init", ELF_SECT_PROGBITS, ELF_SECTFLAG_EXECINSTR) \
    X(FINI, 2, true, ".fini", ELF_SECT_PROGBITS, ELF_SECTFLAG_EXECINSTR) \
    X(CTOR, 3, true, ".init_array", ELF_SECT_NOBITS, 0) \
    X(DTOR, 4, true, ".fini_array", ELF_SECT_NOBITS, 0) \
    X(TEXT, 5, true, ".text", ELF_SECT_PROGBITS, ELF_SECTFLAG_EXECINSTR) \
    X(BSS, 6, true, ".bss", ELF_SECT_NOBITS, ELF_SECTFLAG_WRITE) \
    X(DATA, 7, true, ".data", ELF_SECT_NOBITS, ELF_SECTFLAG_WRITE) \
    X(RODATA, 8, true, ".rodata", ELF_SECT_NOBITS, 0) \
    X(REL, 9, false, ".rel", ELF_SECT_REL, 0) \
    X(REL_GL, 10, false, ".rel.dyn", ELF_SECT_REL, 0) \
    X(REL_FN, 11, false, ".rel.dyn", ELF_SECT_REL, 0) \
    X(SHSTRTAB, 12, false, ".shstrtab", ELF_SECT_STRTAB, 0) \
    X(SYMTAB, 13, false, ".symtab", ELF_SECT_SYMTAB, 0) \
    X(STRTAB, 14, false, ".strtab", ELF_SECT_STRTAB, 0)

typedef enum gfu_elf_section_kind {
#define X(id, val, user_facing, name, type, flags) \
    GFU_ELF_SECT_##id = (int)(val),
GFU_ELF_SECTIONS(X)
#undef X
} gfu_elf_section_kind;

typedef enum gfu_elf_relocation_type {
    GFU_ELF_RELOCATION_RS,
    GFU_ELF_RELOCATION_RT,
    GFU_ELF_RELOCATION_RD,
    GFU_ELF_RELOCATION_SHAMT,
    GFU_ELF_RELOCATION_FUNCT,
    GFU_ELF_RELOCATION_IMM,
    GFU_ELF_RELOCATION_ADDR,
    GFU_ELF_RELOCATION_CODE,
    GFU_ELF_RELOCATION_SEL,
} gfu_elf_relocation_type;

#define GFU_ELF32_ST_RELKIND(x) ((x) >> 8)
#define GFU_ELF32_ST_INFO(k, b, t) (((k) << 8) | (((b) & 0x0F) << 4) | ((t) & 0x0F))

#endif /* GAMEFU_ELF_H_ */


#if (defined(GFU_ELF_IMPL) || defined(GFU_ELF_IMPLEMENTATION)) && !defined(GFU_ELF_IMPLEMENTATION_DEFINED)
#define GFU_ELF_IMPLEMENTATION_DEFINED
#undef GFU_ELF_IMPL
#undef GFU_ELF_IMPLEMENTATION

#define ELF_IMPL
#include <elf.h>

#endif /* GFU_ELF_IMPL */
