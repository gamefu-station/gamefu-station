#define KOS_IMPL
#include <kos.h>

#include <gamefu/fucc.h>

#include <choir/core.h>

#define INPUT_FILE_MAX 256

int main(int argc, char** argv) {
    int result = 0;
    choir_context_ref context = nullptr;
    choir_source_ref source = nullptr;

    const char* output_file = nullptr;
    const char* input_files[INPUT_FILE_MAX] = {0};
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

    choir_diag_issue(context, CHOIR_INFO, "Hello, FUCC, from Choir diagnostics!");

defer:
    choir_context_destroy(context);
    return result;
}
