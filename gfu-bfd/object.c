#include <gamefu/bfd/object.h>

GAMEFU_API gfuobj_section* gfuobj_raw_get_section_header(gfuobj_raw* obj, gfuobj_sectidx section_index) {
    if (obj == nullptr) return nullptr;

    gfuobj_header* header = (void*) obj;
    gfuobj_section* sections = (void*) ((char*) obj + sizeof(gfuobj_header));

    if (header->flags.bits.section_count <= section_index) {
        return nullptr;
    }

    return (void*) ((char*) obj + sections[section_index].offset);
}
