#include <gamefu/hx.h>

static int64_t
minll(int64_t a, int64_t b) {
    return a < b ? a : b;
}

static void
print_octet_bits(unsigned char c) {
    static char bit[2] = {'0', '1'};
    for (int j = 0; j < 8; j++) {
        fputc(bit[(c & 0x80) >> 7], stdout);
        c <<= 1;
    }
}

static void
print_octet_hex(unsigned char c) {
    fprintf(stdout, "%02hhX", c);
}

static void
dump(hx_dump_state* state, void* buff, size_t length) {
    FILE* output_stream = state->output_stream;
    hx_opts opts = state->opts;

    size_t pointer = 0;
    size_t group_counter = 0;

    void (*print_octet)(unsigned char) = opts.print_bits
        ? print_octet_bits
        : print_octet_hex;

    while (pointer < length) {
        size_t nread = (size_t)minll(length - pointer, opts.column_count);
        char* window = (char*)buff + pointer;
        pointer += nread;
        group_counter = 0;

        if (opts.print_plain) {
            for (size_t i = 0; i < nread; i++)
                print_octet(window[i]);
            for (size_t i = nread; i < (size_t) opts.column_count; i++)
                fprintf(output_stream, "%s", opts.print_bits ? "        " : "  ");
            fputc('\n', output_stream);
        } else if (opts.print_cinclude) {
            fprintf(output_stream, "    ");
            for (size_t i = 0; i < nread; i++)
                fprintf(output_stream, "0x%02hhX, ", window[i]);
            fputc('\n', output_stream);
        } else {
            fprintf(
                output_stream,
                "%016"PRIX64": ",
                opts.visual_offset + state->bytes_printed + pointer - nread);

            for (size_t i = 0; i < nread; i++, group_counter++) {
                if (group_counter == (size_t) opts.group_byte_count) {
                    group_counter = 0;
                    fputc(' ', output_stream);
                }

                print_octet(window[i]);
            }

            for (size_t i = nread; i < (size_t) opts.column_count; i++, group_counter++) {
                if (group_counter == (size_t) opts.group_byte_count) {
                    group_counter = 0;
                    fputc(' ', output_stream);
                }

                fprintf(output_stream, "%s", opts.print_bits ? "        " : "  ");
            }

            fprintf(output_stream, "  ");
            for (size_t i = 0; i < nread; i++) {
                int c = window[i];
                if (c >= 32 && c <= 127)
                    fputc(c, output_stream);
                else fputc('.', output_stream);
            }

            for (size_t i = nread; i < (size_t) opts.column_count; i++)
                fputc(' ', output_stream);

            fputc('\n', output_stream);
        }
    }

    state->bytes_printed += length;
}

hx_opts
hx_default_opts(void) {
    return (hx_opts) {
        .column_count = 16,
        .group_byte_count = 1,
    };
}

void
hx_dump(FILE* output_stream, void* buff, size_t length, size_t offset) {
    hx_opts opts = hx_default_opts();
    hx_dump_opt(output_stream, buff, length, offset, opts);
}

void
hx_dump_opt(FILE* output_stream, void* buff, size_t length, size_t offset, hx_opts opts) {
    hx_dump_state* state = hx_dump_begin(output_stream, opts);
    hx_dump_add(state, buff, length, offset);
    hx_dump_end(state);
}

hx_dump_state*
hx_dump_begin(FILE* output_stream, hx_opts opts) {
    hx_dump_state *state = calloc(1, sizeof(*state));
    assertn(state != NULL);

    state->output_stream = output_stream;
    state->opts = opts;
    state->temp = malloc(opts.column_count);
    assertn(state->temp != NULL);

    if (opts.print_cinclude) {
        fprintf(output_stream, "unsigned char %s[] = {\n", opts.cinclude_name);
    }

    return state;
}

hx_dump_state*
hx_dump_begin_file(const char* path, hx_opts opts) {
    FILE* output_stream = fopen(path, "wb");
    if (output_stream == NULL) {
        // REVIEW(nic): bikeshed error reporting
        fprintf(stderr, "error: hx: could not open '%s': %s\n", path, strerror(errno));
        return NULL;
    }
    return hx_dump_begin(output_stream, opts);
}

void
hx_dump_add(hx_dump_state* state, void* buff, size_t length, size_t offset) {
    hx_opts opts = state->opts;
    buff = (char*)buff + offset;

    if (state->temp_size > 0 && state->temp_size + length > (size_t) opts.column_count) {
        size_t n = opts.column_count - state->temp_size;
        memcpy(state->temp + state->temp_size, buff, n);

        dump(state, state->temp, state->temp_size + n);
        state->temp_size = 0;

        buff = (char*)buff + n;
        length -= n;
    }

    size_t nrows = length / opts.column_count;
    size_t aligned_length = nrows * opts.column_count;

    size_t rest = length - aligned_length;
    if (rest > 0) {
        memcpy(state->temp, (char*)buff + length - rest, rest);
        state->temp_size = rest;
    }

    dump(state, buff, length - rest);
}

void
hx_dump_end(hx_dump_state* state) {
    if (state == NULL) return;

    if (state->temp_size > 0) {
        dump(state, state->temp, state->temp_size);
    }

    if (state->opts.print_cinclude) {
        fprintf(
            state->output_stream,
            "};\nunsigned long long %s_len = %"PRIi64";\n",
            state->opts.cinclude_name, state->bytes_printed);
    }

    free(state->temp);
    free(state);
}

size_t hx_dump_count(hx_dump_state* state) {
    if (state == NULL) return 0;
    return state->bytes_printed + state->temp_size;
}
