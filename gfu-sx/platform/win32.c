/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#include "../prologue.h"
#include "../platform.h"

#include <synchapi.h>

void gfusx_platform_sleep(gfusx_t* vm, gfu_udouble nanoseconds) {
    DWORD milliseconds = (DWORD)(nanoseconds / (1000 * 1000));
    if (milliseconds == 0 && nanoseconds != 0) {
        milliseconds = 1;
    }

    Sleep(milliseconds);
}
