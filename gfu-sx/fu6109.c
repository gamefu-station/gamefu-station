#include <gamefu/common.h>

#include "../gfu-opcodes/instruction.h"
#include "../gfu-opcodes/register.h"
#include "../gfu-opcodes/syscall.h"

#include "prologue.h"
#include "fu6109.h"
#include "hardware.h"
#include "memory.h"
#include "vm.h"

#define _rnZero_ GFU_GPR_ZERO
#define _rN_(N)  vm->cpu.regs.gpr.values[N]
#define _rZero_  vm->cpu.regs.gpr.named.zero
#define _rR0_    vm->cpu.regs.gpr.named.r0
#define _rR1_    vm->cpu.regs.gpr.named.r1
#define _rA0_    vm->cpu.regs.gpr.named.a0
#define _rA1_    vm->cpu.regs.gpr.named.a1
#define _rA2_    vm->cpu.regs.gpr.named.a2
#define _rA3_    vm->cpu.regs.gpr.named.a3
#define _rGP_    vm->cpu.regs.gpr.named.gp
#define _rSP_    vm->cpu.regs.gpr.named.sp
#define _rFP_    vm->cpu.regs.gpr.named.fp
#define _rRA_    vm->cpu.regs.gpr.named.ra
#define _rSP_    vm->cpu.regs.gpr.named.sp
#define _rHI_    vm->cpu.regs.gpr.named.hi
#define _rLO_    vm->cpu.regs.gpr.named.lo
#define _rPC_    vm->cpu.regs.pc
#define _rD_     vm->cpu.regs.gpr.values[inst.reg.d]
#define _rL_     vm->cpu.regs.gpr.values[inst.reg.l]
#define _rR_     vm->cpu.regs.gpr.values[inst.reg.r]

#define _c0N_(N)      vm->cpu.regs.cop0.values[N]
#define _c0BadVAddr_  vm->cpu.regs.cop0.named.bad_vaddr
#define _c0Status_    vm->cpu.regs.cop0.named.status
#define _c0Cause_     vm->cpu.regs.cop0.named.cause
#define _c0EPC_       vm->cpu.regs.cop0.named.epc

#define _iD_     inst.reg.d
#define _iL_     inst.reg.l
#define _iR_     inst.reg.r
#define _iOp_    inst.addr.opcode
#define _iFn_    inst.reg.function
#define _iSh_    inst.reg.shift
#define _iImm_   inst.imm.value
#define _iAddr_  inst.addr.value

static const char* regname[] = {
#define GPR(Id, Name) [GFU_GPR_##Id] = "" Name,
#include "../gfu-opcodes/x/registers.h"
    nullptr,
};

#ifdef GFU_NTRACE
#  define gfusx_trace_disasm(Vm, Inst) do { } while (0)
#else
static const char* opnames[] = {
#define X(Id, Value) [GFU_OP_##Id] = #Id,
    GFU_OP(X)
#undef X
};

static const char* fnnames[] = {
#define X(Id, Value) [GFU_OPFN_##Id] = #Id,
    GFU_OPFN(X)
#undef X
};

static const char* c0fnnames[] = {
#define X(Id, Value) [GFU_C0FN_##Id] = #Id,
    GFU_C0FN(X)
#undef X
};

static const char* intnames[] = {
#define X(Id, Value) [GFUSX_INT_##Id] = #Id,
    GFUSX_INT(X)
#undef X
};

static void gfusx_trace_disasm(gfusx_t* vm, gfu_uword addr, gfu_inst inst) {
    if (!vm->trace) return;

    fprintf(stderr, "%08X:   op=%02X (%s), ", addr, inst.addr.opcode, opnames[inst.addr.opcode]);
    if (inst.addr.opcode == GFU_OP_SPECIAL) {
        fprintf(
            stderr,
            "d=%02X (%s), l=%02X (%s), r=%02X (%s), sh=%d, fn=%02X (%s)",
            inst.reg.d, regname[inst.reg.d],
            inst.reg.l, regname[inst.reg.l],
            inst.reg.r, regname[inst.reg.r],
            inst.reg.shift,
            inst.reg.function, fnnames[inst.reg.function]
        );
    } else if (inst.addr.opcode == GFU_OP_COP0) {
        fprintf(
            stderr,
            "d=%02X (%s), l=%02X (%s), r=%02X (%s), sh=%d, fn=%02X (%s)",
            inst.reg.d, regname[inst.reg.d],
            inst.reg.l, regname[inst.reg.l],
            inst.reg.r, regname[inst.reg.r],
            inst.reg.shift,
            inst.reg.function, c0fnnames[inst.reg.function]
        );
    } else if (inst.addr.opcode == GFU_OP_J || inst.addr.opcode == GFU_OP_JAL) {
        fprintf(stderr, "addr=%08X", inst.addr.value << 2);
    } else {
        fprintf(
            stderr,
            "d=%02X (%s), l=%02X (%s), imm=%04X",
            inst.imm.d, regname[inst.imm.d],
            inst.imm.l, regname[inst.imm.l],
            inst.imm.value
        );
    }

    fputc('\n', stderr);
}
#endif /* GFU_NTRACE */

