/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GAMEFU_LS_H_
#define GAMEFU_LS_H_

#include <gamefu/common.h>

__GAMEFU_API__ int gfuls_driver_main(int argc, char** argv);
__GAMEFU_API__ int gfuls_driver_fuzz(const char* text, size_t length);

#endif /* GAMEFU_LS_H_ */
