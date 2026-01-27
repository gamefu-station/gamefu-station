#ifndef GAMEFU_HX_H_
#define GAMEFU_HX_H_

#include <gamefu/common.h>

typedef struct {
    bool print_bits;
    bool print_plain;
    bool print_cinclude;
    const char* cinclude_name;
    int64_t offset;
    int64_t column_count;
    int64_t group_byte_count;
} hx_opts;

hx_opts hx_default_opts(void);
void hx_dump(FILE* out_stream, void* buff, size_t length);
void hx_dump_opt(FILE* out_stream, void* buff, size_t length, hx_opts opts);

#endif /* GAMEFU_HX_H_ */
