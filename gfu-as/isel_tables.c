#define ISEL_MATCH_COUNT 143
static isel_match isel_matches[144] = {
    [0] = {
        .mnemonic = GFUAS_MNEM_BYTES,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_STR,
                .is_constant = false,
                .is_base = false,
                .as.var = "s",
            },
        },
    },
    [1] = {
        .mnemonic = GFUAS_MNEM_BYTES,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "n",
            },
        },
    },
    [2] = {
        .mnemonic = GFUAS_MNEM_NOP,
    },
    [3] = {
        .mnemonic = GFUAS_MNEM_J,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [4] = {
        .mnemonic = GFUAS_MNEM_JMP,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [5] = {
        .mnemonic = GFUAS_MNEM_JAL,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [6] = {
        .mnemonic = GFUAS_MNEM_JAL,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [7] = {
        .mnemonic = GFUAS_MNEM_CALL,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [8] = {
        .mnemonic = GFUAS_MNEM_CALL,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [9] = {
        .mnemonic = GFUAS_MNEM_BEQ,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [10] = {
        .mnemonic = GFUAS_MNEM_BNE,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [11] = {
        .mnemonic = GFUAS_MNEM_BLEZ,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [12] = {
        .mnemonic = GFUAS_MNEM_BGTZ,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "t",
            },
        },
    },
    [13] = {
        .mnemonic = GFUAS_MNEM_ADDI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [14] = {
        .mnemonic = GFUAS_MNEM_ADD,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [15] = {
        .mnemonic = GFUAS_MNEM_ADDIU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [16] = {
        .mnemonic = GFUAS_MNEM_ADDU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [17] = {
        .mnemonic = GFUAS_MNEM_SLTI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [18] = {
        .mnemonic = GFUAS_MNEM_SLT,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [19] = {
        .mnemonic = GFUAS_MNEM_SLTIU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [20] = {
        .mnemonic = GFUAS_MNEM_SLTU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [21] = {
        .mnemonic = GFUAS_MNEM_ANDI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [22] = {
        .mnemonic = GFUAS_MNEM_AND,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [23] = {
        .mnemonic = GFUAS_MNEM_ORI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [24] = {
        .mnemonic = GFUAS_MNEM_OR,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [25] = {
        .mnemonic = GFUAS_MNEM_XORI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [26] = {
        .mnemonic = GFUAS_MNEM_XOR,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [27] = {
        .mnemonic = GFUAS_MNEM_LUI,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [28] = {
        .mnemonic = GFUAS_MNEM_TGEI,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [29] = {
        .mnemonic = GFUAS_MNEM_TGE,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [30] = {
        .mnemonic = GFUAS_MNEM_TGEIU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [31] = {
        .mnemonic = GFUAS_MNEM_TGEU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [32] = {
        .mnemonic = GFUAS_MNEM_TLTI,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [33] = {
        .mnemonic = GFUAS_MNEM_TLT,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [34] = {
        .mnemonic = GFUAS_MNEM_TLTIU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [35] = {
        .mnemonic = GFUAS_MNEM_TLTU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [36] = {
        .mnemonic = GFUAS_MNEM_TEQI,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [37] = {
        .mnemonic = GFUAS_MNEM_TEQ,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [38] = {
        .mnemonic = GFUAS_MNEM_TNEI,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [39] = {
        .mnemonic = GFUAS_MNEM_TNE,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [40] = {
        .mnemonic = GFUAS_MNEM_LB,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [41] = {
        .mnemonic = GFUAS_MNEM_LB,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "l",
            },
        },
    },
    [42] = {
        .mnemonic = GFUAS_MNEM_LH,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [43] = {
        .mnemonic = GFUAS_MNEM_LH,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "l",
            },
        },
    },
    [44] = {
        .mnemonic = GFUAS_MNEM_LWL,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [45] = {
        .mnemonic = GFUAS_MNEM_LWL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "l",
            },
        },
    },
    [46] = {
        .mnemonic = GFUAS_MNEM_LW,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [47] = {
        .mnemonic = GFUAS_MNEM_LW,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "l",
            },
        },
    },
    [48] = {
        .mnemonic = GFUAS_MNEM_LW,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [49] = {
        .mnemonic = GFUAS_MNEM_LBU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [50] = {
        .mnemonic = GFUAS_MNEM_LBU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "l",
            },
        },
    },
    [51] = {
        .mnemonic = GFUAS_MNEM_LHU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [52] = {
        .mnemonic = GFUAS_MNEM_LHU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "l",
            },
        },
    },
    [53] = {
        .mnemonic = GFUAS_MNEM_LWR,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [54] = {
        .mnemonic = GFUAS_MNEM_LWR,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "l",
            },
        },
    },
    [55] = {
        .mnemonic = GFUAS_MNEM_SB,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [56] = {
        .mnemonic = GFUAS_MNEM_SB,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [57] = {
        .mnemonic = GFUAS_MNEM_SH,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [58] = {
        .mnemonic = GFUAS_MNEM_SH,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [59] = {
        .mnemonic = GFUAS_MNEM_SH,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [60] = {
        .mnemonic = GFUAS_MNEM_SH,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [61] = {
        .mnemonic = GFUAS_MNEM_SH,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [62] = {
        .mnemonic = GFUAS_MNEM_SH,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [63] = {
        .mnemonic = GFUAS_MNEM_SWL,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [64] = {
        .mnemonic = GFUAS_MNEM_SWL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [65] = {
        .mnemonic = GFUAS_MNEM_SW,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [66] = {
        .mnemonic = GFUAS_MNEM_SW,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [67] = {
        .mnemonic = GFUAS_MNEM_SW,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [68] = {
        .mnemonic = GFUAS_MNEM_SW,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [69] = {
        .mnemonic = GFUAS_MNEM_SW,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [70] = {
        .mnemonic = GFUAS_MNEM_SWR,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [71] = {
        .mnemonic = GFUAS_MNEM_SWR,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = true,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [72] = {
        .mnemonic = GFUAS_MNEM_BLTZ,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [73] = {
        .mnemonic = GFUAS_MNEM_BGEZ,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [74] = {
        .mnemonic = GFUAS_MNEM_BLTZAL,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [75] = {
        .mnemonic = GFUAS_MNEM_BGEZAL,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [76] = {
        .mnemonic = GFUAS_MNEM_BREAK,
    },
    [77] = {
        .mnemonic = GFUAS_MNEM_BREAK,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "c",
            },
        },
    },
    [78] = {
        .mnemonic = GFUAS_MNEM_SDBBP,
    },
    [79] = {
        .mnemonic = GFUAS_MNEM_SDBBP,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "c",
            },
        },
    },
    [80] = {
        .mnemonic = GFUAS_MNEM_SLL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "s",
            },
        },
    },
    [81] = {
        .mnemonic = GFUAS_MNEM_SRL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "s",
            },
        },
    },
    [82] = {
        .mnemonic = GFUAS_MNEM_SRA,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "s",
            },
        },
    },
    [83] = {
        .mnemonic = GFUAS_MNEM_SLLV,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [84] = {
        .mnemonic = GFUAS_MNEM_SLL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [85] = {
        .mnemonic = GFUAS_MNEM_SRLV,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [86] = {
        .mnemonic = GFUAS_MNEM_SRL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [87] = {
        .mnemonic = GFUAS_MNEM_SRAV,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [88] = {
        .mnemonic = GFUAS_MNEM_SRA,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [89] = {
        .mnemonic = GFUAS_MNEM_JR,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [90] = {
        .mnemonic = GFUAS_MNEM_RET,
    },
    [91] = {
        .mnemonic = GFUAS_MNEM_JALR,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [92] = {
        .mnemonic = GFUAS_MNEM_JALR,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [93] = {
        .mnemonic = GFUAS_MNEM_MOVZ,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [94] = {
        .mnemonic = GFUAS_MNEM_MOVZI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [95] = {
        .mnemonic = GFUAS_MNEM_MOVE,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [96] = {
        .mnemonic = GFUAS_MNEM_MOVN,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [97] = {
        .mnemonic = GFUAS_MNEM_MOVNI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [98] = {
        .mnemonic = GFUAS_MNEM_SYSCALL,
    },
    [99] = {
        .mnemonic = GFUAS_MNEM_MFHI,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
        },
    },
    [100] = {
        .mnemonic = GFUAS_MNEM_MTHI,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [101] = {
        .mnemonic = GFUAS_MNEM_MFLO,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
        },
    },
    [102] = {
        .mnemonic = GFUAS_MNEM_MTLO,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [103] = {
        .mnemonic = GFUAS_MNEM_MADD,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [104] = {
        .mnemonic = GFUAS_MNEM_MADDU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [105] = {
        .mnemonic = GFUAS_MNEM_MSUB,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [106] = {
        .mnemonic = GFUAS_MNEM_MSUBU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [107] = {
        .mnemonic = GFUAS_MNEM_MULT,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [108] = {
        .mnemonic = GFUAS_MNEM_MULTU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [109] = {
        .mnemonic = GFUAS_MNEM_DIV,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [110] = {
        .mnemonic = GFUAS_MNEM_DIVU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [111] = {
        .mnemonic = GFUAS_MNEM_MUL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [112] = {
        .mnemonic = GFUAS_MNEM_MUL,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [113] = {
        .mnemonic = GFUAS_MNEM_ADD,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [114] = {
        .mnemonic = GFUAS_MNEM_ADD,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [115] = {
        .mnemonic = GFUAS_MNEM_ADDU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [116] = {
        .mnemonic = GFUAS_MNEM_SUB,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [117] = {
        .mnemonic = GFUAS_MNEM_SUBI,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [118] = {
        .mnemonic = GFUAS_MNEM_SUBU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [119] = {
        .mnemonic = GFUAS_MNEM_AND,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [120] = {
        .mnemonic = GFUAS_MNEM_OR,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [121] = {
        .mnemonic = GFUAS_MNEM_XOR,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [122] = {
        .mnemonic = GFUAS_MNEM_NOR,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [123] = {
        .mnemonic = GFUAS_MNEM_CLZ,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [124] = {
        .mnemonic = GFUAS_MNEM_CLO,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [125] = {
        .mnemonic = GFUAS_MNEM_SLT,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [126] = {
        .mnemonic = GFUAS_MNEM_SGT,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [127] = {
        .mnemonic = GFUAS_MNEM_SLTU,
        .operand_count = 3,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [128] = {
        .mnemonic = GFUAS_MNEM_TGE,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [129] = {
        .mnemonic = GFUAS_MNEM_TGEU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [130] = {
        .mnemonic = GFUAS_MNEM_TLT,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [131] = {
        .mnemonic = GFUAS_MNEM_TLTU,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [132] = {
        .mnemonic = GFUAS_MNEM_TEQ,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [133] = {
        .mnemonic = GFUAS_MNEM_TNE,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "r",
            },
        },
    },
    [134] = {
        .mnemonic = GFUAS_MNEM_LI,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [135] = {
        .mnemonic = GFUAS_MNEM_LA,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_ADDR,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [136] = {
        .mnemonic = GFUAS_MNEM_STAL,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [137] = {
        .mnemonic = GFUAS_MNEM_STDAL,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "v",
            },
        },
    },
    [138] = {
        .mnemonic = GFUAS_MNEM_PUSH,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
        },
    },
    [139] = {
        .mnemonic = GFUAS_MNEM_POP,
        .operand_count = 1,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
        },
    },
    [140] = {
        .mnemonic = GFUAS_MNEM_MFC0,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [141] = {
        .mnemonic = GFUAS_MNEM_MTC0,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_REG,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    [142] = {
        .mnemonic = GFUAS_MNEM_MTC0,
        .operand_count = 2,
        .operands = {
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "d",
            },
            {
                .type = ISEL_TY_IMM,
                .is_constant = false,
                .is_base = false,
                .as.var = "l",
            },
        },
    },
    {0},
};

#define ISEL_EMIT_COUNT 170
static isel_emit isel_emits[171] = {
    [0] = {
        .kind = ISEL_EMIT_BYTES,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [1] = {
        .kind = ISEL_EMIT_BYTES,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [2] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SLL,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [3] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_J,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 0,
            },
        },
    },
    [4] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_J,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 0,
            },
        },
    },
    [5] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_JAL,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 0,
            },
        },
    },
    [6] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_JAL,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 0,
            },
        },
    },
    [7] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_JAL,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 0,
            },
        },
    },
    [8] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_JAL,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 0,
            },
        },
    },
    [9] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BEQ,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR_OFFS,
                .as.var_index = 2,
            },
        },
    },
    [10] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BNE,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR_OFFS,
                .as.var_index = 2,
            },
        },
    },
    [11] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BLEZ,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_OFFS,
                .as.var_index = 1,
            },
        },
    },
    [12] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BGTZ,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_OFFS,
                .as.var_index = 1,
            },
        },
    },
    [13] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [14] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [15] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [16] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [17] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SLTI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [18] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SLTI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [19] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SLTIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [20] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SLTIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [21] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ANDI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [22] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ANDI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [23] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [24] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [25] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_XORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [26] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_XORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [27] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [28] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TGEI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [29] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TGEI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [30] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TGEIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [31] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TGEIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [32] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TLTI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [33] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TLTI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [34] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TLTIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [35] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TLTIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [36] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TEQI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [37] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TEQI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [38] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TNEI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [39] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_TNEI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [40] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LB,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [41] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LB,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [42] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [43] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [44] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LWL,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [45] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LWL,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [46] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [47] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [48] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 1,
            },
        },
    },
    [49] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [50] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [51] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LBU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [52] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LBU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [53] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LHU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [54] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LHU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [55] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LWR,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [56] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LWR,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
        },
    },
    [57] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SB,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [58] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SB,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [59] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [60] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 0,
            },
        },
    },
    [61] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 0,
            },
        },
    },
    [62] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [63] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 0,
            },
        },
    },
    [64] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 0,
            },
        },
    },
    [65] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [66] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [67] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [68] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [69] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 2,
            },
        },
    },
    [70] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SH,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [71] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SWL,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [72] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SWL,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [73] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [74] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 1,
            },
        },
    },
    [75] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [76] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [77] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [78] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 2,
            },
        },
    },
    [79] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 2,
            },
        },
    },
    [80] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [81] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 0,
            },
        },
    },
    [82] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 0,
            },
        },
    },
    [83] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [84] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SWR,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [85] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SWR,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [86] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BLTZ,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 1,
            },
        },
    },
    [87] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BGEZ,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 1,
            },
        },
    },
    [88] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BLTZAL,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 1,
            },
        },
    },
    [89] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_BGEZAL,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_ADDR,
                .as.var_index = 1,
            },
        },
    },
    [90] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_BREAK,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [91] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_BREAK,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [92] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_SDBBP,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [93] = {
        .kind = ISEL_EMIT_ADDR,
        .op.opcode = GFU_OP_SDBBP,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [94] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SLL,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [95] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SRL,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [96] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SRA,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [97] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SLLV,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [98] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SLLV,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [99] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SRLV,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [100] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SRLV,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [101] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SRAV,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [102] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SRAV,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [103] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_JR,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [104] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_JR,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_RA,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [105] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_JALR,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_RA,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [106] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_JALR,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [107] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MOVZ,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [108] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 1,
            },
        },
    },
    [109] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [110] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MOVZ,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [111] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MOVZ,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [112] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MOVN,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [113] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 1,
            },
        },
    },
    [114] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [115] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MOVN,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [116] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SYSCALL,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [117] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MFHI,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [118] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MTHI,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [119] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MFLO,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [120] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MTLO,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [121] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MADD,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [122] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MADDU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [123] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MSUB,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [124] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MSUBU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [125] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MULT,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [126] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MULTU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [127] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_DIV,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [128] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_DIVU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [129] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MUL,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [130] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
        },
    },
    [131] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_MUL,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [132] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_ADD,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [133] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_ADD,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [134] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_ADDU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [135] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SUB,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [136] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 2,
            },
        },
    },
    [137] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 2,
            },
        },
    },
    [138] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SUB,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
        },
    },
    [139] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SUBU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [140] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_AND,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [141] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_OR,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [142] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_XOR,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [143] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_NOR,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [144] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_CLZ,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [145] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_CLO,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [146] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SLT,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [147] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SLT,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [148] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_SLTU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 2,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [149] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_TGE,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [150] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_TGEU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [151] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_TLT,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [152] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_TLTU,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [153] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_TEQ,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [154] = {
        .kind = ISEL_EMIT_REG,
        .op.function = GFU_OPFN_TNE,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [155] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 1,
            },
        },
    },
    [156] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [157] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 1,
            },
        },
    },
    [158] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [159] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_VAR_NEGATE,
            },
        },
    },
    [160] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    [161] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = -4,
            },
        },
    },
    [162] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_SW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [163] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LW,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [164] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ADDIU,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_SP,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 4,
            },
        },
    },
    [165] = {
        .kind = ISEL_EMIT_COP0,
        .op.opcode = GFU_OP_SPECIAL,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [166] = {
        .kind = ISEL_EMIT_COP0,
        .op.opcode = GFU_OP_BEQ,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 1,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    [167] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_LUI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_VAR_UPPER,
                .as.var_index = 1,
            },
        },
    },
    [168] = {
        .kind = ISEL_EMIT_IMM,
        .op.opcode = GFU_OP_ORI,
        .argument_count = 3,
        .arguments = {
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_VAR_LOWER,
                .as.var_index = 1,
            },
        },
    },
    [169] = {
        .kind = ISEL_EMIT_COP0,
        .op.opcode = GFU_OP_BEQ,
        .argument_count = 4,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_AT,
            },
            {
                .kind = ISEL_ARG_REGISTER,
                .as._register = GFU_GPR_ZERO,
            },
            {
                .kind = ISEL_ARG_IMM,
                .as.immediate = 0,
            },
        },
    },
    {0},
};

