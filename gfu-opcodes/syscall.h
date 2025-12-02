#ifndef GAMEFU_OPCODES_SYSCALL_H_
#define GAMEFU_OPCODES_SYSCALL_H_

#include <gamefu/common.h>

#define GFU_SYSCALL(X) \
    X(RESERVED00, 0x00) \
    X(FILE_WRITE_STRING, 0x43) \
    X(FILE_WRITE_FORMAT, 0x45)

typedef enum gfu_syscall {
#define X(Id, Value) GFU_SYS_##Id = (Value),
    GFU_SYSCALL(X)
#undef X
} gfu_syscall;

#endif /* GAMEFU_OPCODES_SYSCALL_H_ */
