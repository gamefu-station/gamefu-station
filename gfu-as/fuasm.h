#ifndef GAMEFU_GFUAS_FUASM_H_
#define GAMEFU_GFUAS_FUASM_H_

#include <gamefu/common.h>

int fuasm_driver_main(int argc, char** argv);
int fuasm_driver_fuzz(const char* text, size_t length);

#endif /* GAMEFU_GFUAS_FUASM_H_ */
