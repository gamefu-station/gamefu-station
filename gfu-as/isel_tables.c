#define ISEL_MATCH_COUNT 143
static isel_match isel_matches[144] = {
    {
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
    {
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
    {
        .mnemonic = GFUAS_MNEM_NOP,
    },
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
        .mnemonic = GFUAS_MNEM_BREAK,
    },
    {
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
    {
        .mnemonic = GFUAS_MNEM_SDBBP,
    },
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
        .mnemonic = GFUAS_MNEM_RET,
    },
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
        .mnemonic = GFUAS_MNEM_SYSCALL,
    },
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
        .kind = ISEL_EMIT_BYTES,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    {
        .kind = ISEL_EMIT_BYTES,
        .argument_count = 1,
        .arguments = {
            {
                .kind = ISEL_ARG_VAR,
                .as.var_index = 0,
            },
        },
    },
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
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
    {
        .var_count = 1,
        .match_index = 0,
        .match_count = 1,
        .emit_index = 0,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 1,
        .match_count = 1,
        .emit_index = 1,
        .emit_count = 1,
    },
    {
        .var_count = 0,
        .match_index = 2,
        .match_count = 1,
        .emit_index = 2,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 3,
        .match_count = 1,
        .emit_index = 3,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 4,
        .match_count = 1,
        .emit_index = 4,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 5,
        .match_count = 1,
        .emit_index = 5,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 6,
        .match_count = 1,
        .emit_index = 6,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 7,
        .match_count = 1,
        .emit_index = 7,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 8,
        .match_count = 1,
        .emit_index = 8,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 9,
        .match_count = 1,
        .emit_index = 9,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 10,
        .match_count = 1,
        .emit_index = 10,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 11,
        .match_count = 1,
        .emit_index = 11,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 12,
        .match_count = 1,
        .emit_index = 12,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 13,
        .match_count = 1,
        .emit_index = 13,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 14,
        .match_count = 1,
        .emit_index = 14,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 15,
        .match_count = 1,
        .emit_index = 15,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 16,
        .match_count = 1,
        .emit_index = 16,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 17,
        .match_count = 1,
        .emit_index = 17,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 18,
        .match_count = 1,
        .emit_index = 18,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 19,
        .match_count = 1,
        .emit_index = 19,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 20,
        .match_count = 1,
        .emit_index = 20,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 21,
        .match_count = 1,
        .emit_index = 21,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 22,
        .match_count = 1,
        .emit_index = 22,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 23,
        .match_count = 1,
        .emit_index = 23,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 24,
        .match_count = 1,
        .emit_index = 24,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 25,
        .match_count = 1,
        .emit_index = 25,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 26,
        .match_count = 1,
        .emit_index = 26,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 27,
        .match_count = 1,
        .emit_index = 27,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 28,
        .match_count = 1,
        .emit_index = 28,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 29,
        .match_count = 1,
        .emit_index = 29,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 30,
        .match_count = 1,
        .emit_index = 30,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 31,
        .match_count = 1,
        .emit_index = 31,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 32,
        .match_count = 1,
        .emit_index = 32,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 33,
        .match_count = 1,
        .emit_index = 33,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 34,
        .match_count = 1,
        .emit_index = 34,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 35,
        .match_count = 1,
        .emit_index = 35,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 36,
        .match_count = 1,
        .emit_index = 36,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 37,
        .match_count = 1,
        .emit_index = 37,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 38,
        .match_count = 1,
        .emit_index = 38,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 39,
        .match_count = 1,
        .emit_index = 39,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 40,
        .match_count = 1,
        .emit_index = 40,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 41,
        .match_count = 1,
        .emit_index = 41,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 42,
        .match_count = 1,
        .emit_index = 42,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 43,
        .match_count = 1,
        .emit_index = 43,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 44,
        .match_count = 1,
        .emit_index = 44,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 45,
        .match_count = 1,
        .emit_index = 45,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 46,
        .match_count = 1,
        .emit_index = 46,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 47,
        .match_count = 1,
        .emit_index = 47,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 48,
        .match_count = 1,
        .emit_index = 48,
        .emit_count = 3,
    },
    {
        .var_count = 2,
        .match_index = 49,
        .match_count = 1,
        .emit_index = 51,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 50,
        .match_count = 1,
        .emit_index = 52,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 51,
        .match_count = 1,
        .emit_index = 53,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 52,
        .match_count = 1,
        .emit_index = 54,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 53,
        .match_count = 1,
        .emit_index = 55,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 54,
        .match_count = 1,
        .emit_index = 56,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 55,
        .match_count = 1,
        .emit_index = 57,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 56,
        .match_count = 1,
        .emit_index = 58,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 57,
        .match_count = 1,
        .emit_index = 59,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 58,
        .match_count = 1,
        .emit_index = 60,
        .emit_count = 3,
    },
    {
        .var_count = 2,
        .match_index = 59,
        .match_count = 1,
        .emit_index = 63,
        .emit_count = 3,
    },
    {
        .var_count = 2,
        .match_index = 60,
        .match_count = 1,
        .emit_index = 66,
        .emit_count = 2,
    },
    {
        .var_count = 3,
        .match_index = 61,
        .match_count = 1,
        .emit_index = 68,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 62,
        .match_count = 1,
        .emit_index = 69,
        .emit_count = 2,
    },
    {
        .var_count = 2,
        .match_index = 63,
        .match_count = 1,
        .emit_index = 71,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 64,
        .match_count = 1,
        .emit_index = 72,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 65,
        .match_count = 1,
        .emit_index = 73,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 66,
        .match_count = 1,
        .emit_index = 74,
        .emit_count = 3,
    },
    {
        .var_count = 3,
        .match_index = 67,
        .match_count = 1,
        .emit_index = 77,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 68,
        .match_count = 1,
        .emit_index = 78,
        .emit_count = 3,
    },
    {
        .var_count = 2,
        .match_index = 69,
        .match_count = 1,
        .emit_index = 81,
        .emit_count = 3,
    },
    {
        .var_count = 2,
        .match_index = 70,
        .match_count = 1,
        .emit_index = 84,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 71,
        .match_count = 1,
        .emit_index = 85,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 72,
        .match_count = 1,
        .emit_index = 86,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 73,
        .match_count = 1,
        .emit_index = 87,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 74,
        .match_count = 1,
        .emit_index = 88,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 75,
        .match_count = 1,
        .emit_index = 89,
        .emit_count = 1,
    },
    {
        .var_count = 0,
        .match_index = 76,
        .match_count = 1,
        .emit_index = 90,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 77,
        .match_count = 1,
        .emit_index = 91,
        .emit_count = 1,
    },
    {
        .var_count = 0,
        .match_index = 78,
        .match_count = 1,
        .emit_index = 92,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 79,
        .match_count = 1,
        .emit_index = 93,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 80,
        .match_count = 1,
        .emit_index = 94,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 81,
        .match_count = 1,
        .emit_index = 95,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 82,
        .match_count = 1,
        .emit_index = 96,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 83,
        .match_count = 1,
        .emit_index = 97,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 84,
        .match_count = 1,
        .emit_index = 98,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 85,
        .match_count = 1,
        .emit_index = 99,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 86,
        .match_count = 1,
        .emit_index = 100,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 87,
        .match_count = 1,
        .emit_index = 101,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 88,
        .match_count = 1,
        .emit_index = 102,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 89,
        .match_count = 1,
        .emit_index = 103,
        .emit_count = 1,
    },
    {
        .var_count = 0,
        .match_index = 90,
        .match_count = 1,
        .emit_index = 104,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 91,
        .match_count = 1,
        .emit_index = 105,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 92,
        .match_count = 1,
        .emit_index = 106,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 93,
        .match_count = 1,
        .emit_index = 107,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 94,
        .match_count = 1,
        .emit_index = 108,
        .emit_count = 3,
    },
    {
        .var_count = 2,
        .match_index = 95,
        .match_count = 1,
        .emit_index = 111,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 96,
        .match_count = 1,
        .emit_index = 112,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 97,
        .match_count = 1,
        .emit_index = 113,
        .emit_count = 3,
    },
    {
        .var_count = 0,
        .match_index = 98,
        .match_count = 1,
        .emit_index = 116,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 99,
        .match_count = 1,
        .emit_index = 117,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 100,
        .match_count = 1,
        .emit_index = 118,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 101,
        .match_count = 1,
        .emit_index = 119,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 102,
        .match_count = 1,
        .emit_index = 120,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 103,
        .match_count = 1,
        .emit_index = 121,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 104,
        .match_count = 1,
        .emit_index = 122,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 105,
        .match_count = 1,
        .emit_index = 123,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 106,
        .match_count = 1,
        .emit_index = 124,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 107,
        .match_count = 1,
        .emit_index = 125,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 108,
        .match_count = 1,
        .emit_index = 126,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 109,
        .match_count = 1,
        .emit_index = 127,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 110,
        .match_count = 1,
        .emit_index = 128,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 111,
        .match_count = 1,
        .emit_index = 129,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 112,
        .match_count = 1,
        .emit_index = 130,
        .emit_count = 2,
    },
    {
        .var_count = 3,
        .match_index = 113,
        .match_count = 1,
        .emit_index = 132,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 114,
        .match_count = 1,
        .emit_index = 133,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 115,
        .match_count = 1,
        .emit_index = 134,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 116,
        .match_count = 1,
        .emit_index = 135,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 117,
        .match_count = 1,
        .emit_index = 136,
        .emit_count = 3,
    },
    {
        .var_count = 3,
        .match_index = 118,
        .match_count = 1,
        .emit_index = 139,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 119,
        .match_count = 1,
        .emit_index = 140,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 120,
        .match_count = 1,
        .emit_index = 141,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 121,
        .match_count = 1,
        .emit_index = 142,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 122,
        .match_count = 1,
        .emit_index = 143,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 123,
        .match_count = 1,
        .emit_index = 144,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 124,
        .match_count = 1,
        .emit_index = 145,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 125,
        .match_count = 1,
        .emit_index = 146,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 126,
        .match_count = 1,
        .emit_index = 147,
        .emit_count = 1,
    },
    {
        .var_count = 3,
        .match_index = 127,
        .match_count = 1,
        .emit_index = 148,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 128,
        .match_count = 1,
        .emit_index = 149,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 129,
        .match_count = 1,
        .emit_index = 150,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 130,
        .match_count = 1,
        .emit_index = 151,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 131,
        .match_count = 1,
        .emit_index = 152,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 132,
        .match_count = 1,
        .emit_index = 153,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 133,
        .match_count = 1,
        .emit_index = 154,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 134,
        .match_count = 1,
        .emit_index = 155,
        .emit_count = 2,
    },
    {
        .var_count = 2,
        .match_index = 135,
        .match_count = 1,
        .emit_index = 157,
        .emit_count = 2,
    },
    {
        .var_count = 1,
        .match_index = 136,
        .match_count = 1,
        .emit_index = 159,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 137,
        .match_count = 1,
        .emit_index = 160,
        .emit_count = 1,
    },
    {
        .var_count = 1,
        .match_index = 138,
        .match_count = 1,
        .emit_index = 161,
        .emit_count = 2,
    },
    {
        .var_count = 1,
        .match_index = 139,
        .match_count = 1,
        .emit_index = 163,
        .emit_count = 2,
    },
    {
        .var_count = 2,
        .match_index = 140,
        .match_count = 1,
        .emit_index = 165,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 141,
        .match_count = 1,
        .emit_index = 166,
        .emit_count = 1,
    },
    {
        .var_count = 2,
        .match_index = 142,
        .match_count = 1,
        .emit_index = 167,
        .emit_count = 3,
    },
    {0},
};
