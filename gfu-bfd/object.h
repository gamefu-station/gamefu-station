#ifndef GAMEFU_BFD_OBJECT_H_
#define GAMEFU_BFD_OBJECT_H_

#include <gamefu/common.h>
#include <gamefu/bfd/system.h>

#define GFUOBJ_MAGIC 0xF0FF6109

#define GFUOBJ_ADDRSPACE_BIOS 0x01
#define GFUOBJ_ADDRSPACE_USER 0x02

#define GFUOBJ_RAM_SIZE_SMALL 0x3F
#define GFUOBJ_RAM_SIZE_LARGE 0xFF

typedef struct gfuobj_header {
    gfu_uword magic;
    gfu_ubyte version;
    gfu_ubyte address_space;
    gfu_ubyte ram_size;
    gfu_ubyte reserved1;
    gfu_uword entry_address;
    gfu_uword rom_size;
} gfuobj_header_t;

#endif /* GAMEFU_BFD_OBJECT_H_ */
