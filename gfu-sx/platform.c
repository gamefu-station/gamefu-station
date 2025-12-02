#if defined(GFUSX_PLATFORM_GLFW)
#  include "platform/glfw3.c"
#else
#  include "null.c"
#endif

#if defined(_WIN32)
#  include "platform/win32.c"
#elif defined(__linux__)
#  include "platform/linux.c"
#endif
