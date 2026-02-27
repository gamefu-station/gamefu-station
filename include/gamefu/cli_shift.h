/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#ifndef __GAMEFU__CLI_SHIFT_H__
#define __GAMEFU__CLI_SHIFT_H__


/*
Advances past the current CLI argument and returns it.
If no arguments are left, `NULL` is returned instead.
*/
#define GFU_CLI_SHIFT()  ((argc) == 0 ? NULL : ((argc)--, *((argv)++)))


#endif /* __GAMEFU__CLI_SHIFT_H__ */
