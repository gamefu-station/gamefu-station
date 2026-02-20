/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


/* Include this to get some platform macros and target defines for "free". */
#include "include/gamefu/common/prelude.h"


#define NOB_IMPLEMENTATION
#define NOB_WARN_DEPRECATED
#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_UNSTRIP_PREFIX
#include "nob.h"


#if __GAMEFU_HOSTOS_WINDOWS__
#  define EXE ".exe"
#else
#  define EXE ""
#endif

#if __GAMEFU_HOSTCC_MSVC__
#  define gfu_cmd_cc(Cmd) \
    nob_cmd_append((Cmd), "cl.exe");
#  define gfu_cmd_exe(Cmd, Out) \
    nob_cmd_append((Cmd), nob_temp_sprintf("/Fe:%s", (Out)));
#  define gfu_cmd_cflags(Cmd) \
    nob_cmd_append((Cmd), "/Iinclude");
#else
#  define gfu_cmd_cc(Cmd) \
    nob_cmd_append((Cmd), "cc");
#  define gfu_cmd_exe(Cmd, Out) \
    nob_cmd_append((Cmd), "-o", (Out));
#  define gfu_cmd_cflags(Cmd) \
    nob_cmd_append((Cmd), "-Iinclude"); \
    nob_cmd_append((Cmd), "-std=c99", "-Wall"); \
    nob_cmd_append((Cmd), "-Wreturn-type"); \
    nob_cmd_append((Cmd), "-Wno-unused"); \
    nob_cmd_append((Cmd), "-Wno-unused-parameter"); \
    nob_cmd_append((Cmd), "-Wno-stringop-truncation"); \
    nob_cmd_append((Cmd), "-Wenum-compare"); \
    nob_cmd_append((Cmd), "-Wenum-conversion"); \
    nob_cmd_append((Cmd), "-Wtype-limits"); \
    nob_cmd_append((Cmd), "-Wparentheses"); \
    nob_cmd_append((Cmd), "-Wno-unknown-warning-option"); \
    nob_cmd_append((Cmd), "-Werror", "-pedantic", "-pedantic-errors");
#endif


static Nob_Cmd cmd = {0};


static inline void
gfu_cmd_link(const char* exe, const char* src)
{
    gfu_cmd_cc(&cmd);
    gfu_cmd_exe(&cmd, exe);
    gfu_cmd_cflags(&cmd);
    nob_cmd_append(&cmd, src);
}


int
main(int argc, char** argv)
{
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");

    /* Gotta put the output somewhere. */
    if (!nob_mkdir_if_not_exists("out")) return 1;
    if (!nob_mkdir_if_not_exists("out/o")) return 1;

    /* Build 'gfu-hx' utility. */
    gfu_cmd_link("out/gfu-hx" EXE, "gfu-hx/main.c");
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Run HX to generate 'gfu-isel/isel_source.h'. */
    nob_cmd_append(&cmd, "out/gfu-hx", "gfu-iselgen/isel.txt");
    nob_cmd_append(&cmd, "-i", "-n", "isel");
    if (!nob_cmd_run(&cmd, .stdout_path = "gfu-iselgen/isel_source.h")) {
        return 1;
    }

    /* Build 'gfu-iselgen' utility. */
    gfu_cmd_link("out/gfu-iselgen" EXE, "gfu-iselgen/main.c");
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Run ISel table generator to generate 'gfu-as/isel_tables.c'. */
    nob_cmd_append(&cmd, "out/gfu-iselgen");
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Build 'gfu-as' assembler. */
    gfu_cmd_link("out/gfu-as" EXE, "gfu-as/main.c");
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Perfect. */
    return 0;
}
