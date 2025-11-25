#if defined(_WIN32)
#  define _GLFW_WIN32
#elif defined(__linux__)
#  define _GLFW_WAYLAND
#  define _GLFW_X11
#endif

#include "../../vendor/glfw3/src/context.c"
#include "../../vendor/glfw3/src/egl_context.c"
#include "../../vendor/glfw3/src/glx_context.c"
#include "../../vendor/glfw3/src/init.c"
#include "../../vendor/glfw3/src/input.c"
#include "../../vendor/glfw3/src/monitor.c"
#include "../../vendor/glfw3/src/platform.c"
#include "../../vendor/glfw3/src/vulkan.c"
#include "../../vendor/glfw3/src/wgl_context.c"
#include "../../vendor/glfw3/src/window.c"
