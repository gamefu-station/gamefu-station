#ifndef GAMEFU_FUASM_H_
#define GAMEFU_FUASM_H_

#include "../common/common.h"

int fuasm_driver_main(int argc, char** argv);
int fuasm_driver_fuzz(const char* text, size_t length);

#endif /* GAMEFU_FUASM_H_ */
