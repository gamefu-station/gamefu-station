#include <gamefu/hx.h>

static int64_t minll(int64_t a, int64_t b) {
    return a < b ? a : b;
}

static void print_octet_bits(char c) {
    static char bit[2] = {'0', '1'};
    for (int j = 0; j < 8; j++) {
        fputc(bit[(c & 0x80) >> 7], stdout);
        c <<= 1;
    }
}

static void print_octet_hex(char c) {
    fprintf(stdout, "%02hhX", c);
}

hx_opts hx_default_opts(void) {
    return (hx_opts) {
        .column_count = 16,
        .group_byte_count = 1,
    };
}

void hx_dump(FILE* out_stream, void* buff, size_t length) {
    hx_opts opts = hx_default_opts();
    hx_dump_opt(out_stream, buff, length, opts);
}

bool advance(void* buff, size_t length, size_t* pointer, char* window, size_t nread) {
    if (*pointer >= length) return false;
    memcpy(window, (char*)buff + *pointer, nread);
    *pointer += nread;
    return true;
}

void hx_dump_opt(FILE* out_stream, void* buff, size_t length, hx_opts opts) {
    char window[256];
    int nread = 0;

    int64_t total_read = 0;
    int64_t group_counter = 0;

    size_t pointer = opts.offset;
    if (pointer >= length) {
        return;
    }

    void (*print_octet)(char) = opts.print_bits ? print_octet_bits : print_octet_hex;

    if (opts.print_cinclude) {
        fprintf(out_stream, "unsigned char %s[] = {\n", opts.cinclude_name);
    }

    setvbuf(out_stream, NULL, _IONBF, 0);
    while (nread = (size_t)minll(length - total_read, opts.column_count), advance(buff, length, &pointer, window, nread)) {
        total_read += nread;
        group_counter = 0;

        if (opts.print_plain) {
            for (int i = 0; i < nread; i++)
                print_octet(window[i]);
            for (int i = nread; i < opts.column_count; i++)
                fprintf(out_stream, "%s", opts.print_bits ? "        " : "  ");
            fputc('\n', out_stream);
        } else if (opts.print_cinclude) {
            fprintf(out_stream, "    ");
            for (int i = 0; i < nread; i++)
                fprintf(out_stream, "0x%02hhX, ", window[i]);
            fputc('\n', out_stream);
        } else {
            fprintf(out_stream, "%016"PRIX64": ", opts.offset + total_read - nread);

            for (int i = 0; i < nread; i++, group_counter++) {
                if (group_counter == opts.group_byte_count) {
                    group_counter = 0;
                    fputc(' ', out_stream);
                }

                print_octet(window[i]);
            }

            for (int i = nread; i < opts.column_count; i++, group_counter++) {
                if (group_counter == opts.group_byte_count) {
                    group_counter = 0;
                    fputc(' ', out_stream);
                }

                fprintf(out_stream, "%s", opts.print_bits ? "        " : "  ");
            }

            fprintf(out_stream, "  ");
            for (int i = 0; i < nread; i++) {
                int c = window[i];
                if (c >= 32 && c <= 127)
                    fputc(c, out_stream);
                else fputc('.', out_stream);
            }

            for (int i = nread; i < opts.column_count; i++)
                fputc(' ', out_stream);

            fputc('\n', out_stream);
        }
    }

    if (opts.print_cinclude) {
        fprintf(out_stream, "};\nunsigned long long %s_len = %"PRIi64";\n", opts.cinclude_name, total_read);
    }
}
