/* -----------------------------------------------------------------------------
Part of the Choir Compiler Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#define NOB_IMPLEMENTATION
#define NOB_WARN_DEPRECATED
#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_UNSTRIP_PREFIX
#include "nob.h"

#undef nob_shift
#define nob_shift(xs, xs_sz) ((xs_sz) == 0 ? NULL : ((xs_sz)--, *(xs)++))

#include "platform.h"
#include "directories.h"

int
main(int argc, char** argv) {
    int result = 1;

    fprintf(stderr, "argc: %d\n", argc);
    fprintf(stderr, "argv:\n");
    for (int i = 0; i < argc; i++) {
        fprintf(stderr, "  %s\n", argv[i]);
    }

    result = 0;
fail:;
    return result;
}
