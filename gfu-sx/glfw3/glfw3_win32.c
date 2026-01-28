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

#if defined(_WIN32)
#  include "../../vendor/glfw3/src/win32_init.c"
#  include "../../vendor/glfw3/src/win32_joystick.c"
#  include "../../vendor/glfw3/src/win32_module.c"
#  include "../../vendor/glfw3/src/win32_monitor.c"
#  include "../../vendor/glfw3/src/win32_thread.c"
#  include "../../vendor/glfw3/src/win32_time.c"
#  include "../../vendor/glfw3/src/win32_window.c"
#else
static void _win32_glfw_dummy(void) { }
#endif