static void gfusx_syscall(gfusx_t* vm, gfu_word s) {
    switch (s) {
        default: {
            gfusx_logf(vm, "Unknown syscall %02X, currently doing nothing and resuming.", s);
        } break;

        // void gfu_system_halt()
        case GFU_SYS_RESERVED00: {
            // it's marked as reserved, but for now we're implementing it as an exit/halt instruction while the inner workings of the emulated hardware are fleshed out.
            vm->halt = true;
        } break;

        // [word, word] file_write_string(word handle, byte* string)
        case GFU_SYS_FILE_WRITE_STRING: {
            gfu_uword fd = _rA0_;
            gfu_uword string_addr = _rA1_;

            const char* string = (const char*)gfusx_memory_get_rptr(vm, string_addr);
            if (string == nullptr) {
                _rR0_ = 0;
                _rR1_ = 1; // Status code 1, an error which needs to be fleshed out
                break;
            }

            FILE* output_stream = nullptr;
            switch (fd) {
                default: break;
                case 0: output_stream = stdin; break;
                case 1: output_stream = stdout; break;
                case 2: output_stream = stderr; break;
            }

            if (output_stream == nullptr) {
                _rR0_ = 0;
                _rR1_ = 2; // Status code 2, an error which needs to be fleshed out
                break;
            }

            errno = 0;
            int r = fprintf(output_stream, "%s", string);

            if (ferror(output_stream)) {
                _rR0_ = 0;
                _rR1_ = 3; // Status code 3, an error which needs to be fleshed out
                break;
            }

            _rR0_ = (gfu_uword)(gfu_word)r;
            _rR1_ = 0; // Status code 0, no error
        } break;

        // [word, word] file_write_format(word handle, byte* format, ...)
        case GFU_SYS_FILE_WRITE_FORMAT: {
            gfu_uword fd = _rA0_;
            gfu_uword format_addr = _rA1_;

            gfu_uword varargs_count = _rR1_;
            gfu_uword varargs_top = _rSP_;

            const char* format = (const char*)gfusx_memory_get_rptr(vm, format_addr);
            if (format == nullptr) {
                _rR0_ = 0;
                _rR1_ = 1; // Status code 1, an error which needs to be fleshed out
                break;
            }

            FILE* output_stream = nullptr;
            switch (fd) {
                default: break;
                case 0: output_stream = stdin; break;
                case 1: output_stream = stdout; break;
                case 2: output_stream = stderr; break;
            }

            if (output_stream == nullptr) {
                _rR0_ = 0;
                _rR1_ = 2; // Status code 2, an error which needs to be fleshed out
                break;
            }

            gfu_uword count = 0;
            gfu_uword vararg_index = 0;

            for (const char* f = format; *f != 0; ) {
                errno = 0;

                gfu_uword current_vararg_addr = varargs_top +
                    (sizeof(gfu_uword) * vararg_index);

                char c = *(f++);
                switch (c) {
                    default: {
                        fputc(c, output_stream);
                        if (ferror(output_stream)) {
                            _rR1_ = 3; // Status code 3, an error which needs to be fleshed out
                            goto just_break_please_there_was_an_error;
                        }

                        count += 1;
                    } break;

                    case '%': {
                        if (*f == 0) {
                            _rR1_ = 4; // Status code 4, an error which needs to be fleshed out
                            goto just_break_please_there_was_an_error;
                        }

                        char fspec = *(f++);
                        switch (fspec) {
                            default: {
                                _rR1_ = 5; // Status code 5, an error which needs to be fleshed out
                                goto just_break_please_there_was_an_error;
                            }

                            case 'w': {
                                gfu_word i = (gfu_word)gfusx_memory_read_word(vm, current_vararg_addr);
                                int r = fprintf(output_stream, "%d", i);
                                count += (gfu_uword)(gfu_word)r;
                            } break;

                            case 'u': {
                                gfu_uword i = gfusx_memory_read_word(vm, current_vararg_addr);
                                int r = fprintf(output_stream, "%u", i);
                                count += (gfu_uword)(gfu_word)r;
                            } break;

                            case 's': {
                                gfu_uword s_addr = gfusx_memory_read_word(vm, current_vararg_addr);
                                const char* s = (const char*)gfusx_memory_get_rptr(vm, s_addr);
                                int r = fprintf(output_stream, "%s", s);
                                count += (gfu_uword)(gfu_word)r;
                            } break;
                        }

                        vararg_index += 1;
                    } break;
                }

                if (ferror(output_stream)) {
                    _rR1_ = 3; // Status code 3, an error which needs to be fleshed out
                    goto just_break_please_there_was_an_error;
                }
            }

            _rR0_ = count;
            _rR1_ = 0; // Status code 0, no error
            break;

        just_break_please_there_was_an_error:;
            _rR0_ = 0;
        } break;
    }
}

