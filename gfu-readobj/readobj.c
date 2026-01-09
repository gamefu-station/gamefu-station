#include <gamefu/common.h>
#include <gamefu/bfd/object.h>

#include <stdio.h>

static void readobj_print_help(void);

int main(int argc, char** argv) {
    if (argc == 1) {
        readobj_print_help();
        return 0;
    }

    int result = 0;

    const char* obj_path = argv[1];

fail:;
    return result;

}

static void readobj_print_help(void) {
    fprintf(
        stderr,
        "readobj -- Display information about a GFU object file.\n"
    );
}
