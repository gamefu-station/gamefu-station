#include <gamefu/prologue.h>

typedef struct readgfu_options {
    const char* program_name;
} readgfu_options_t;

static bool readgfu_parse_options(
    int argc, char** argv,
    readgfu_options_t* options
);

int main(int argc, char** argv) {
    int result = 0;
defer:;
    return result;
}

static void readgfu_print_help() {
    fprintf(
        stderr,
        "readgfu -- Display information about a GFU object file.\n"
    );
}

static bool readgfu_parse_options(
    int argc, char** argv,
    readgfu_options_t* options
) {
    return false;
}
