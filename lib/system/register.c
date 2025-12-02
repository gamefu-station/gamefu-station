#include "../common/common.h"
#include <gamefu/system/register.h>

static struct {
    gfu_gpr_t gpr;
    const char* name;
    size_t name_length;
} gfu_gpr_names[] = {
#define X(Id, Name) { GFU_GPR_##Id, "" Name "", sizeof(Name) - 1 },
    GFU_GPR(X)
#undef X
    {GFU_GPR_INVALID, nullptr, 0},
};

gfu_gpr_t gfu_gpr_from_string(const char* s, size_t length) {
    for (int i = 0; gfu_gpr_names[i].gpr != GFU_GPR_INVALID; i++) {
        if (length != gfu_gpr_names[i].name_length) continue;
        if (0 == strncmp(s, gfu_gpr_names[i].name, length)) {
            return gfu_gpr_names[i].gpr;
        }
    }

    return GFU_GPR_INVALID;
}

static struct {
    gfu_cp0r_t cp0r;
    const char* name;
    size_t name_length;
} gfu_cp0r_names[] = {
#define X(Id, Value, Name) { GFU_CP0R_##Id, "" Name "", sizeof(Name) - 1 },
    GFU_CP0R(X)
#undef X
    {GFU_CP0R_INVALID, nullptr, 0},
};

gfu_cp0r_t gfu_cp0r_from_string(const char* s, size_t length) {
    for (int i = 0; gfu_cp0r_names[i].cp0r != GFU_CP0R_INVALID; i++) {
        if (length != gfu_cp0r_names[i].name_length) continue;
        if (0 == strncmp(s, gfu_cp0r_names[i].name, length)) {
            return gfu_cp0r_names[i].cp0r;
        }
    }

    return GFU_CP0R_INVALID;
}
