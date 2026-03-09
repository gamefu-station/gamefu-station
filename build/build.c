/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#include "build.h"


static bool
build_obj(
    const char* target_path,
    const char* source_path
) {
    assert(target_path != NULL);
    assert(source_path != NULL);

    bool result = false;
    Cmd cmd = {0};


    if (!cmd_run(&cmd, 0)) goto fail;

success:;
    result = true;
fail:;
    cmd_free(cmd);
    return result;
}


int
main(int argc, char** argv) {
    int result = 1;

success:;
    result = 0;
fail:;
    return result;
}


#define NOB_IMPLEMENTATION
#include "nob.h"
