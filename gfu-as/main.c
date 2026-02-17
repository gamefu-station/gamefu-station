/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include <gamefu/as.h>

static int gfuas_test(int argc, char** argv);

int
main(int argc, char** argv) {
    if (argc >= 2 && 0 == strcmp("test", argv[1])) {
        return gfuas_test(argc - 2, argv + 2);
    }

    return gfuas_driver_main(argc, argv);
}

static int
gfuas_test(int argc, char** argv) {
    int result = 1;

    gfuas_builder b = {0};
    gfuas_builder_init(&b);

    gfuas_build_label(&b, "_start", false);

    gfuas_builder_assemble(&b);

    result = 0;
fail:;
    gfuas_builder_deinit(&b);
    return result;
}
