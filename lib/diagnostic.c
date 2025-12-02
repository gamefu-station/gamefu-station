#include <stdarg.h>

#include "gamefu/diagnostic.h"

static const char* level_names[] = {
    "Ignored",
    "Note",
    "Remark",
    "Warning",
    "Error",
    "Fatal",
};

static const char* level_colors[] = {
    "",
    "\x1b[32m", // green
    "\x1b[36m", // cyan
    "\x1b[33m", // yellow
    "\x1b[31m", // red
    "\x1b[35m", // magenta
};

static void seek_line_column(source source, int32_t location, int32_t* line, int32_t* column) {
    if (location > source.length) location = source.length;

    const char* line_start = source.text;
    const char* end = line_start + location;

    *line = 1;
    for (const char* cur = source.text; cur < end; cur++) {
        if (*cur == '\n') {
            (*line)++;
            line_start = cur + 1;
        }
    }

    *column = 1 + (end - line_start);
}

#ifdef FUZZ

static thread_local bool has_issued_error;

void diag_flush(void) {
}

void diag_pause_error_flush(void) {
}

void diag_color_output(bool enable) {
}

void diag_exit_on_error(bool enable) {
}

bool diag_has_issued_error(void) {
    return has_issued_error;
}

void diag_issue(diag_level level, source source, int32_t location, const char* format, ...) {
    va_list v;
    va_start(v, format);
    diag_issue_v(level, source, location, format, v);
    va_end(v);
}

void diag_issue_v(diag_level level, source source, int32_t location, const char* format, va_list v) {
    if (level == DIAG_IGNORED) return;

    fputs(level_names[level], stderr);
    if (source.text == nullptr) {
        fprintf(stderr, ": ");
    } else {
        //fprintf(stderr, ": %s[%" PRIi32 "]: ", source.name, location);
        int32_t line, column;
        seek_line_column(source, location, &line, &column);
        fprintf(stderr, ": %s(%" PRIi32 ",%" PRIi32 "): ", source.name, line, column);
    }

    vfprintf(stderr, format, v);
    fputc('\n', stderr);

    if (level >= DIAG_ERROR) has_issued_error = true;
}

#else

#define DIAG_MAX (16)

typedef struct diag {
    diag_level level;
    source source;
    int32_t location;
    char* message;
} diag;

struct {
    diag group[DIAG_MAX];
    int32_t count;
    bool color_output;
    bool pause_error_flush;
    bool exit_on_error;
    int32_t error_limit, error_count;
} diag_state = {0};

void diag_flush(void) {
    if (diag_state.count <= 0) return;
    diag_level level = diag_state.group[0].level;

    for (int32_t i = 0; i < diag_state.count; i++) {
        diag d = diag_state.group[i];

        if (diag_state.color_output) fputs(level_colors[d.level], stderr);
        fputs(level_names[d.level], stderr);
        if (diag_state.color_output) fputs("\x1b[0m", stderr);

        if (d.source.text == nullptr) {
            fprintf(stderr, ": ");
        } else {
            //fprintf(stderr, ": %s[%" PRIi32 "]: ", d.source.name, d.location);
            int32_t line, column;
            seek_line_column(d.source, d.location, &line, &column);
            fprintf(stderr, ": %s(%" PRIi32 ",%" PRIi32 "): ", d.source.name, line, column);
        }

        fprintf(stderr, "%s\n", d.message);
        free(d.message);
    }

    diag_state.count = 0;

    if (level == DIAG_ERROR) {
        diag_state.error_count++;
        if (diag_state.exit_on_error) exit(1);
    }

    if (level == DIAG_FATAL) abort();
}

void diag_pause_error_flush(void) {
    diag_state.pause_error_flush = true;
}

void diag_color_output(bool enable) {
    diag_state.color_output = enable;
}

void diag_exit_on_error(bool enable) {
    diag_state.exit_on_error = enable;
}

bool diag_has_issued_error(void) {
    return diag_state.error_count > 0;
}

void diag_issue(diag_level level, source source, int32_t location, const char* format, ...) {
    va_list v;
    va_start(v, format);
    diag_issue_v(level, source, location, format, v);
    va_end(v);
}

void diag_issue_v(diag_level level, source source, int32_t location, const char* format, va_list v) {
    if (level == DIAG_IGNORED) return;
    if (level == DIAG_NOTE && diag_state.count == 0) return;

    if (diag_state.count >= DIAG_MAX) diag_flush();
    if (level > DIAG_NOTE && diag_state.count > 0) diag_flush();

    va_list v0, v1;
    va_copy(v0, v);
    va_copy(v1, v);

    long length = vsnprintf(NULL, 0, format, v0);
    va_end(v0);

    char* message = malloc((size_t)(length + 1));
    (void)vsnprintf(message, (size_t)(length + 1), format, v1);
    va_end(v1);

    diag_state.group[diag_state.count++] = (diag) {
        .level = level,
        .source = source,
        .location = location,
        .message = message,
    };

    if (level >= DIAG_ERROR && !diag_state.pause_error_flush) {
        diag_flush();
    }
}

#endif
