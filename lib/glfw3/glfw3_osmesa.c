#if defined(_WIN32)
#  define _GLFW_WIN32
#elif defined(__linux__)
#  define _GLFW_WAYLAND
#  define _GLFW_X11
#endif

#include "../../vendor/glfw3/src/osmesa_context.c"
