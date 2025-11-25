#include <gamefu/prologue.h>
#include <fucc/source.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool fucc_read_source_from_file(const char* path, fucc_source_t* source) {
    bool result = true;
    source->name = path;

    errno = 0;
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error: Failed to open source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    errno = 0;
    if (0 != fseek(f, 0, SEEK_END)) {
        fprintf(stderr, "Error: Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    errno = 0;
    int64_t length = ftell(f);
    if (length < 0) {
        fprintf(stderr, "Error: Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    errno = 0;
    if (0 != fseek(f, 0, SEEK_SET)) {
        fprintf(stderr, "Error: Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    char* text = calloc((size_t)(length + 1), sizeof *text);

    errno = 0;
    (void)fread(text, 1, (size_t)length, f);
    if (ferror(f)) {
        fprintf(stderr, "Error: Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    source->text = text;
    source->length = (gfu_uword_t)length;

defer:;
    if (f != NULL) fclose(f);
    return result;
}

void fucc_source_free(fucc_source_t* source) {
    free((void*)source->text);
    *source = (fucc_source_t){0};
}
