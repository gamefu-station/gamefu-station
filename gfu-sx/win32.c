#include "prologue.h"
#include "platform.h"

#include <synchapi.h>

void gfusx_platform_sleep(gfusx_t* vm, gfu_ulong_t nanoseconds) {
    DWORD milliseconds = (DWORD)(nanoseconds / (1000 * 1000));
    if (milliseconds == 0 && nanoseconds != 0) {
        milliseconds = 1;
    }

    Sleep(milliseconds);
}
