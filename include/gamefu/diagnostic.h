#ifndef FUCC_DIAGNOSTIC_H_
#define FUCC_DIAGNOSTIC_H_

#include "gamefu.h"
#include "source.h"

#include <stdarg.h>

#define NOSOURCE (struct source) {0}, 0

#define fu_assert(Cond) \
    do { \
        if (!(Cond)) diag_issue(DIAG_FATAL, NOSOURCE, "%s:%d: Assertion failed: " #Cond, __FILE__, __LINE__); \
    } while (0)

typedef enum diag_level {
    DIAG_IGNORED,
    DIAG_NOTE,
    DIAG_REMARK,
    DIAG_WARNING,
    DIAG_ERROR,
    DIAG_FATAL,
} diag_level;

void diag_flush(void);
void diag_pause_error_flush(void);
void diag_color_output(bool enable);
void diag_exit_on_error(bool enable);
bool diag_has_issued_error(void);
void diag_issue(diag_level level, source source, int32_t location, const char* format, ...);
void diag_issue_v(diag_level level, source source, int32_t location, const char* format, va_list v);

#endif /* FUCC_DIAGNOSTIC_H_ */
