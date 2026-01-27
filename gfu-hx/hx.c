#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* shift(int* argc, char*** argv) {
    if (*argc == 0) return NULL;
    const char* arg = **argv;
    (*argc)--;
    (*argv)++;
    return arg;
}

static bool parse_ull(const char* arg, const char* opt, int64_t* ull) {
    if (arg == NULL) {
        fprintf(stderr, "error: '%s' requires an argument.\n", opt);
        return false;
    }

    int base = 10;
    if (0 == strncmp("0x", arg, 2)) {
        arg += 2;
        base = 16;
    } else if (0 == strncmp("0", arg, 1)) {
        arg += 1;
        base = 8;
    }

    errno = 0;
    char* arg_end = NULL;
    *ull = strtoll(arg, &arg_end, base);

    if (errno == ERANGE) {
        fprintf(stderr, "error: argument '%s' to '%s' is out of range.\n", arg, opt);
        return false;
    } else if (arg_end != arg + strlen(arg)) {
        fprintf(stderr, "error: argument '%s' to '%s' is not an integer.\n", arg, opt);
        return false;
    }

    return true;
}

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

int main(int argc, char** argv) {
    bool verbose = false;

    bool print_help = false;
    bool print_version = false;
    bool print_bits = false;
    bool print_plain = false;
    bool print_cinclude = false;

    const char* input_file_path = NULL;
    const char* cinclude_name = NULL;

    int64_t column_count = 16;
    int64_t group_byte_count = 1;

    int64_t seek = 0;
    int64_t length = INT64_MAX;

    const char* program_name = shift(&argc, &argv);

    while (argc > 0) {
        const char* opt = shift(&argc, &argv);
        if (0 == strcmp(opt, "-###")) {
            verbose = true;
        } else if (0 == strcmp(opt, "-h") || 0 == strcmp(opt, "-help") || 0 == strcmp(opt, "--help")) {
            print_help = true;
        } else if (0 == strcmp(opt, "-v") || 0 == strcmp(opt, "-version") || 0 == strcmp(opt, "--version")) {
            print_version = true;
        } else if (0 == strcmp(opt, "-b") || 0 == strcmp(opt, "-bits") || 0 == strcmp(opt, "--bits") || 0 == strcmp(opt, "--binary")) {
            print_bits = true;
        } else if (0 == strcmp(opt, "-p") || 0 == strcmp(opt, "-ps") || 0 == strcmp(opt, "-postscript") || 0 == strcmp(opt, "-plain") || 0 == strcmp(opt, "--plain")) {
            print_plain = true;
        } else if (0 == strcmp(opt, "-i") || 0 == strcmp(opt, "-include") || 0 == strcmp(opt, "--include")) {
            print_cinclude = true;
        } else if (0 == strcmp(opt, "-n") || 0 == strcmp(opt, "--name")) {
            cinclude_name = shift(&argc, &argv);
            if (cinclude_name == NULL) {
                fprintf(stderr, "error: '%s' requires an argument.\n", opt);
                return 1;
            }
        } else if (0 == strcmp(opt, "-c") || 0 == strcmp(opt, "-cols") || 0 == strcmp(opt, "--columns")) {
            if (!parse_ull(shift(&argc, &argv), opt, &column_count)) return 1;
        } else if (0 == strcmp(opt, "-g") || 0 == strcmp(opt, "-groupsize") || 0 == strcmp(opt, "--group") || 0 == strcmp(opt, "--group-size")) {
            if (!parse_ull(shift(&argc, &argv), opt, &group_byte_count)) return 1;
        } else if (0 == strcmp(opt, "-s") || 0 == strcmp(opt, "-seek") || 0 == strcmp(opt, "--seek") || 0 == strcmp(opt, "--offset")) {
            if (!parse_ull(shift(&argc, &argv), opt, &seek)) return 1;
        } else if (0 == strcmp(opt, "-l") || 0 == strcmp(opt, "-len") || 0 == strcmp(opt, "--length") || 0 == strcmp(opt, "--count")) {
            if (!parse_ull(shift(&argc, &argv), opt, &length)) return 1;
        } else {
            input_file_path = opt;
        }
    }

    bool options_errors = false;

    if (column_count < 1 || column_count > 256) {
        fprintf(stderr, "error: column count must be in the range [1, 256].\n");
        options_errors = true;
    }

    if (group_byte_count < 1 || group_byte_count > 256) {
        fprintf(stderr, "error: group byte count must be in the range [1, 256].\n");
        options_errors = true;
    }

    if (length < 0) {
        fprintf(stderr, "error: length must not be negative.\n");
        options_errors = true;
    }

    if (print_cinclude && cinclude_name == NULL) {
        fprintf(stderr, "error: C include mode requires a name.\n");
        options_errors = true;
    }

    if (options_errors) return 1;

    FILE* stream = stdin;
    if (input_file_path != NULL) {
        errno = 0;
        stream = fopen(input_file_path, "rb");
        if (stream == NULL) {
            fprintf(stderr, "error: failed to open '%s': %s.\n", input_file_path, strerror(errno));
            return 1;
        }
    }

    if (seek > 0) {
        errno = 0;
        if (0 != fseek(stream, (long)seek, SEEK_CUR)) {
            fprintf(stderr, "error: failed to seek input: %s.\n", strerror(errno));
            if (input_file_path != NULL) fclose(stream);
            return 1;
        }

        seek = ftell(stream);
    } else if (seek < 0) {
        errno = 0;
        if (0 != fseek(stream, (long)-seek, SEEK_END)) {
            fprintf(stderr, "error: failed to seek input: %s.\n", strerror(errno));
            if (input_file_path != NULL) fclose(stream);
            return 1;
        }

        seek = ftell(stream);
    }

    char buffer[256];
    int nread = 0;

    int64_t total_read = 0;
    int64_t group_counter = 0;
    size_t next_read_count = 0;

    void (*print_octet)(char) = print_bits ? print_octet_bits : print_octet_hex;

    if (print_cinclude) {
        fprintf(stdout, "unsigned char %s[] = {\n", cinclude_name);
    }

    setvbuf(stdout, NULL, _IONBF, 0);
    while (next_read_count = (size_t)minll(length - total_read, column_count), errno = 0, nread = fread(buffer, 1, next_read_count, stream), nread > 0) {
        total_read += nread;
        group_counter = 0;

        if (print_plain) {
            for (int i = 0; i < nread; i++)
                print_octet(buffer[i]);
            for (int i = nread; i < column_count; i++)
                fprintf(stdout, "%s", print_bits ? "        " : "  ");
            fputc('\n', stdout);
        } else if (print_cinclude) {
            fprintf(stdout, "    ");
            for (int i = 0; i < nread; i++)
                fprintf(stdout, "0x%02hhX, ", buffer[i]);
            fputc('\n', stdout);
        } else {
            fprintf(stdout, "%016"PRIX64": ", seek + total_read - nread);

            for (int i = 0; i < nread; i++, group_counter++) {
                if (group_counter == group_byte_count) {
                    group_counter = 0;
                    fputc(' ', stdout);
                }

                print_octet(buffer[i]);
            }

            for (int i = nread; i < column_count; i++, group_counter++) {
                if (group_counter == group_byte_count) {
                    group_counter = 0;
                    fputc(' ', stdout);
                }

                fprintf(stdout, "%s", print_bits ? "        " : "  ");
            }

            fprintf(stdout, "  ");
            for (int i = 0; i < nread; i++) {
                int c = buffer[i];
                if (c >= 32 && c <= 127)
                    fputc(c, stdout);
                else fputc('.', stdout);
            }

            for (int i = nread; i < column_count; i++)
                fputc(' ', stdout);

            fputc('\n', stdout);
        }
    }

    if (print_cinclude) {
        fprintf(stdout, "};\nunsigned long long %s_len = %"PRIi64";\n", cinclude_name, total_read);
    }

    if (input_file_path != NULL) fclose(stream);

    return 0;
}