#define ISEL_PATTERN_COUNT 143
static isel_pattern isel_patterns[144] = {
    [0] = {
        .var_count = 1,
        .match_index = 0,
        .match_count = 1,
        .emit_index = 0,
        .emit_count = 1,
    },
    [1] = {
        .var_count = 1,
        .match_index = 1,
        .match_count = 1,
        .emit_index = 1,
        .emit_count = 1,
    },
    [2] = {
        .var_count = 0,
        .match_index = 2,
        .match_count = 1,
        .emit_index = 2,
        .emit_count = 1,
    },
    [3] = {
        .var_count = 1,
        .match_index = 3,
        .match_count = 1,
        .emit_index = 3,
        .emit_count = 1,
    },
    [4] = {
        .var_count = 1,
        .match_index = 4,
        .match_count = 1,
        .emit_index = 4,
        .emit_count = 1,
    },
    [5] = {
        .var_count = 1,
        .match_index = 5,
        .match_count = 1,
        .emit_index = 5,
        .emit_count = 1,
    },
    [6] = {
        .var_count = 1,
        .match_index = 6,
        .match_count = 1,
        .emit_index = 6,
        .emit_count = 1,
    },
    [7] = {
        .var_count = 1,
        .match_index = 7,
        .match_count = 1,
        .emit_index = 7,
        .emit_count = 1,
    },
    [8] = {
        .var_count = 1,
        .match_index = 8,
        .match_count = 1,
        .emit_index = 8,
        .emit_count = 1,
    },
    [9] = {
        .var_count = 3,
        .match_index = 9,
        .match_count = 1,
        .emit_index = 9,
        .emit_count = 1,
    },
    [10] = {
        .var_count = 3,
        .match_index = 10,
        .match_count = 1,
        .emit_index = 10,
        .emit_count = 1,
    },
    [11] = {
        .var_count = 2,
        .match_index = 11,
        .match_count = 1,
        .emit_index = 11,
        .emit_count = 1,
    },
    [12] = {
        .var_count = 2,
        .match_index = 12,
        .match_count = 1,
        .emit_index = 12,
        .emit_count = 1,
    },
    [13] = {
        .var_count = 3,
        .match_index = 13,
        .match_count = 1,
        .emit_index = 13,
        .emit_count = 1,
    },
    [14] = {
        .var_count = 3,
        .match_index = 14,
        .match_count = 1,
        .emit_index = 14,
        .emit_count = 1,
    },
    [15] = {
        .var_count = 3,
        .match_index = 15,
        .match_count = 1,
        .emit_index = 15,
        .emit_count = 1,
    },
    [16] = {
        .var_count = 3,
        .match_index = 16,
        .match_count = 1,
        .emit_index = 16,
        .emit_count = 1,
    },
    [17] = {
        .var_count = 3,
        .match_index = 17,
        .match_count = 1,
        .emit_index = 17,
        .emit_count = 1,
    },
    [18] = {
        .var_count = 3,
        .match_index = 18,
        .match_count = 1,
        .emit_index = 18,
        .emit_count = 1,
    },
    [19] = {
        .var_count = 3,
        .match_index = 19,
        .match_count = 1,
        .emit_index = 19,
        .emit_count = 1,
    },
    [20] = {
        .var_count = 3,
        .match_index = 20,
        .match_count = 1,
        .emit_index = 20,
        .emit_count = 1,
    },
    [21] = {
        .var_count = 3,
        .match_index = 21,
        .match_count = 1,
        .emit_index = 21,
        .emit_count = 1,
    },
    [22] = {
        .var_count = 3,
        .match_index = 22,
        .match_count = 1,
        .emit_index = 22,
        .emit_count = 1,
    },
    [23] = {
        .var_count = 3,
        .match_index = 23,
        .match_count = 1,
        .emit_index = 23,
        .emit_count = 1,
    },
    [24] = {
        .var_count = 3,
        .match_index = 24,
        .match_count = 1,
        .emit_index = 24,
        .emit_count = 1,
    },
    [25] = {
        .var_count = 3,
        .match_index = 25,
        .match_count = 1,
        .emit_index = 25,
        .emit_count = 1,
    },
    [26] = {
        .var_count = 3,
        .match_index = 26,
        .match_count = 1,
        .emit_index = 26,
        .emit_count = 1,
    },
    [27] = {
        .var_count = 2,
        .match_index = 27,
        .match_count = 1,
        .emit_index = 27,
        .emit_count = 1,
    },
    [28] = {
        .var_count = 2,
        .match_index = 28,
        .match_count = 1,
        .emit_index = 28,
        .emit_count = 1,
    },
    [29] = {
        .var_count = 2,
        .match_index = 29,
        .match_count = 1,
        .emit_index = 29,
        .emit_count = 1,
    },
    [30] = {
        .var_count = 2,
        .match_index = 30,
        .match_count = 1,
        .emit_index = 30,
        .emit_count = 1,
    },
    [31] = {
        .var_count = 2,
        .match_index = 31,
        .match_count = 1,
        .emit_index = 31,
        .emit_count = 1,
    },
    [32] = {
        .var_count = 2,
        .match_index = 32,
        .match_count = 1,
        .emit_index = 32,
        .emit_count = 1,
    },
    [33] = {
        .var_count = 2,
        .match_index = 33,
        .match_count = 1,
        .emit_index = 33,
        .emit_count = 1,
    },
    [34] = {
        .var_count = 2,
        .match_index = 34,
        .match_count = 1,
        .emit_index = 34,
        .emit_count = 1,
    },
    [35] = {
        .var_count = 2,
        .match_index = 35,
        .match_count = 1,
        .emit_index = 35,
        .emit_count = 1,
    },
    [36] = {
        .var_count = 2,
        .match_index = 36,
        .match_count = 1,
        .emit_index = 36,
        .emit_count = 1,
    },
    [37] = {
        .var_count = 2,
        .match_index = 37,
        .match_count = 1,
        .emit_index = 37,
        .emit_count = 1,
    },
    [38] = {
        .var_count = 2,
        .match_index = 38,
        .match_count = 1,
        .emit_index = 38,
        .emit_count = 1,
    },
    [39] = {
        .var_count = 2,
        .match_index = 39,
        .match_count = 1,
        .emit_index = 39,
        .emit_count = 1,
    },
    [40] = {
        .var_count = 2,
        .match_index = 40,
        .match_count = 1,
        .emit_index = 40,
        .emit_count = 1,
    },
    [41] = {
        .var_count = 3,
        .match_index = 41,
        .match_count = 1,
        .emit_index = 41,
        .emit_count = 1,
    },
    [42] = {
        .var_count = 2,
        .match_index = 42,
        .match_count = 1,
        .emit_index = 42,
        .emit_count = 1,
    },
    [43] = {
        .var_count = 3,
        .match_index = 43,
        .match_count = 1,
        .emit_index = 43,
        .emit_count = 1,
    },
    [44] = {
        .var_count = 2,
        .match_index = 44,
        .match_count = 1,
        .emit_index = 44,
        .emit_count = 1,
    },
    [45] = {
        .var_count = 3,
        .match_index = 45,
        .match_count = 1,
        .emit_index = 45,
        .emit_count = 1,
    },
    [46] = {
        .var_count = 2,
        .match_index = 46,
        .match_count = 1,
        .emit_index = 46,
        .emit_count = 1,
    },
    [47] = {
        .var_count = 3,
        .match_index = 47,
        .match_count = 1,
        .emit_index = 47,
        .emit_count = 1,
    },
    [48] = {
        .var_count = 2,
        .match_index = 48,
        .match_count = 1,
        .emit_index = 48,
        .emit_count = 3,
    },
    [49] = {
        .var_count = 2,
        .match_index = 49,
        .match_count = 1,
        .emit_index = 51,
        .emit_count = 1,
    },
    [50] = {
        .var_count = 3,
        .match_index = 50,
        .match_count = 1,
        .emit_index = 52,
        .emit_count = 1,
    },
    [51] = {
        .var_count = 2,
        .match_index = 51,
        .match_count = 1,
        .emit_index = 53,
        .emit_count = 1,
    },
    [52] = {
        .var_count = 3,
        .match_index = 52,
        .match_count = 1,
        .emit_index = 54,
        .emit_count = 1,
    },
    [53] = {
        .var_count = 2,
        .match_index = 53,
        .match_count = 1,
        .emit_index = 55,
        .emit_count = 1,
    },
    [54] = {
        .var_count = 3,
        .match_index = 54,
        .match_count = 1,
        .emit_index = 56,
        .emit_count = 1,
    },
    [55] = {
        .var_count = 2,
        .match_index = 55,
        .match_count = 1,
        .emit_index = 57,
        .emit_count = 1,
    },
    [56] = {
        .var_count = 3,
        .match_index = 56,
        .match_count = 1,
        .emit_index = 58,
        .emit_count = 1,
    },
    [57] = {
        .var_count = 2,
        .match_index = 57,
        .match_count = 1,
        .emit_index = 59,
        .emit_count = 1,
    },
    [58] = {
        .var_count = 2,
        .match_index = 58,
        .match_count = 1,
        .emit_index = 60,
        .emit_count = 3,
    },
    [59] = {
        .var_count = 2,
        .match_index = 59,
        .match_count = 1,
        .emit_index = 63,
        .emit_count = 3,
    },
    [60] = {
        .var_count = 2,
        .match_index = 60,
        .match_count = 1,
        .emit_index = 66,
        .emit_count = 2,
    },
    [61] = {
        .var_count = 3,
        .match_index = 61,
        .match_count = 1,
        .emit_index = 68,
        .emit_count = 1,
    },
    [62] = {
        .var_count = 3,
        .match_index = 62,
        .match_count = 1,
        .emit_index = 69,
        .emit_count = 2,
    },
    [63] = {
        .var_count = 2,
        .match_index = 63,
        .match_count = 1,
        .emit_index = 71,
        .emit_count = 1,
    },
    [64] = {
        .var_count = 3,
        .match_index = 64,
        .match_count = 1,
        .emit_index = 72,
        .emit_count = 1,
    },
    [65] = {
        .var_count = 2,
        .match_index = 65,
        .match_count = 1,
        .emit_index = 73,
        .emit_count = 1,
    },
    [66] = {
        .var_count = 2,
        .match_index = 66,
        .match_count = 1,
        .emit_index = 74,
        .emit_count = 3,
    },
    [67] = {
        .var_count = 3,
        .match_index = 67,
        .match_count = 1,
        .emit_index = 77,
        .emit_count = 1,
    },
    [68] = {
        .var_count = 3,
        .match_index = 68,
        .match_count = 1,
        .emit_index = 78,
        .emit_count = 3,
    },
    [69] = {
        .var_count = 2,
        .match_index = 69,
        .match_count = 1,
        .emit_index = 81,
        .emit_count = 3,
    },
    [70] = {
        .var_count = 2,
        .match_index = 70,
        .match_count = 1,
        .emit_index = 84,
        .emit_count = 1,
    },
    [71] = {
        .var_count = 3,
        .match_index = 71,
        .match_count = 1,
        .emit_index = 85,
        .emit_count = 1,
    },
    [72] = {
        .var_count = 2,
        .match_index = 72,
        .match_count = 1,
        .emit_index = 86,
        .emit_count = 1,
    },
    [73] = {
        .var_count = 2,
        .match_index = 73,
        .match_count = 1,
        .emit_index = 87,
        .emit_count = 1,
    },
    [74] = {
        .var_count = 2,
        .match_index = 74,
        .match_count = 1,
        .emit_index = 88,
        .emit_count = 1,
    },
    [75] = {
        .var_count = 2,
        .match_index = 75,
        .match_count = 1,
        .emit_index = 89,
        .emit_count = 1,
    },
    [76] = {
        .var_count = 0,
        .match_index = 76,
        .match_count = 1,
        .emit_index = 90,
        .emit_count = 1,
    },
    [77] = {
        .var_count = 1,
        .match_index = 77,
        .match_count = 1,
        .emit_index = 91,
        .emit_count = 1,
    },
    [78] = {
        .var_count = 0,
        .match_index = 78,
        .match_count = 1,
        .emit_index = 92,
        .emit_count = 1,
    },
    [79] = {
        .var_count = 1,
        .match_index = 79,
        .match_count = 1,
        .emit_index = 93,
        .emit_count = 1,
    },
    [80] = {
        .var_count = 3,
        .match_index = 80,
        .match_count = 1,
        .emit_index = 94,
        .emit_count = 1,
    },
    [81] = {
        .var_count = 3,
        .match_index = 81,
        .match_count = 1,
        .emit_index = 95,
        .emit_count = 1,
    },
    [82] = {
        .var_count = 3,
        .match_index = 82,
        .match_count = 1,
        .emit_index = 96,
        .emit_count = 1,
    },
    [83] = {
        .var_count = 3,
        .match_index = 83,
        .match_count = 1,
        .emit_index = 97,
        .emit_count = 1,
    },
    [84] = {
        .var_count = 3,
        .match_index = 84,
        .match_count = 1,
        .emit_index = 98,
        .emit_count = 1,
    },
    [85] = {
        .var_count = 3,
        .match_index = 85,
        .match_count = 1,
        .emit_index = 99,
        .emit_count = 1,
    },
    [86] = {
        .var_count = 3,
        .match_index = 86,
        .match_count = 1,
        .emit_index = 100,
        .emit_count = 1,
    },
    [87] = {
        .var_count = 3,
        .match_index = 87,
        .match_count = 1,
        .emit_index = 101,
        .emit_count = 1,
    },
    [88] = {
        .var_count = 3,
        .match_index = 88,
        .match_count = 1,
        .emit_index = 102,
        .emit_count = 1,
    },
    [89] = {
        .var_count = 1,
        .match_index = 89,
        .match_count = 1,
        .emit_index = 103,
        .emit_count = 1,
    },
    [90] = {
        .var_count = 0,
        .match_index = 90,
        .match_count = 1,
        .emit_index = 104,
        .emit_count = 1,
    },
    [91] = {
        .var_count = 1,
        .match_index = 91,
        .match_count = 1,
        .emit_index = 105,
        .emit_count = 1,
    },
    [92] = {
        .var_count = 2,
        .match_index = 92,
        .match_count = 1,
        .emit_index = 106,
        .emit_count = 1,
    },
    [93] = {
        .var_count = 3,
        .match_index = 93,
        .match_count = 1,
        .emit_index = 107,
        .emit_count = 1,
    },
    [94] = {
        .var_count = 3,
        .match_index = 94,
        .match_count = 1,
        .emit_index = 108,
        .emit_count = 3,
    },
    [95] = {
        .var_count = 2,
        .match_index = 95,
        .match_count = 1,
        .emit_index = 111,
        .emit_count = 1,
    },
    [96] = {
        .var_count = 3,
        .match_index = 96,
        .match_count = 1,
        .emit_index = 112,
        .emit_count = 1,
    },
    [97] = {
        .var_count = 3,
        .match_index = 97,
        .match_count = 1,
        .emit_index = 113,
        .emit_count = 3,
    },
    [98] = {
        .var_count = 0,
        .match_index = 98,
        .match_count = 1,
        .emit_index = 116,
        .emit_count = 1,
    },
    [99] = {
        .var_count = 1,
        .match_index = 99,
        .match_count = 1,
        .emit_index = 117,
        .emit_count = 1,
    },
    [100] = {
        .var_count = 1,
        .match_index = 100,
        .match_count = 1,
        .emit_index = 118,
        .emit_count = 1,
    },
    [101] = {
        .var_count = 1,
        .match_index = 101,
        .match_count = 1,
        .emit_index = 119,
        .emit_count = 1,
    },
    [102] = {
        .var_count = 1,
        .match_index = 102,
        .match_count = 1,
        .emit_index = 120,
        .emit_count = 1,
    },
    [103] = {
        .var_count = 2,
        .match_index = 103,
        .match_count = 1,
        .emit_index = 121,
        .emit_count = 1,
    },
    [104] = {
        .var_count = 2,
        .match_index = 104,
        .match_count = 1,
        .emit_index = 122,
        .emit_count = 1,
    },
    [105] = {
        .var_count = 2,
        .match_index = 105,
        .match_count = 1,
        .emit_index = 123,
        .emit_count = 1,
    },
    [106] = {
        .var_count = 2,
        .match_index = 106,
        .match_count = 1,
        .emit_index = 124,
        .emit_count = 1,
    },
    [107] = {
        .var_count = 2,
        .match_index = 107,
        .match_count = 1,
        .emit_index = 125,
        .emit_count = 1,
    },
    [108] = {
        .var_count = 2,
        .match_index = 108,
        .match_count = 1,
        .emit_index = 126,
        .emit_count = 1,
    },
    [109] = {
        .var_count = 2,
        .match_index = 109,
        .match_count = 1,
        .emit_index = 127,
        .emit_count = 1,
    },
    [110] = {
        .var_count = 2,
        .match_index = 110,
        .match_count = 1,
        .emit_index = 128,
        .emit_count = 1,
    },
    [111] = {
        .var_count = 3,
        .match_index = 111,
        .match_count = 1,
        .emit_index = 129,
        .emit_count = 1,
    },
    [112] = {
        .var_count = 3,
        .match_index = 112,
        .match_count = 1,
        .emit_index = 130,
        .emit_count = 2,
    },
    [113] = {
        .var_count = 3,
        .match_index = 113,
        .match_count = 1,
        .emit_index = 132,
        .emit_count = 1,
    },
    [114] = {
        .var_count = 2,
        .match_index = 114,
        .match_count = 1,
        .emit_index = 133,
        .emit_count = 1,
    },
    [115] = {
        .var_count = 3,
        .match_index = 115,
        .match_count = 1,
        .emit_index = 134,
        .emit_count = 1,
    },
    [116] = {
        .var_count = 3,
        .match_index = 116,
        .match_count = 1,
        .emit_index = 135,
        .emit_count = 1,
    },
    [117] = {
        .var_count = 3,
        .match_index = 117,
        .match_count = 1,
        .emit_index = 136,
        .emit_count = 3,
    },
    [118] = {
        .var_count = 3,
        .match_index = 118,
        .match_count = 1,
        .emit_index = 139,
        .emit_count = 1,
    },
    [119] = {
        .var_count = 3,
        .match_index = 119,
        .match_count = 1,
        .emit_index = 140,
        .emit_count = 1,
    },
    [120] = {
        .var_count = 3,
        .match_index = 120,
        .match_count = 1,
        .emit_index = 141,
        .emit_count = 1,
    },
    [121] = {
        .var_count = 3,
        .match_index = 121,
        .match_count = 1,
        .emit_index = 142,
        .emit_count = 1,
    },
    [122] = {
        .var_count = 3,
        .match_index = 122,
        .match_count = 1,
        .emit_index = 143,
        .emit_count = 1,
    },
    [123] = {
        .var_count = 2,
        .match_index = 123,
        .match_count = 1,
        .emit_index = 144,
        .emit_count = 1,
    },
    [124] = {
        .var_count = 2,
        .match_index = 124,
        .match_count = 1,
        .emit_index = 145,
        .emit_count = 1,
    },
    [125] = {
        .var_count = 3,
        .match_index = 125,
        .match_count = 1,
        .emit_index = 146,
        .emit_count = 1,
    },
    [126] = {
        .var_count = 3,
        .match_index = 126,
        .match_count = 1,
        .emit_index = 147,
        .emit_count = 1,
    },
    [127] = {
        .var_count = 3,
        .match_index = 127,
        .match_count = 1,
        .emit_index = 148,
        .emit_count = 1,
    },
    [128] = {
        .var_count = 2,
        .match_index = 128,
        .match_count = 1,
        .emit_index = 149,
        .emit_count = 1,
    },
    [129] = {
        .var_count = 2,
        .match_index = 129,
        .match_count = 1,
        .emit_index = 150,
        .emit_count = 1,
    },
    [130] = {
        .var_count = 2,
        .match_index = 130,
        .match_count = 1,
        .emit_index = 151,
        .emit_count = 1,
    },
    [131] = {
        .var_count = 2,
        .match_index = 131,
        .match_count = 1,
        .emit_index = 152,
        .emit_count = 1,
    },
    [132] = {
        .var_count = 2,
        .match_index = 132,
        .match_count = 1,
        .emit_index = 153,
        .emit_count = 1,
    },
    [133] = {
        .var_count = 2,
        .match_index = 133,
        .match_count = 1,
        .emit_index = 154,
        .emit_count = 1,
    },
    [134] = {
        .var_count = 2,
        .match_index = 134,
        .match_count = 1,
        .emit_index = 155,
        .emit_count = 2,
    },
    [135] = {
        .var_count = 2,
        .match_index = 135,
        .match_count = 1,
        .emit_index = 157,
        .emit_count = 2,
    },
    [136] = {
        .var_count = 1,
        .match_index = 136,
        .match_count = 1,
        .emit_index = 159,
        .emit_count = 1,
    },
    [137] = {
        .var_count = 1,
        .match_index = 137,
        .match_count = 1,
        .emit_index = 160,
        .emit_count = 1,
    },
    [138] = {
        .var_count = 1,
        .match_index = 138,
        .match_count = 1,
        .emit_index = 161,
        .emit_count = 2,
    },
    [139] = {
        .var_count = 1,
        .match_index = 139,
        .match_count = 1,
        .emit_index = 163,
        .emit_count = 2,
    },
    [140] = {
        .var_count = 2,
        .match_index = 140,
        .match_count = 1,
        .emit_index = 165,
        .emit_count = 1,
    },
    [141] = {
        .var_count = 2,
        .match_index = 141,
        .match_count = 1,
        .emit_index = 166,
        .emit_count = 1,
    },
    [142] = {
        .var_count = 2,
        .match_index = 142,
        .match_count = 1,
        .emit_index = 167,
        .emit_count = 3,
    },
    {0},
};
