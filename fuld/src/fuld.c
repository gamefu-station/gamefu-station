#define KOS_IMPL
#include <kos.h>

#define GFU_ELF_IMPL
#include <gamefu/elf.h>

#include <choir/core.h>

#define INPUT_FILE_MAX 256

int main(int argc, char** argv) {
    int result = 0;
    choir_context_ref context = nullptr;

    const char* output_file = "a.o";
    const char* input_files[INPUT_FILE_MAX] = {0};
    elf32_raw elf32_data[INPUT_FILE_MAX] = {0};
    ssize_t input_file_count = 0;

    context = choir_context_create();

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        if (0 == strcmp("-o", arg)) {
            output_file = argv[++i];
        } else {
            if (input_file_count >= INPUT_FILE_MAX) {
                choir_diag_issue(context, CHOIR_ERROR, "Too many input files. Max is %d.", INPUT_FILE_MAX);
                kos_return_defer(1);
            }

            input_files[input_file_count++] = arg;
        }
    }

    // TODO(local): check if the output file is part of the input list, that's probably not intended

    for (ssize_t i = 0; i < input_file_count; i++)
    {
        elf32_data[i] = elf32_read_raw_from_file(input_files[i]);
    }

defer:;
    for (ssize_t i = 0; i < input_file_count; i++)
        elf32_raw_free(&elf32_data[i]);

    choir_context_destroy(context);
    return result;
}
