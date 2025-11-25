#ifndef GAMEFU_SOURCE_H_
#define GAMEFU_SOURCE_H_

#include "gamefu.h"

typedef struct source {
    const char* name;
    const char* text;
    int32_t length;
} source;

bool load_source_from_file(const char* path, source* source);

#endif /* GAMEFU_SOURCE_H_ */
