#include <gfusx/prologue.h>
#include <gfusx/platform.h>

#include <unistd.h>

void gfusx_platform_sleep(gfusx_t* vm, gfu_ulong_t nanoseconds) {
    useconds_t microseconds = (useconds_t)(nanoseconds / 1000);
    if (microseconds == 0 && nanoseconds != 0) {
        microseconds = 1;
    }

    usleep(microseconds);
}
