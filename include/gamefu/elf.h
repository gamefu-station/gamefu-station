#ifndef GAMEFU_ELF_H_
#define GAMEFU_ELF_H_

#include <elf.h>

#define GFU_ELF_SECTIONS(X) \
    X(INIT, 0, true, "init", ".init", ELF_SECT_PROGBITS, ELF_SECTFLAG_EXECINSTR) \
    X(FINI, 0, true, "fini", ".fini", ELF_SECT_PROGBITS, ELF_SECTFLAG_EXECINSTR) \
    X(CTOR, 0, true, "ctor", ".init_array", ELF_SECT_NOBITS, 0) \
    X(DTOR, 0, true, "dtor", ".fini_array", ELF_SECT_NOBITS, 0) \
    X(TEXT, 0, true, "text", ".text", ELF_SECT_PROGBITS, ELF_SECTFLAG_EXECINSTR) \
    X(BSS, 0, true, "bss", ".bss", ELF_SECT_NOBITS, ELF_SECTFLAG_WRITE) \
    X(DATA, 0, true, "data", ".data", ELF_SECT_NOBITS, ELF_SECTFLAG_WRITE) \
    X(RODATA, 0, true, "rodata", ".rodata", ELF_SECT_NOBITS, 0) \
    X(REL, 0, false, "reloc", ".rel", ELF_SECT_REL, 0) \
    X(REL_GL, 0, false, "global_reloc", ".rel.dyn") \
    X(REL_FN, 0, false, "function_reloc", ".rel.dyn") \
    X(SHSTRTAB, 0, false, "section_names", ".shstrtab", ELF_SECT_STRTAB, 0) \
    X(SYMTAB, 0, false, "symbol_names", ".symtab", ELF_SECT_SYMTAB, 0) \
    X(STRTAB, 0, false, "symbol_names", ".strtab", ELF_SECT_STRTAB, 0)

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
