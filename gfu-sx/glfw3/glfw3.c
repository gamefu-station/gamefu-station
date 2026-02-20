/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#if defined(_WIN32)
#  define _GLFW_WIN32
#elif defined(__linux__)
#  define _GLFW_WAYLAND
#  define _GLFW_X11
#endif

#include "vendor/src/context.c"
#include "vendor/src/egl_context.c"
#include "vendor/src/glx_context.c"
#include "vendor/src/init.c"
#include "vendor/src/input.c"
#include "vendor/src/monitor.c"
#include "vendor/src/platform.c"
#include "vendor/src/vulkan.c"
#include "vendor/src/wgl_context.c"
#include "vendor/src/window.c"
