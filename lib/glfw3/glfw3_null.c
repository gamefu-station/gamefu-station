#if defined(_WIN32)
#  define _GLFW_WIN32
#elif defined(__linux__)
#  define _GLFW_WAYLAND
#  define _GLFW_X11
#endif

#include "../../vendor/glfw3/src/null_init.c"
#include "../../vendor/glfw3/src/null_joystick.c"
#include "../../vendor/glfw3/src/null_monitor.c"
#include "../../vendor/glfw3/src/null_window.c"
