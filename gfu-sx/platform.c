/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#if defined(GFUSX_PLATFORM_GLFW)
#  include "platform/glfw3.c"
#else
#  include "platform/null.c"
#endif

#if defined(_WIN32)
#  include "platform/win32.c"
#elif defined(__linux__)
#  include "platform/linux.c"
#endif
