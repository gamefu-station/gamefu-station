#include <gamefu/common.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <io.h>
#  define isatty _isatty
#  define fileno _fileno
#elif defined(__linux__)
#  include <unistd.h>
#endif

#include <gamefu/bfd/object.h>

#include <gamefu/as/as.h>

int main(int argc, char** argv) {
    if (argc >= 2 && 0 == strcmp(argv[1], "-as1")) {
        return gfuas_driver_main(argc, argv);
    }

    int fuasm_driver_main(int argc, char** argv);
    return fuasm_driver_main(argc, argv);
}

#define VERSION "0.2.0"

typedef struct gfuas_options {
    const char* program;
    bool verbose;
    source* sources;
    gfu_uword source_count;
    const char* output;
} gfuas_options;

typedef struct gfuas_state {
    gfuas_options options;
    gfuas_builder builder;

    const char* entry_name;
    bool is_entry_set;
} gfuas_state;

GAMEFU_API int gfuas_driver_main(int argc, char** argv) {
    int result = 1;

    diag_color_output(isatty(fileno(stderr)));

    gfuas_state state = {
        .entry_name = "_start",
    };

    // if we make it here, we succeeded; set the exit code appropriately.
    result = 0;
fail:;
    return result;
}

GAMEFU_API int gfuas_driver_fuzz(const char* text, size_t length) {
    return 0;
}
