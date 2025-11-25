#include <gamefu/prologue.h>

#include <fucc/context.h>
#include <fucc/diagnostic.h>

#include <stdarg.h>
#include <inttypes.h>

static const char* fucc_diag_level_names[] = {
    "Ignored",
    "Note",
    "Remark",
    "Warning",
    "Error",
    "Fatal",
};

static const char* fucc_diag_level_colors[] = {
    "",
    "\x1b[32m", // green
    "\x1b[36m", // cyan
    "\x1b[33m", // yellow
    "\x1b[31m", // red
    "\x1b[35m", // magenta
};

static void fucc_source_seek_line_column(fucc_source_t* source, gfu_uword_t location, gfu_uword_t* line, gfu_uword_t* column) {
    if (location > source->length) location = source->length;

    const char* line_start = source->text;
    const char* end = line_start + location;

    *line = 1;
    for (const char* cur = source->text; cur < end; cur++) {
        if (*cur == '\n') {
            (*line)++;
            line_start = cur + 1;
        }
    }

    *column = 1 + (end - line_start);
}

void fucc_diag_flush(fucc_context_t* c) {
    if (c->diag_count <= 0) return;
    fucc_diag_level_t level = c->diag_group[0].level;

    for (gfu_uword_t i = 0; i < c->diag_count; i++) {
        fucc_diag_t d = c->diag_group[i];

        if (c->color_output) fputs(fucc_diag_level_colors[d.level], stderr);
        fputs(fucc_diag_level_names[d.level], stderr);
        if (c->color_output) fputs("\x1b[0m", stderr);

        if (d.source->text == nullptr) {
            fprintf(stderr, ": ");
        } else {
            //fprintf(stderr, ": %s[%" PRIi32 "]: ", d.source->name, d.location);
            gfu_uword_t line, column;
            fucc_source_seek_line_column(d.source, d.location, &line, &column);
            fprintf(stderr, ": %s(%" PRIi32 ",%" PRIi32 "): ", d.source->name, line, column);
        }

        fprintf(stderr, "%s\n", d.message);
        free(d.message);
    }

    c->diag_count = 0;

    if (level == FUCC_ERROR) {
        c->error_count++;
    } else if (level == FUCC_FATAL) abort();
}

bool fucc_diag_has_issued_error(fucc_context_t* c) {
    return c->error_count > 0;
}

void fucc_diag_issue(fucc_context_t* c, fucc_diag_level_t level, fucc_source_t* source, gfu_uword_t location, const char* format, ...) {
    va_list v;
    va_start(v, format);
    fucc_diag_issue_v(c, level, source, location, format, v);
    va_end(v);
}

void fucc_diag_issue_v(fucc_context_t* c, fucc_diag_level_t level, fucc_source_t* source, gfu_uword_t location, const char* format, va_list v) {
    if (level == FUCC_IGNORED) return;
    if (level == FUCC_NOTE && c->diag_count == 0) return;

    if (c->diag_count >= FUCC_DIAG_MAX) fucc_diag_flush(c);
    if (level > FUCC_NOTE && c->diag_count > 0) fucc_diag_flush(c);

    va_list v0, v1;
    va_copy(v0, v);
    va_copy(v1, v);

    long length = vsnprintf(NULL, 0, format, v0);
    va_end(v0);

    char* message = malloc((size_t)(length + 1));
    (void)vsnprintf(message, (size_t)(length + 1), format, v1);
    va_end(v1);

    c->diag_group[c->diag_count++] = (fucc_diag_t) {
        .level = level,
        .source = source,
        .location = location,
        .message = message,
    };
}
