/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GAMEFU_NOB_BUILD_PLATFORM_H_
#define GAMEFU_NOB_BUILD_PLATFORM_H_


#if defined(_WIN32)
#  define BUILD_PATH_SEP  "\\"
#  define BUILD_EXE_EXT  ".exe"
#  define BUILD_BOOTSTRAP(Cmd, ExeFile, NobFile)  ( \
        nob_cmd_append((Cmd), "cl.exe", nob_temp_sprintf("/Fe%s", (ExeFile)), (NobFile)), \
        nob_cmd_run((Cmd), 0) \
    )
#else
#  define BUILD_PATH_SEP  "/"
#  define BUILD_EXE_EXT  ""
#  define BUILD_BOOTSTRAP(Cmd, ExeFile, NobFile)  ( \
        nob_cmd_append((Cmd), "cc", "-o", (ExeFile), (NobFile)), \
        nob_cmd_run((Cmd), 0) \
    )
#endif /* _WIN32 */


#endif /* GAMEFU_NOB_BUILD_PLATFORM_H_ */