static void gfusx_exception(gfusx_t* vm, gfusx_exc_t cause) {
    _c0EPC_ = _rPC_;
    if ((_c0Status_ & 0x400000) != 0) {
        _rPC_ = GFU_KSEG1_BASE + GFU_BIOS_ROM_BASE + 0x0180;
    } else {
        _rPC_ = GFU_KSEG0_BASE + 0x80;
    }

    gfusx_logf(vm, "Exception jumping to PC %08X.", _rPC_);

    _c0Cause_ = (gfu_uword)cause;
    _c0Status_ = (_c0Status_ & ~0x3F) | ((_c0Status_ & 0x0F) << 2);
}

static void gfusx_branch_test(gfusx_t* vm) {
    const gfu_ulong cycle = vm->cpu.cycle;

    const gfu_uword interrupts = vm->cpu.interrupt;
    gfu_long lowest_dist = 0x7FFFFFFFFFFFFFFF;
    gfu_long lowest_target = (gfu_long)cycle;
    const gfu_ulong* targets = &vm->cpu.interrupt_targets[0];

    if (interrupts != 0 && vm->cpu.lowest_target < cycle) {
#define CHECK_AND_UPDATE(Irq, Act) \
        do { \
            const gfu_uword mask = 1 << (Irq); \
            if ((interrupts & mask) != 0) { \
                gfu_ulong target = targets[Irq]; \
                gfu_long dist = (gfu_long)target - (gfu_long)cycle; \
                if (dist > 0) { \
                    if (dist < lowest_dist) { \
                        lowest_dist = dist; \
                        lowest_target = target; \
                    } \
                } else { \
                    vm->cpu.interrupt &= ~mask; \
                    gfusx_logf(vm, "Triggering interrupt %s, using action '%s'.", intnames[Irq], #Act); \
                    Act(vm); \
                } \
            } \
        } while (0)

        // CHECK_AND_UPDATE(GFUSX_INT_SIO, gfusx_sio_interrupt);
        // CHECK_AND_UPDATE(GFUSX_INT_SIO1, gfusx_sio1_interrupt);
        CHECK_AND_UPDATE(GFUSX_INT_CHIP, gfusx_chip_interrupt);
        CHECK_AND_UPDATE(GFUSX_INT_CHIP_READ, gfusx_chip_read_interrupt);
        CHECK_AND_UPDATE(GFUSX_INT_CHIP_DMA, gfusx_chip_dma_interrupt);
        // CHECK_AND_UPDATE(GFUSX_INT_GPU_DMA, gfusx_gpu_dma_interrupt);

#undef CHECK_AND_UPDATE

        vm->cpu.lowest_target = lowest_target;
    }

    const gfu_uword istat = gfusx_memory_read_hwreg_word(vm, GFU_ISTAT_ADDR & 0xFFFF);
    const gfu_uword imask = gfusx_memory_read_hwreg_word(vm, GFU_IMASK_ADDR & 0xFFFF);

    //gfusx_logf(vm, "ISTAT %08X, IMASK %08X, COP0 STATUS %08X", istat, imask, _c0Status_);

    // 0x0401 is bits 10 and 0 of cop0r12 (status).
    // bit 0 is "current interrupt enable".
    // bit 10 is one of the interrupts that could be triggered.
    // NOTE(echoe): we might want to have this check more than just bit 10?
    if ((istat & imask) != 0 && (_c0Status_ & 0x0401) == 0x0401) {
        gfusx_logf(vm, "Interrupt: %08X, %08X.", istat, imask);
        gfusx_exception(vm, 0x0400);
    }
}

static void gfusx_test_sw_interrupts(gfusx_t* vm) {
    // 0x0300 is cop0r13 (cause) bits 8-9, SW interrupts.
    if (_c0Status_ & _c0Cause_ & 0x0300 && _c0Status_ & 0x01) {
        gfusx_exception(vm, _c0Cause_);
    }
}

static bool gfusx_step(gfusx_t* vm, gfu_uword pc, gfu_uword code) {
    gfu_inst inst = { .raw = code };
    gfusx_trace_disasm(vm, pc, inst);

    switch (_iOp_) {
        default: {
            assertf(false, "Unimplemented opcode %02X.", (unsigned char)(_iOp_));
        } break;

        case GFU_OP_J: { // 0x02  j addr $t
            _rPC_ = (_rPC_ & 0xF0000000) | (_iAddr_ << 2);
            return true;
        }

        case GFU_OP_JAL: { // 0x03  jal addr $t
            _rRA_ = _rPC_;
            _rPC_ = (_rPC_ & 0xF0000000) | (_iAddr_ << 2);
            return true;
        }

        case GFU_OP_BEQ: { // 0x04  beq reg $l, reg $r, addr $t
            if (_rD_ == _rL_) {
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            } else return false;
        }

        case GFU_OP_BNE: { // 0x05  bne reg $l, reg $r, addr $t
            if (_rD_ != _rL_) {
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            } else return false;
        }

        case GFU_OP_BLEZ: { // 0x06  blez reg $d, addr $t
            if (_rD_ <= 0) {
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            } else return false;
        }

        case GFU_OP_BGTZ: { // 0x07  bgtz reg $d, addr $t
            if (_rD_ > 0) {
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            } else return false;
        }

        case GFU_OP_ADDI: { // 0x08  addi reg $d, reg $l, imm $v
            gfu_long v = ((gfu_long)_rL_ + sext(half, long, _iImm_));
            // TODO(echoe): overflow exceptions
            if (_iD_ != _rnZero_) _rD_ = (gfu_uword)v;
        } break;

        case GFU_OP_ADDIU: { // 0x09  addiu reg $d, reg $l, imm $v
            gfu_long v = ((gfu_long)_rL_ + sext(half, long, _iImm_));
            if (_iD_ != _rnZero_) _rD_ = (gfu_uword)v;
        } break;

        case GFU_OP_SLTI: { // 0x0A  slti reg $d, reg $l, imm $v
            if (_iD_ != _rnZero_) _rD_ = (gfu_uword)((gfu_word)_rL_ < (gfu_half)_iImm_);
        } break;

        case GFU_OP_SLTIU: { // 0x0B  sltiu reg $d, reg $l, imm $v
            if (_iD_ != _rnZero_) _rD_ = (gfu_uword)(_rL_ < _iImm_);
        } break;

        case GFU_OP_ANDI: { // 0x0C  andi reg $d, reg $l, imm $v
            if (_iD_ != _rnZero_) _rD_ = _rL_ & _iImm_;
        } break;

        case GFU_OP_ORI: { // 0x0D  ori reg $d, reg $l, imm $v
            if (_iD_ != _rnZero_) _rD_ = _rL_ | _iImm_;
        } break;

        case GFU_OP_XORI: { // 0x0E  xori reg $d, reg $l, imm $v
            if (_iD_ != _rnZero_) _rD_ = _rL_ | _iImm_;
        } break;

        case GFU_OP_LUI: { // 0x0F  lui reg $d, imm $v
            if (_iD_ != _rnZero_) _rD_ = _iImm_ << 16;
        } break;

        // TODO(echoe): Trap instructions

        case GFU_OP_LB: { // 0x20  lb reg $d, imm $v(reg $l)
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rL_);
            gfu_ubyte v = gfusx_memory_read_byte(vm, addr);
            if (_iD_ != _rnZero_) _rD_ = sext(byte, word, v);
        } break;

        case GFU_OP_LH: { // 0x21  lh reg $d, imm $v(reg $l)
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rL_);
            gfu_uhalf v = gfusx_memory_read_half(vm, addr);
            if (_iD_ != _rnZero_) _rD_ = sext(half, word, v);
        } break;

        case GFU_OP_LWL: { // 0x22  lwl reg $d, imm $v(reg $l)
            static gfu_uword lwl_mask[4] = { 0x00FFFFFFu, 0x0000FFFFu, 0x000000FFu, 0 };
            static gfu_uword lwl_shift[4] = { 24, 16, 8, 0 };
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rL_);
            gfu_uword shift = addr & 0x03, v = gfusx_memory_read_word(vm, addr & ~0x03);
            if (_iD_ != _rnZero_) _rD_ = ((_rD_ & lwl_mask[shift]) | (v << lwl_shift[shift]));
        } break;

        case GFU_OP_LW: { // 0x23  lw reg $d, imm $v(reg $l)
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rL_);
            gfu_uword v = gfusx_memory_read_word(vm, addr);
            if (_iD_ != _rnZero_) _rD_ = v;
        } break;

        case GFU_OP_LBU: { // 0x24  lbu reg $d, imm $v(reg $l)
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rL_);
            gfu_ubyte v = gfusx_memory_read_byte(vm, addr);
            if (_iD_ != _rnZero_) _rD_ = v;
        } break;

        case GFU_OP_LHU: { // 0x25  lhu reg $d, imm $v(reg $l)
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rL_);
            gfu_uhalf v = gfusx_memory_read_half(vm, addr);
            if (_iD_ != _rnZero_) _rD_ = v;
        } break;

        case GFU_OP_LWR: { // 0x26  lwr reg $d, imm $v(reg $l)
            static gfu_uword lwr_mask[4] = { 0, 0xFF000000u, 0xFFFF0000u, 0xFFFFFF00u };
            static gfu_uword lwr_shift[4] = { 0, 8, 16, 24 };
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rL_);
            gfu_uword shift = addr & 0x03, v = gfusx_memory_read_word(vm, addr & ~0x03);
            if (_iD_ != _rnZero_) _rD_ = ((_rD_ & lwr_mask[shift]) | (v >> lwr_shift[shift]));
        } break;

        case GFU_OP_SB: { // 0x28  sb imm $v(reg $d), reg $l
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rD_);
            gfusx_memory_write_byte(vm, addr, (gfu_ubyte)_rL_);
        } break;

        case GFU_OP_SH: { // 0x29  sh imm $v(reg $d), reg $l
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rD_);
            gfusx_memory_write_half(vm, addr, (gfu_uhalf)_rL_);
        } break;

        case GFU_OP_SWL: { // 0x2A  swl imm $v(reg $d), reg $l
            static gfu_uword swl_mask[4] = { 0xFFFFFF00u, 0xFFFF0000u, 0xFF000000u, 0 };
            static gfu_uword swl_shift[4] = { 24, 16, 8, 0 };
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rD_);
            gfu_uword shift = addr & 0x03, v = gfusx_memory_read_word(vm, addr & ~0x03);
            gfusx_memory_write_word(vm, addr, (_rL_ >> swl_shift[shift]) | (v & swl_mask[shift]));
        } break;

        case GFU_OP_SW: { // 0x2B  sw imm $v(reg $d), reg $l
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rD_);
            gfusx_memory_write_word(vm, addr, (gfu_uword)_rL_);
        } break;

        case GFU_OP_SWR: { // 0x2E  swr imm $v(reg $d), reg $l
            static gfu_uword swr_mask[4] = { 0, 0x000000FFu, 0x0000FFFFu, 0x00FFFFFFu };
            static gfu_uword swr_shift[4] = { 0, 8, 16, 24 };
            gfu_uword addr = (gfu_uword)((gfu_half)_iImm_ + (gfu_long)_rD_);
            gfu_uword shift = addr & 0x03, v = gfusx_memory_read_word(vm, addr & ~0x03);
            gfusx_memory_write_word(vm, addr, (_rL_ << swr_shift[shift]) | (v & swr_mask[shift]));
        } break;

        case GFU_OP_BLTZ: { // 0x30  bltz reg $d, imm $v
            if ((gfu_word)_rD_ < 0) {
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            }
            return false;
        }

        case GFU_OP_BGEZ: { // 0x31  bgez reg $d, imm $v
            if ((gfu_word)_rD_ >= 0) {
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            }
            return false;
        }

        case GFU_OP_BLTZAL: { // 0x32  bltzal reg $d, imm $v
            if ((gfu_word)_rD_ < 0) {
                _rRA_ = _rPC_;
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            }
            return false;
        }

        case GFU_OP_BGEZAL: { // 0x31  bgezal reg $d, imm $v
            if ((gfu_word)_rD_ >= 0) {
                _rRA_ = _rPC_;
                _rPC_ += (gfu_word)((gfu_half)_iImm_) << 2;
                return true;
            }
            return false;
        }

        // TODO(echoe): BREAK and SDBBP

        case GFU_OP_SPECIAL: {
            switch (_iFn_) {
                default: {
                    assertf(false, "Unimplemented register function %02X.", (unsigned char)(_iFn_));
                } break;

                case GFU_OPFN_SLL: { // 0x00  sll reg $d, reg $l, imm $s
                    if (_iD_ != _rnZero_) _rD_ = _rL_ << _iSh_;
                } break;

                case GFU_OPFN_SRL: { // 0x02  srl reg $d, reg $l, imm $s
                    if (_iD_ != _rnZero_) _rD_ = _rL_ >> _iSh_;
                } break;

                case GFU_OPFN_SRA: { // 0x03  sra reg $d, reg $l, imm $s
                    if (_iD_ != _rnZero_) _rD_ = (gfu_uword)(((gfu_word)_rL_) >> _iSh_);
                } break;

                case GFU_OPFN_SLLV: { // 0x04  sllv reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = _rL_ << _rR_;
                } break;

                case GFU_OPFN_SRLV: { // 0x06  srlv reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = _rL_ >> _rR_;
                } break;

                case GFU_OPFN_SRAV: { // 0x07  srav reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = (gfu_uword)(((gfu_word)_rL_) >> _rR_);
                } break;

                case GFU_OPFN_JR: { // 0x08  jr reg $l
                    gfu_uword jaddr = _rL_;
                    if ((jaddr & 3) != 0) {
                        _rPC_ -= sizeof(gfu_uword);
                        gfusx_logf(vm, "Attempt unaligned JR to 0x%08X from 0x%08X.", jaddr, _rPC_);
                        _c0BadVAddr_ = jaddr;
                        gfusx_exception(vm, GFUSX_EXC_BAD_LOAD);
                    } else {
                        _rPC_ = jaddr;
                        return true;
                    }
                } break;

                case GFU_OPFN_JALR: { // 0x09  jalr reg $d, reg $l
                    gfu_uword jaddr = _rL_;
                    if ((jaddr & 3) != 0) {
                        _rPC_ -= sizeof(gfu_uword);
                        gfusx_logf(vm, "Attempt unaligned JALR to 0x%08X from 0x%08X.", jaddr, _rPC_);
                        _c0BadVAddr_ = jaddr;
                        gfusx_exception(vm, GFUSX_EXC_BAD_LOAD);
                    } else {
                        if (_iD_ != _rnZero_) {
                            _rD_ = _rPC_;
                            // TODO(echoe): update call stack info
                        }
                        _rPC_ = jaddr;
                        return true;
                    }
                } break;

                case GFU_OPFN_MOVZ: { // 0x0A  movz reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_ && _rR_ == 0) _rD_ = _rL_;
                } break;

                case GFU_OPFN_MOVN: { // 0x0B  movn reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_ && _rR_ != 0) _rD_ = _rL_;
                } break;

                case GFU_OPFN_SYSCALL: { // 0x0C  syscall
                    gfusx_syscall(vm, _rR0_);
                } break;

                case GFU_OPFN_MFHI: { // 0x10  mfhi reg $d
                    if (_iD_ != _rnZero_) _rD_ = _rHI_;
                } break;

                case GFU_OPFN_MTHI: { // 0x11  mthi reg $l
                    _rHI_ = _rL_;
                } break;

                case GFU_OPFN_MFLO: { // 0x12  mflo reg $d
                    if (_iD_ != _rnZero_) _rD_ = _rLO_;
                } break;

                case GFU_OPFN_MTLO: { // 0x13  mtlo reg $l
                    _rLO_ = _rL_;
                } break;

                case GFU_OPFN_MADD: { // 0x14  madd reg $l, reg $r
                    gfu_long m = ((gfu_word)_rL_) * ((gfu_word)_rR_);
                    gfu_long v = ((((gfu_ulong)_rHI_) << 32) | ((gfu_ulong)_rLO_)) + m;
                    _rHI_ = (gfu_uword)((v >> 32) & 0xFFFFFFFFu);
                    _rLO_ = (gfu_uword)((v      ) & 0xFFFFFFFFu);
                } break;

                case GFU_OPFN_MADDU: { // 0x15  maddu reg $l, reg $r
                    gfu_long m = _rL_ * _rR_;
                    gfu_long v = ((((gfu_ulong)_rHI_) << 32) | ((gfu_ulong)_rLO_)) + m;
                    _rHI_ = (gfu_uword)((v >> 32) & 0xFFFFFFFFu);
                    _rLO_ = (gfu_uword)((v      ) & 0xFFFFFFFFu);
                } break;

                case GFU_OPFN_MSUB: { // 0x16  msub reg $l, reg $r
                    gfu_long m = ((gfu_word)_rL_) * ((gfu_word)_rR_);
                    gfu_long v = ((((gfu_ulong)_rHI_) << 32) | ((gfu_ulong)_rLO_)) - m;
                    _rHI_ = (gfu_uword)((v >> 32) & 0xFFFFFFFFu);
                    _rLO_ = (gfu_uword)((v      ) & 0xFFFFFFFFu);
                } break;

                case GFU_OPFN_MSUBU: { // 0x17  msubu reg $l, reg $r
                    gfu_long m = _rL_ * _rR_;
                    gfu_long v = ((((gfu_ulong)_rHI_) << 32) | ((gfu_ulong)_rLO_)) - m;
                    _rHI_ = (gfu_uword)((v >> 32) & 0xFFFFFFFFu);
                    _rLO_ = (gfu_uword)((v      ) & 0xFFFFFFFFu);
                } break;

                case GFU_OPFN_MULT: { // 0x18  mult reg $l, reg $r
                    gfu_long v = ((gfu_word)_rL_) * ((gfu_word)_rR_);
                    _rHI_ = (gfu_uword)((v >> 32) & 0xFFFFFFFFu);
                    _rLO_ = (gfu_uword)((v      ) & 0xFFFFFFFFu);
                } break;

                case GFU_OPFN_MULTU: { // 0x19  multu reg $l, reg $r
                    gfu_long v = _rL_ * _rR_;
                    _rHI_ = (gfu_uword)((v >> 32) & 0xFFFFFFFFu);
                    _rLO_ = (gfu_uword)((v      ) & 0xFFFFFFFFu);
                } break;

                case GFU_OPFN_DIV: { // 0x1A  div reg $l, reg $r
                    _rHI_ = (gfu_uword)(((gfu_word)_rL_) % ((gfu_word)_rR_));
                    _rLO_ = (gfu_uword)(((gfu_word)_rL_) / ((gfu_word)_rR_));
                } break;

                case GFU_OPFN_DIVU: { // 0x1B  divu reg $l, reg $r
                    _rHI_ = _rL_ % _rR_;
                    _rLO_ = _rL_ / _rR_;
                } break;

                case GFU_OPFN_MUL: { // 0x1C  mul reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = (gfu_uword)(sext(word, long, _rL_) * sext(word, long, _rR_));
                } break;

                case GFU_OPFN_ADD: { // 0x20  add reg $d, reg $l, reg $r
                    gfu_long v = (gfu_word)_rL_ + (gfu_word)_rR_;
                    // TODO(echoe): overflow exceptions
                    if (_iD_ != _rnZero_) _rD_ = (gfu_uword)v;
                } break;

                case GFU_OPFN_ADDU: { // 0x21  addu reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = _rL_ + _rR_;
                } break;

                case GFU_OPFN_SUB: { // 0x22  sub reg $d, reg $l, reg $r
                    gfu_long v = (gfu_word)_rL_ - (gfu_word)_rR_;
                    // TODO(echoe): overflow exceptions
                    if (_iD_ != _rnZero_) _rD_ = (gfu_uword)v;
                } break;

                case GFU_OPFN_SUBU: { // 0x23  subu reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = _rL_ - _rR_;
                } break;

                case GFU_OPFN_AND: { // 0x24  and reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = _rL_ & _rR_;
                } break;

                case GFU_OPFN_OR: { // 0x25  or reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = _rL_ | _rR_;
                } break;

                case GFU_OPFN_XOR: { // 0x26  xor reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = _rL_ ^ _rR_;
                } break;

                case GFU_OPFN_NOR: { // 0x27  nor reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = ~(_rL_ | _rR_);
                } break;

                // TODO(echoe): See if we can get hardware calls through some compilers.
                case GFU_OPFN_CLZ: { // 0x28  clz reg $d, reg $l
                    if (_iD_ != _rnZero_) {
                        gfu_long x = _rL_;
                        gfu_long r = (x > 0xFFFFu) << 4; x >>= r;
                        gfu_long q = (x > 0xFFu  ) << 3; x >>= q; r |= q;
                                 q = (x > 0xFu   ) << 2; x >>= q; r |= q;
                                 q = (x > 0x3u   ) << 1; x >>= q; r |= q;
                                                                  r |= (x >> 1);
                        _rD_ = r;
                    }
                } break;

                case GFU_OPFN_CLO: { // 0x29  clo reg $d, reg $l
                    if (_iD_ != _rnZero_) {
                        gfu_long x = ~_rL_;
                        gfu_long r = (x > 0xFFFFu) << 4; x >>= r;
                        gfu_long q = (x > 0xFFu  ) << 3; x >>= q; r |= q;
                                 q = (x > 0xFu   ) << 2; x >>= q; r |= q;
                                 q = (x > 0x3u   ) << 1; x >>= q; r |= q;
                                                                  r |= (x >> 1);
                        _rD_ = r;
                    }
                } break;

                case GFU_OPFN_SLT: { // 0x2A  slt reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = (gfu_uword)((gfu_word)_rL_ < (gfu_word)_rR_);
                } break;

                case GFU_OPFN_SLTU: { // 0x2B  sltu reg $d, reg $l, reg $r
                    if (_iD_ != _rnZero_) _rD_ = (gfu_uword)(_rL_ < _rR_);
                } break;

                // TODO(echoe): Trap instructions
            }
        } break;

        case GFU_OP_COP0: {
            switch (_iFn_) {
                default: {
                    assertf(false, "Unimplemented COP0 function %02X.", (unsigned char)(_iFn_));
                } break;

                case GFU_C0FN_MF: {
                    gfu_uword reg = _iL_ + _iSh_;
                    if (_iD_ != _rnZero_) _rD_ = _c0N_(reg);
                } break;

                case GFU_C0FN_MT: {
                    gfu_uword reg = _iD_ + _iSh_;
                    switch (reg) {
                        default: _c0N_(reg) = _rL_; break;
                        case 12: { /* IRQ Status */
                            _c0Status_ = _rL_;
                            gfusx_test_sw_interrupts(vm);
                        } break;
                        case 13: { /* IRQ Cause */
                            _c0Cause_ = _rL_ & 0x03FF;
                            gfusx_test_sw_interrupts(vm);
                        } break;
                    }
                } break;
            }
        } break;
    }

    return false;
}

