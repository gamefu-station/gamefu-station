/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#ifndef GAMEFU_PROJECT__BUILD__BUILD_H
#define GAMEFU_PROJECT__BUILD__BUILD_H


#ifndef _GNU_SOURCE
#  define _GNU_SOURCE  1
#endif

#define NOB_WARN_DEPRECATED
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"


#undef nob_shift
#define nob_shift(xs, xs_sz) ((xs_sz) == 0 ? NULL : ((xs_sz)--, *(xs)++))


#endif /* GAMEFU_PROJECT__BUILD__BUILD_H */
