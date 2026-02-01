#ifndef GAMEFU_HX_H_
#define GAMEFU_HX_H_

#include <gamefu/common.h>

typedef struct {
    bool print_bits;
    bool print_plain;
    bool print_cinclude;
    const char* cinclude_name;
    int64_t visual_offset;
    int64_t column_count;
    int64_t group_byte_count;
} hx_opts;

typedef struct {
    FILE* output_stream;
    char* temp;
    size_t temp_size;
    size_t bytes_printed; // NOTE(nic): do not access directly, use API function
    hx_opts opts;
} hx_dump_state;

hx_opts hx_default_opts(void);

void hx_dump(FILE* output_stream, void* buff, size_t length, size_t offset);
void hx_dump_opt(FILE* output_stream, void* buff, size_t length, size_t offset, hx_opts opts);

hx_dump_state* hx_dump_begin(FILE* output_stream, hx_opts opts);
hx_dump_state* hx_dump_begin_file(const char* path, hx_opts opts);
void hx_dump_add(hx_dump_state* state, void* buff, size_t length, size_t offset);
void hx_dump_end(hx_dump_state* state);
size_t hx_dump_count(hx_dump_state* state);

#endif /* GAMEFU_HX_H_ */
