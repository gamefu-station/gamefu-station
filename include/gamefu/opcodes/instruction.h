/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#ifndef GAMEFU_OPCODES_INSTRUCTION_H_
#define GAMEFU_OPCODES_INSTRUCTION_H_

#include <gamefu/common.h>

#include "register.h"

#define GFU_OP(X) \
    X(SPECIAL, 0x00) \
    X(J, 0x02) \
    X(JAL, 0x03) \
    X(BEQ, 0x04) \
    X(BNE, 0x05) \
    X(BLEZ, 0x06) \
    X(BGTZ, 0x07) \
    X(ADDI, 0x08) \
    X(ADDIU, 0x09) \
    X(SLTI, 0x0A) \
    X(SLTIU, 0x0B) \
    X(ANDI, 0x0C) \
    X(ORI, 0x0D) \
    X(XORI, 0x0E) \
    X(LUI, 0x0F) \
    X(COP0, 0x10) \
    X(TGEI, 0x18) \
    X(TGEIU, 0x19) \
    X(TLTI, 0x1A) \
    X(TLTIU, 0x1B) \
    X(TEQI, 0x1C) \
    X(TNEI, 0x1D) \
    X(LB, 0x20) \
    X(LH, 0x21) \
    X(LWL, 0x22) \
    X(LW, 0x23) \
    X(LBU, 0x24) \
    X(LHU, 0x25) \
    X(LWR, 0x26) \
    X(SB, 0x28) \
    X(SH, 0x29) \
    X(SWL, 0x2A) \
    X(SW, 0x2B) \
    X(SWR, 0x2E) \
    X(BLTZ, 0x30) \
    X(BGEZ, 0x31) \
    X(BLTZAL, 0x32) \
    X(BGEZAL, 0x33) \
    X(BREAK, 0x3E) \
    X(SDBBP, 0x3F)

#define GFU_OPFN(X) \
    X(SLL, 0x00) \
    X(SRL, 0x02) \
    X(SRA, 0x03) \
    X(SLLV, 0x04) \
    X(SRLV, 0x06) \
    X(SRAV, 0x07) \
    X(JR, 0x08) \
    X(JALR, 0x09) \
    X(MOVZ, 0x0A) \
    X(MOVN, 0x0B) \
    X(SYSCALL, 0x0C) \
    X(MFHI, 0x10) \
    X(MTHI, 0x11) \
    X(MFLO, 0x12) \
    X(MTLO, 0x13) \
    X(MADD, 0x14) \
    X(MADDU, 0x15) \
    X(MSUB, 0x16) \
    X(MSUBU, 0x17) \
    X(MULT, 0x18) \
    X(MULTU, 0x19) \
    X(DIV, 0x1A) \
    X(DIVU, 0x1B) \
    X(MUL, 0x1C) \
    X(ADD, 0x20) \
    X(ADDU, 0x21) \
    X(SUB, 0x22) \
    X(SUBU, 0x23) \
    X(AND, 0x24) \
    X(OR, 0x25) \
    X(XOR, 0x26) \
    X(NOR, 0x27) \
    X(CLZ, 0x28) \
    X(CLO, 0x29) \
    X(SLT, 0x2A) \
    X(SLTU, 0x2B) \
    X(TGE, 0x30) \
    X(TGEU, 0x31) \
    X(TLT, 0x32) \
    X(TLTU, 0x33) \
    X(TEQ, 0x34) \
    X(TNE, 0x36)

#define GFU_C0FN(X) \
    X(MF, 0x00) \
    X(MT, 0x04)

typedef enum gfu_opcode {
#define X(Id, Value) GFU_OP_##Id = (Value),
    GFU_OP(X)
#undef X
} gfu_opcode;

typedef enum gfu_opfn {
#define X(Id, Value) GFU_OPFN_##Id = (Value),
    GFU_OPFN(X)
#undef X
} gfu_opfn;

typedef enum gfu_opc0fn {
#define X(Id, Value) GFU_C0FN_##Id = (Value),
    GFU_C0FN(X)
#undef X
} gfu_c0fn;

typedef union gfu_inst {
    gfu_uword raw;
    struct {
        gfu_uword function : 6;
        gfu_uword shift : 5;
        gfu_uword r : 5;
        gfu_uword l : 5;
        gfu_uword d : 5;
        gfu_uword opcode : 6;
    } reg;
    struct {
        gfu_uword value : 16;
        gfu_uword l : 5;
        gfu_uword d : 5;
        gfu_uword opcode : 6;
    } imm;
    struct {
        gfu_uword value : 26;
        gfu_uword opcode : 6;
    } addr;
} gfu_inst;

static_assert(sizeof(gfu_inst) == sizeof(gfu_uword), "Ensure that the union of bitfields does not change the size of the instruction type.");

#endif /* GAMEFU_OPCODES_INSTRUCTION_H_ */