static void gfusx_dumpreg(gfusx_t* vm) {
    static const char* regname[34] = {
        " zero", "  at", "  r0", "  r1",
        "   a0", "  a1", "  a2", "  a3",
        "   t0", "  t1", "  t2", "  t3",
        "   t4", "  t5", "  t6", "  t7",
        "   s0", "  s1", "  s2", "  s3",
        "   s4", "  s5", "  s6", "  s7",
        "   t8", "  t9", "  k0", "  k1",
        "   gp", "  sp", "  fp", "  ra",
        "   hi", "  lo",
    };

    fprintf(stderr, "CPU Registers:\n");
    fprintf(stderr, "   pc %08X\n", _rPC_);
    for (int i = 0; i < 9; i++) {
        int begin = i * 4;
        int end = begin + 4;
        for (int j = begin; j < end && j < 34; j++) {
            fprintf(stderr, "%s %08X ", regname[j], _rN_(j));
        }
        fputc('\n', stderr);
    }

    fputc('\n', stderr);
}

static void gfusx_dumpstack(gfusx_t* vm) {
    gfu_uword sp = _rSP_ & ~0x1E;
    gfu_uword ram_size = gfusx_memory_read_hwreg_half(vm, GFU_RAM_SIZE_ADDR & 0xFFFF) == GFU_RAM_SIZE_LARGE
        ? GFU_MAIN1_RAM_SIZE : GFU_MAIN0_RAM_SIZE;

    if (
        sp < (GFU_KSEG0_BASE + GFU_WRAM_BASE + GFU_BIOS_RAM_SIZE) ||
        sp >= (GFU_KSEG0_BASE + GFU_WRAM_BASE + GFU_BIOS_RAM_SIZE + ram_size)
    ) {
        return;
    }

    gfu_uword* stack_top = (gfu_uword*)gfusx_memory_get_rptr(vm, sp);
    assert(stack_top != nullptr, "Stack pointer should have been in range.");
    gfu_uword* stack_bottom = (gfu_uword*)gfusx_memory_get_rptr(vm, GFU_KSEG0_BASE + GFU_WRAM_BASE + GFU_WRAM_SIZE);
    assert(stack_top != nullptr, "Stack pointer should have been in range.");

    if (gfusx_memory_read_hwreg_half(vm, GFU_RAM_SIZE_ADDR & 0xFFFF) == GFU_RAM_SIZE_LARGE) {
        stack_bottom += GFU_MAIN1_RAM_SIZE;
    } else {
        stack_bottom += GFU_MAIN0_RAM_SIZE;
    }

    gfu_uword stack_size = cast(gfu_uword, stack_bottom - stack_top);
    if (stack_size == 0) return;

    fprintf(stderr, "Stack           +00      +04      +08      +0C      +10      +14      +18      +1C\n");

    for (gfu_uword* stack = stack_top; stack < stack_bottom; stack += 8) {
        fprintf(stderr, "%08X | ", (gfu_uword)(gfu_ulong)stack);
        for (gfu_uword i = 0; i < 8 && stack + i < stack_bottom; i++) {
            fprintf(stderr, "%08X ", *(stack + i));
        }
        fputc('\n', stderr);
    }

    fputc('\n', stderr);
}

void gfusx_exec_block(gfusx_t* vm) {
    while (!vm->halt) {
        gfu_ulong begin_cycle_count = vm->cpu.cycle;

        gfu_uword pc = _rPC_;
        gfu_uword code = gfusx_memory_read_inst(vm, pc);

        _rPC_ += 4;
        // TODO(echoe): Put more "accurate" cycle counts into varying instructions and operations.
        vm->cpu.cycle += GFUSX_CYCLE_BIAS;

        bool jumped = gfusx_step(vm, pc, code);

        gfu_ulong end_cycle_count = vm->cpu.cycle;
        assert(end_cycle_count > begin_cycle_count, "Missing cycle counting on some instructions.");

        if (jumped) {
            gfusx_branch_test(vm);
            break;
        }

    }

    if (vm->trace) {
        gfusx_dumpreg(vm);
        gfusx_dumpstack(vm);
    }
}
