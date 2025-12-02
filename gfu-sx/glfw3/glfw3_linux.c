#if defined(_WIN32)
#  define _GLFW_WIN32
#elif defined(__linux__)
#  define _GLFW_WAYLAND
#  define _GLFW_X11
#endif

#if defined(__linux__)
#  include "../../vendor/glfw3/src/linux_joystick.c"
#  include "../../vendor/glfw3/src/posix_module.c"
#  include "../../vendor/glfw3/src/posix_poll.c"
#  include "../../vendor/glfw3/src/posix_thread.c"
#  include "../../vendor/glfw3/src/posix_time.c"
#else
static void _linux_glfw_dummy() { }
#endif
