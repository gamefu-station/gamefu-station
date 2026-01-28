/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GFUSX_PLATFORM_H_
#define GFUSX_PLATFORM_H_

#include "prologue.h"

typedef struct gfusx_platform gfusx_platform_t;

bool gfusx_platform_init(gfusx_t* vm);
void gfusx_platform_deinit(gfusx_t* vm);

void gfusx_platform_main_loop(gfusx_t* vm);

void gfusx_platform_sleep(gfusx_t* vm, gfu_udouble nanoseconds);

#endif /* GFUSX_PLATFORM_H_ */
