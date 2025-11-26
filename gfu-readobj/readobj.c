#include <gamefu/prologue.h>

typedef struct readobj_options {
    const char* program_name;
} readobj_options_t;

static bool readobj_parse_options(
    int argc, char** argv,
    readobj_options_t* options
);

int main(int argc, char** argv) {
    int result = 0;
defer:;
    return result;
}

static void readobj_print_help() {
    fprintf(
        stderr,
        "readobj -- Display information about a GFU object file.\n"
    );
}

static bool readobj_parse_options(
    int argc, char** argv,
    readobj_options_t* options
) {
    return false;
}
