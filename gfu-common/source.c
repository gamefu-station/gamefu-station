#include <gamefu/common.h>

bool
gfu_source_load_from_file(const char* path, gfu_source* source) {
    bool result = true;
    source->name = path;

    errno = 0;
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        gfu_diag_pause_error_flush();
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, \
            "Failed to open source file '%s': %s.", path, strerror(errno));
        return_defer(false);
    }

    errno = 0;
    if (0 != fseek(f, 0, SEEK_END)) {
        gfu_diag_pause_error_flush();
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, \
            "Failed to read source file '%s': %s.", path, strerror(errno));
        return_defer(false);
    }

    errno = 0;
    gfu_word length = (gfu_word) ftell(f);
    if (length < 0) {
        gfu_diag_pause_error_flush();
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, \
            "Failed to read source file '%s': %s.", path, strerror(errno));
        return_defer(false);
    }

    errno = 0;
    if (0 != fseek(f, 0, SEEK_SET)) {
        gfu_diag_pause_error_flush();
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, \
            "Failed to read source file '%s': %s.", path, strerror(errno));
        return_defer(false);
    }

    char* text = calloc((size_t) (length + 1), sizeof *text);

    errno = 0;
    gfu_word nread = (gfu_word) fread(text, 1, (size_t) length, f);
    assertn(nread == length);
    if (ferror(f)) {
        gfu_diag_pause_error_flush();
        gfu_diag_issue(DIAG_ERROR, GFU_NOSOURCE, \
            "Failed to read source file '%s': %s.", path, strerror(errno));
        return_defer(false);
    }

    source->text = text;
    source->length = length;

defer:;
    if (f != nullptr) fclose(f);
    gfu_diag_flush();
    return result;
}
