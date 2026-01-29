/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GAMEFU_LS_INTERNAL_H_
#define GAMEFU_LS_INTERNAL_H_

#include <gamefu/common.h>
#include <gamefu/ls.h>

#define VERSION "0.1.0"

typedef struct gfuls_state {
    const char* program;
    bool show_help : 1;
    bool show_version : 1;
    bool verbose : 1;
} gfuls_state;

static inline void gfuls_print_verbose(gfuls_state* state, const char* format, ...) {
    if (!state->verbose) return;
    va_list v;
    va_start(v, format);
    vfprintf(stderr, format, v);
    va_end(v);
    fprintf(stderr, "\n");
}

#endif /* GAMEFU_LS_INTERNAL_H_ */
