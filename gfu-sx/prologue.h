#ifndef GFUSX_PROLOGUE_H_
#define GFUSX_PROLOGUE_H_

#include "../common/common.h"

typedef struct gfusx gfusx_t;

#ifdef GFU_NTRACE
#  define gfusx_log(Vm, Message) do { } while (0)
#  define gfusx_logf(Vm, Format, ...) do { } while (0)
#else
#  define gfusx_log(Vm, Message) do { if ((Vm)->verbose) fprintf(stderr, "gfusx: " __FILE__ "(%d): %s\n", __LINE__, "" Message ""); } while (0)
#  define gfusx_logf(Vm, Format, ...) do { if ((Vm)->verbose) fprintf(stderr, "gfusx: " __FILE__ "(%d): " Format "\n", __LINE__, __VA_ARGS__); } while (0)
#endif

typedef struct gfusx_uwpair {
    gfu_uword l;
    gfu_uword r;
} gfusx_uwpair_t;

static_assert(sizeof(gfusx_uwpair_t) == sizeof(gfu_ulong), "uwpair should be two uwords, or one ulong in size.");

#define GFUSX_UWPAIR(L, R) ((gfusx_uwpair_t){(L), (R)})

#endif /* GFUSX_PROLOGUE_H_ */
