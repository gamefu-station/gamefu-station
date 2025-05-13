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
    gfu_elf elf_data[INPUT_FILE_MAX] = {0};
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
        FILE* f = fopen(input_files[i], "rb");
        if (f == nullptr) {
            choir_diag_issue(context, CHOIR_ERROR, "Failed to open ELF object file '%s'.", input_files[i]);
            kos_return_defer(1);
        }

        bool success = gfu_elf_from_file(&elf_data[i], f);
        fclose(f);

        if (!success) {
            choir_diag_issue(context, CHOIR_ERROR, "Failed to read ELF object file '%s'.", input_files[i]);
            kos_return_defer(1);
        }

        gfu_elf_debug_print(&elf_data[i]);

        //fprintf(stderr, ";; ELF Hexdump for '%s'\n", input_files[i]);
        //kos_hexdump(elf_data[i].raw.file_data, (ssize_t)elf_data[i].raw.data_size);

        for (uint32_t j = 0; j < elf_data[i].segment_count; j++) {
            fprintf(stderr, ";; Segment 0x%04X for '%s'\n", elf_data[i].segments[j].type, input_files[i]);
            kos_hexdump(elf_data[i].segments[j].data, (ssize_t)elf_data[i].segments[j].data_size);
            fprintf(stderr, "\n");
        }

        for (uint32_t j = 0; j < elf_data[i].section_count; j++) {
            fprintf(stderr, ";; Section %s (0x%04X) for '%s'\n", elf_data[i].sections[j].name, elf_data[i].raw.sections[j].name_index, input_files[i]);
            kos_hexdump(elf_data[i].sections[j].data, (ssize_t)elf_data[i].sections[j].data_size);
            fprintf(stderr, "\n");
        }
    }

defer:;
    for (ssize_t i = 0; i < input_file_count; i++)
        gfu_elf_free(&elf_data[i]);

    choir_context_destroy(context);
    return result;
}
