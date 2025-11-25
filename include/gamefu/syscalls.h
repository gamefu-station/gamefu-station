#ifndef GAMEFU_SYSCALLS_H_
#define GAMEFU_SYSCALLS_H_

#include <gamefu/prologue.h>

#define FUASM_SYSCALLS(X) \
    X(RESERVED00, 0x00) \
    X(FILE_WRITE_STRING, 0x43) \
    X(FILE_WRITE_FORMAT, 0x45)

typedef enum fuasm_syscall {
#define X(Id, Value) FUASM_SYS_##Id = (Value),
    FUASM_SYSCALLS(X)
#undef X
} fuasm_syscall, fuasm_syscall_t;

#endif /* GAMEFU_SYSCALLS_H_ */
