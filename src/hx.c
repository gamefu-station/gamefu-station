/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#include <gamefu/host_features.h>

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gamefu/cli_shift.h>
#include <gamefu/types.h>


static enum {
    MODE_GRID,
    MODE_C_HEADER,
} mode = MODE_GRID;

static FILE* infile = NULL;

static gfu_uword data_pos = 0;
static gfu_uword data_len = GFU_UWORD_MAX / 2;

static const char* c_header_ident = "bytes";


static bool emit_c_header(void);


extern int
main(int argc, char** argv) {
    int result = 1;

    const char* program_name = GFU_CLI_SHIFT();
    while (argc > 0) {
        const char* opt = GFU_CLI_SHIFT();
        if (0 == strcmp(opt, "-i")) {
            mode = MODE_C_HEADER;
        } else if (0 == strcmp(opt, "-n")) {
            c_header_ident = GFU_CLI_SHIFT();
            if (c_header_ident == NULL) {
                fprintf(stderr, "error: option `-n` requires an argument.\n");
                goto fail;
            }
        } else if (0 == strcmp(opt, "-b")) {
            const char* arg = GFU_CLI_SHIFT();
            if (arg == NULL) {
                fprintf(stderr, "error: option `-b` requires an argument.\n");
                goto fail;
            }
            data_pos = (gfu_uword) strtoul(arg, NULL, 10);
            fprintf(stderr, "%u\n", data_pos);
        } else {
            if (infile != NULL) {
                fprintf(stderr, "error: multiple input files.\n");
                goto fail;
            }
            errno = 0;
            infile = fopen(opt, "rb");
            if (infile == NULL) {
                fprintf(stderr, "error: %s.\n", strerror(errno));
                goto fail;
            }
        }
    }

    if (infile == NULL) {
        infile = stdin;
    }

    if (mode == MODE_C_HEADER) {
        if (!emit_c_header()) goto fail;
        else goto success;
    }

    printf("          00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F");

success:;
    result = 0;

fail:;
    if (infile != NULL && infile != stdin) {
        fclose(infile);
    }

    return 0;
}


static bool
emit_c_header(void) {
    assert(c_header_ident != NULL);

    gfu_uword count = 0;

    gfu_uword nread = 0;
    char buffer[1024];

    while (data_pos > 0 && !feof(infile)) {
        gfu_uword to_read = data_pos;
        if (to_read > sizeof(buffer)) {
            to_read = sizeof(buffer);
        }

        errno = 0;
        nread = (gfu_uword) fread(buffer, 1, to_read, infile);

        if (nread < to_read && ferror(infile)) {
            fprintf(stderr, "error: %s.\n", strerror(errno));
            return false;
        }

        data_pos -= nread;
    }

    printf("static const unsigned char %s[] = {\n", c_header_ident);

    while (count < data_len && !feof(infile)) {
        gfu_uword to_read = data_len - count;
        if (to_read > sizeof(buffer)) {
            to_read = sizeof(buffer);
        }

        errno = 0;
        nread = (gfu_uword) fread(buffer, 1, to_read, infile);

        if (nread < to_read && ferror(infile)) {
            fprintf(stderr, "error: %s.\n", strerror(errno));
            return false;
        }

        count += nread;
        if (nread == 0) break;

        for (gfu_uword i = 0; i < nread; i += 16) {
            for (gfu_uword j = 0; j < 16 && i + j < nread; j++) {
                printf("0x%02X,", (unsigned char) buffer[i + j]);
            }
            printf("\n");
        }
    }

    printf("};\n\n");
    printf("static const gfu_uword %s_count = %u;\n", c_header_ident, count);

    return true;
}
