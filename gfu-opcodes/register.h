#ifndef GAMEFU_SYSTEM_REGISTER_H_
#define GAMEFU_SYSTEM_REGISTER_H_

#define GFU_GPR(X) \
    X(ZERO, "zero") \
    X(AT, "at") \
    X(R0, "r0") \
    X(R1, "r1") \
    X(A0, "a0") \
    X(A1, "a1") \
    X(A2, "a2") \
    X(A3, "a3") \
    X(T0, "t0") \
    X(T1, "t1") \
    X(T2, "t2") \
    X(T3, "t3") \
    X(T4, "t4") \
    X(T5, "t5") \
    X(T6, "t6") \
    X(T7, "t7") \
    X(S0, "s0") \
    X(S1, "s1") \
    X(S2, "s2") \
    X(S3, "s3") \
    X(S4, "s4") \
    X(S5, "s5") \
    X(S6, "s6") \
    X(S7, "s7") \
    X(T8, "t8") \
    X(T9, "t9") \
    X(K0, "k0") \
    X(K1, "k1") \
    X(GP, "gp") \
    X(SP, "sp") \
    X(FP, "fp") \
    X(RA, "ra")

typedef enum gfu_gpr {
#define X(Id, Image) GFU_GPR_##Id,
    GFU_GPR(X)
#undef X
    GFU_GPR_INVALID = 0xFF,
} gfu_gpr;

#define GFU_CP0R(X) \
    X(BAD_VADDR, 9, "bad_vaddr") \
    X(STATUS, 12, "status") \
    X(CAUSE, 13, "cause")

typedef enum gfu_cp0r {
#define X(Id, Value, Image) GFU_CP0R_##Id,
    GFU_CP0R(X)
#undef X
    GFU_CP0R_INVALID = 0xFF,
} gfu_cp0r;

gfu_gpr gfu_gpr_from_string(const char* s, size_t length);
gfu_cp0r gfu_cp0r_from_string(const char* s, size_t length);

#endif /* GAMEFU_SYSTEM_REGISTER_H_ */
