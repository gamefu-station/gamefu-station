#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gamefu/source.h"

bool load_source_from_file(const char* path, source* source) {
    bool result = true;
    source->name = path;

    errno = 0;
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        diag_pause_error_flush();
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to open source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    errno = 0;
    if (0 != fseek(f, 0, SEEK_END)) {
        diag_pause_error_flush();
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    errno = 0;
    int64_t length = ftell(f);
    if (length < 0) {
        diag_pause_error_flush();
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    errno = 0;
    if (0 != fseek(f, 0, SEEK_SET)) {
        diag_pause_error_flush();
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    char* text = calloc((size_t)(length + 1), sizeof *text);

    errno = 0;
    (void)fread(text, 1, (size_t)length, f);
    if (ferror(f)) {
        diag_pause_error_flush();
        diag_issue(DIAG_ERROR, NOSOURCE, "Failed to read source file '%s': %s.", path, strerror(errno));
        gfu_return_defer(false);
    }

    source->text = text;
    source->length = (int32_t)length;

defer:;
    if (f != NULL) fclose(f);
    diag_flush();
    return result;
}
