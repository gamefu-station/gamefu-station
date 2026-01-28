#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gamefu/hx.h>

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

int main(int argc, char** argv) {
    hx_opts opts = hx_default_opts();

    const char* input_file_path = NULL;
    int64_t length = -1;

    bool verbose = false;
    bool print_help = false;
    bool print_version = false;

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
            opts.print_bits = true;
        } else if (0 == strcmp(opt, "-p") || 0 == strcmp(opt, "-ps") || 0 == strcmp(opt, "-postscript") || 0 == strcmp(opt, "-plain") || 0 == strcmp(opt, "--plain")) {
            opts.print_plain = true;
        } else if (0 == strcmp(opt, "-i") || 0 == strcmp(opt, "-include") || 0 == strcmp(opt, "--include")) {
            opts.print_cinclude = true;
        } else if (0 == strcmp(opt, "-n") || 0 == strcmp(opt, "--name")) {
            opts.cinclude_name = shift(&argc, &argv);
            if (opts.cinclude_name == NULL) {
                fprintf(stderr, "error: '%s' requires an argument.\n", opt);
                return 1;
            }
        } else if (0 == strcmp(opt, "-c") || 0 == strcmp(opt, "-cols") || 0 == strcmp(opt, "--columns")) {
            if (!parse_ull(shift(&argc, &argv), opt, &opts.column_count)) return 1;
        } else if (0 == strcmp(opt, "-g") || 0 == strcmp(opt, "-groupsize") || 0 == strcmp(opt, "--group") || 0 == strcmp(opt, "--group-size")) {
            if (!parse_ull(shift(&argc, &argv), opt, &opts.group_byte_count)) return 1;
        } else if (0 == strcmp(opt, "-s") || 0 == strcmp(opt, "-seek") || 0 == strcmp(opt, "--seek") || 0 == strcmp(opt, "--offset")) {
            if (!parse_ull(shift(&argc, &argv), opt, &opts.offset)) return 1;
        } else if (0 == strcmp(opt, "-l") || 0 == strcmp(opt, "-len") || 0 == strcmp(opt, "--length") || 0 == strcmp(opt, "--count")) {
            if (!parse_ull(shift(&argc, &argv), opt, &length)) return 1;
            if (length < 0) {
                fprintf(stderr, "error: length must not be negative.\n");
                return 1;
            }
        } else {
            input_file_path = opt;
        }
    }

    bool options_errors = false;

    if (opts.column_count < 1 || opts.column_count > 256) {
        fprintf(stderr, "error: column count must be in the range [1, 256].\n");
        options_errors = true;
    }

    if (opts.group_byte_count < 1 || opts.group_byte_count > 256) {
        fprintf(stderr, "error: group byte count must be in the range [1, 256].\n");
        options_errors = true;
    }

    if (opts.print_cinclude && opts.cinclude_name == NULL) {
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

    errno = 0;
    if (0 != fseek(stream, 0, SEEK_END)) {
        fprintf(stderr, "error: failed to seek input: %s.\n", strerror(errno));
        if (input_file_path != NULL) fclose(stream);
        return 1;
    }

    errno = 0;
    long file_size = ftell(stream);
    if (file_size < 0) {
        fprintf(stderr, "error: failed to retrieve file size: %s.\n", strerror(errno));
        if (input_file_path != NULL) fclose(stream);
        return 1;
    }

    rewind(stream);

    if (opts.offset > 0) {
        errno = 0;
        if (0 != fseek(stream, (long)opts.offset, SEEK_CUR)) {
            fprintf(stderr, "error: failed to seek input: %s.\n", strerror(errno));
            if (input_file_path != NULL) fclose(stream);
            return 1;
        }

        opts.offset = ftell(stream);
    } else if (opts.offset < 0) {
        errno = 0;
        if (0 != fseek(stream, (long)-opts.offset, SEEK_END)) {
            fprintf(stderr, "error: failed to seek input: %s.\n", strerror(errno));
            if (input_file_path != NULL) fclose(stream);
            return 1;
        }

        opts.offset = ftell(stream);
    }

    if (length < 0) {
        length = file_size;
    }

    void *buffer = malloc(length);
    assertn(buffer != NULL);

    errno = 0;
    fread(buffer, 1, length, stream);
    if (ferror(stream)) {
        fprintf(stderr, "error: failed to read input: %s.\n", strerror(errno));
        if (buffer) free(buffer);
        if (input_file_path != NULL) fclose(stream);
        return 1;
    }

    hx_dump_opt(stdout, buffer, length, opts);

    if (buffer) free(buffer);
    if (input_file_path != NULL) fclose(stream);

    return 0;
}
