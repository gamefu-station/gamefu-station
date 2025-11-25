#if defined(_WIN32)
#  define _GLFW_WIN32
#elif defined(__linux__)
#  define _GLFW_WAYLAND
#  define _GLFW_X11
#endif

#if defined(__linux__)
#  include "../../vendor/glfw3/src/x11_init.c"
#  include "../../vendor/glfw3/src/x11_monitor.c"
#  include "../../vendor/glfw3/src/x11_window.c"
#  include "../../vendor/glfw3/src/xkb_unicode.c"
#else
static void _wl_glfw_dummy() { }
#endif
