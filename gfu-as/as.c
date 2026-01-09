#include "fuasm.h"

#include <stdio.h>
#include <gamefu/bfd/object.h>

int main(int argc, char** argv) {
    if (argc > 1) return gfuas_driver_main(argc, argv);

    gfuobj_builder b = {0};
    gfuobj_builder_init(&b);

    gfuobj_sectidx data_idx = gfuobj_builder_add_section(&b, GFUOBJ_DEFAULT_DATA_SECTION_NAME);
    assertn(data_idx != GFUOBJ_SECTIDX_INVALID);

    gfuobj_builder_push_symbol(&b, "_start");

    gfuobj_raw* obj = gfuobj_builder_to_raw(&b);

    FILE* file = fopen("testout.gfu", "w");
    assertn(file != nullptr);

    size_t written = fwrite(obj, 1, obj->header.rom_size, file);
    assertn(written == obj->header.rom_size);

    (void) fclose(file);
    free(obj);
    gfuobj_builder_deinit(&b);
}
