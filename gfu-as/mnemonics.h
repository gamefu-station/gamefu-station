#ifndef GAMEFU_GFUAS_MNEMONICS_H_
#define GAMEFU_GFUAS_MNEMONICS_H_

typedef enum gfuas_mnemonic {
    GFUAS_MNEM_INVALID,
#define MNEM(Id, Name) GFUAS_MNEM_##Id,
#include "x/mnemonics.h"
} gfuas_mnemonic;

#endif /* GAMEFU_GFUAS_MNEMONICS_H_ */
