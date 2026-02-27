/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#if defined(_MSC_VER)
#  define EXE ".exe"
#else
#  define EXE ""
#endif

#ifndef nob_cc_flags
#  if defined(_MSC_VER) && !defined(__clang__)
#    define nob_cc_flags(cmd)  nob_cmd_append(cmd, \
        "/Iinclude" \
    )
#  elif defined(__clang__)
#    define nob_cc_flags(cmd)  nob_cmd_append(cmd, \
        "-Iinclude", \
        "-std=c99", \
        "-Wall", \
        "-Wno-unused-variable", \
        "-Wno-unused-function", \
        "-Wno-unused-label", \
        "-Wno-unused-but-set-variable", \
        "-Wreturn-type", \
        "-Werror", \
        "-pedantic", \
        "-pedantic-errors", \
        "-ggdb", \
        "-fsanitize=address,undefined,leak,integer" \
    )
#  else
#    define nob_cc_flags(cmd)  nob_cmd_append(cmd, \
        "-Iinclude", \
        "-std=c99", \
        "-Wall", \
        "-Wno-unused-variable", \
        "-Wno-unused-function", \
        "-Wno-unused-label", \
        "-Wno-unused-but-set-variable", \
        "-Wreturn-type", \
        "-Werror", \
        "-pedantic", \
        "-pedantic-errors", \
        "-ggdb", \
        "-fsanitize=address,undefined,leak" \
    )
#  endif
#endif // nob_cc_flags

#define NOB_IMPLEMENTATION
#define NOB_WARN_DEPRECATED
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"


static Cmd cmd = {0};


extern int
main(int argc, char** argv) {
    GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");

    if (!mkdir_if_not_exists("out")) return 1;

    nob_cc(&cmd);
    nob_cc_output(&cmd, "out/gfu-hx" EXE);
    nob_cc_inputs(&cmd, "src/hx.c");
    nob_cc_flags(&cmd);
    if (!cmd_run(&cmd, 0)) return 1;

    nob_cc(&cmd);
    nob_cc_output(&cmd, "out/gfu-iselgen" EXE);
    nob_cc_inputs(&cmd, "src/iselgen.c");
    nob_cc_flags(&cmd);
    if (!cmd_run(&cmd, 0)) return 1;

    nob_cc(&cmd);
    nob_cc_output(&cmd, "out/gfu-as" EXE);
    nob_cc_inputs(&cmd, "src/as.c");
    nob_cc_flags(&cmd);
    if (!cmd_run(&cmd, 0)) return 1;

    nob_cc(&cmd);
    nob_cc_output(&cmd, "out/gfu-cc" EXE);
    nob_cc_inputs(&cmd, "src/cc.c");
    nob_cc_flags(&cmd);
    if (!cmd_run(&cmd, 0)) return 1;

    nob_cc(&cmd);
    nob_cc_output(&cmd, "out/gfu-ar" EXE);
    nob_cc_inputs(&cmd, "src/ar.c");
    nob_cc_flags(&cmd);
    if (!cmd_run(&cmd, 0)) return 1;

    nob_cc(&cmd);
    nob_cc_output(&cmd, "out/gfu-ld" EXE);
    nob_cc_inputs(&cmd, "src/ld.c");
    nob_cc_flags(&cmd);
    if (!cmd_run(&cmd, 0)) return 1;

    nob_cc(&cmd);
    nob_cc_output(&cmd, "out/gfu-sx" EXE);
    nob_cc_inputs(&cmd, "src/sx.c");
    nob_cc_flags(&cmd);
    if (!cmd_run(&cmd, 0)) return 1;

    return 0;
}
