#include <gamefu/common.h>

#include "register.h"

static struct {
    gfu_gpr gpr;
    const char* name;
    size_t name_length;
} gfu_gpr_names[] = {
#define GPR(Id, Name) { GFU_GPR_##Id, "" Name "", sizeof(Name) - 1 },
#include "x/registers.h"
    {GFU_GPR_INVALID, nullptr, 0},
};

gfu_gpr gfu_gpr_from_string(const char* s, size_t length) {
    for (int i = 0; gfu_gpr_names[i].gpr != GFU_GPR_INVALID; i++) {
        if (length != gfu_gpr_names[i].name_length) continue;
        if (0 == strncmp(s, gfu_gpr_names[i].name, length)) {
            return gfu_gpr_names[i].gpr;
        }
    }

    return GFU_GPR_INVALID;
}

static struct {
    gfu_cp0r cp0r;
    const char* name;
    size_t name_length;
} gfu_cp0r_names[] = {
#define C0R(Id, Name) { GFU_CP0R_##Id, "" Name "", sizeof(Name) - 1 },
#include "x/registers.h"
    {GFU_CP0R_INVALID, nullptr, 0},
};

gfu_cp0r gfu_cp0r_from_string(const char* s, size_t length) {
    for (int i = 0; gfu_cp0r_names[i].cp0r != GFU_CP0R_INVALID; i++) {
        if (length != gfu_cp0r_names[i].name_length) continue;
        if (0 == strncmp(s, gfu_cp0r_names[i].name, length)) {
            return gfu_cp0r_names[i].cp0r;
        }
    }

    return GFU_CP0R_INVALID;
}
