#ifndef GAMEFU_OPCODES_REGISTER_H_
#define GAMEFU_OPCODES_REGISTER_H_

#include "../common/common.h"

typedef enum gfu_gpr {
#define GPR(Id, Image) GFU_GPR_##Id,
#include "x/registers.h"
    GFU_GPR_INVALID = 0xFF,
} gfu_gpr;

typedef enum gfu_cp0r {
#define C0R(Id, Image) GFU_CP0R_##Id,
#include "x/registers.h"
    GFU_CP0R_INVALID = 0xFF,
} gfu_cp0r;

gfu_gpr gfu_gpr_from_string(const char* s, size_t length);
gfu_cp0r gfu_cp0r_from_string(const char* s, size_t length);

#endif /* GAMEFU_OPCODES_REGISTER_H_ */
