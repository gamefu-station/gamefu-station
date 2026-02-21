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
#  define LIB ".lib"
#else
#  define EXE ""
#  define LIB ".a"
#endif

#if __GAMEFU_HOSTCC_MSVC__
#  define gfu_cmd_cc(Cmd) \
    nob_cmd_append((Cmd), "cl.exe");
#  define gfu_cmd_ar(Cmd, Out) \
    nob_cmd_append((Cmd), "lib.exe", nob_temp_sprintf("/OUT:%s", (Out)));
#  define gfu_cmd_exe(Cmd, Out) \
    nob_cmd_append((Cmd), nob_temp_sprintf("/Fe:%s", (Out)));
#  define gfu_cmd_obj(Cmd, Out) \
    nob_cmd_append((Cmd), nob_temp_sprintf("/Fo:%s", (Out), "/c"));
#  define gfu_cmd_cflags(Cmd) \
    nob_cmd_append((Cmd), "/Iinclude");
#else
#  define gfu_cmd_cc(Cmd) \
    nob_cmd_append((Cmd), "cc");
#  define gfu_cmd_ar(Cmd, Out) \
    nob_cmd_append((Cmd), "ar", "rcs", (Out));
#  define gfu_cmd_exe(Cmd, Out) \
    nob_cmd_append((Cmd), "-o", (Out));
#  define gfu_cmd_obj(Cmd, Out) \
    nob_cmd_append((Cmd), "-o", (Out), "-c");
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


static bool
delete_directory_recursively(const char *path)
{
    bool result = true;
    Nob_File_Paths children = {0};
    size_t temp_checkpoint = nob_temp_save();

    Nob_File_Type type = nob_get_file_type(path);
    if (type < 0) nob_return_defer(false);

    switch (type) {
        default: NOB_UNREACHABLE("delete_directory_recursively");

        case NOB_FILE_DIRECTORY: {
            if (!nob_read_entire_dir(path, &children)) nob_return_defer(false);
            for (size_t i = 0; i < children.count; i++) {
                if (strcmp(children.items[i], ".") == 0) continue;
                if (strcmp(children.items[i], "..") == 0) continue;

                const char* child_path = nob_temp_sprintf("%s/%s", path, children.items[i]);
                if (!delete_directory_recursively(child_path)) nob_return_defer(false);
            }

#if defined(_WIN32)
            if (0 == RemoveDirectory(path)) {
                nob_log(NOB_ERROR, "Failed to remove directory '%s'.", path);
                nob_return_defer(false);
            }
#else
            if (0 != remove(path)) {
                nob_log(NOB_ERROR, "Failed to remove directory '%s'.", path);
                nob_return_defer(false);
            }
#endif
        } break;

        case NOB_FILE_REGULAR: {
            if (!nob_delete_file(path)) nob_return_defer(false);
        } break;

        case NOB_FILE_SYMLINK: {
            nob_log(NOB_WARNING, "TODO: Deleting symlinks is not supported yet.");
        } break;

        case NOB_FILE_OTHER: {
            nob_log(NOB_ERROR, "Unsupported type of file '%s'.", path);
            nob_return_defer(false);
        } break;
    }

    for (size_t i = 0; i < children.count; ++i) {
        if (strcmp(children.items[i], ".") == 0) continue;
        if (strcmp(children.items[i], "..") == 0) continue;
    }
defer:
    nob_temp_rewind(temp_checkpoint);
    nob_da_free(children);
    return result;
}


static Nob_Cmd cmd = {0};


static inline bool
gfu_cmd_glfw3_obj(const char* obj, const char* src)
{
    gfu_cmd_cc(&cmd);
    gfu_cmd_obj(&cmd, obj);
    nob_cmd_append(&cmd, src);
    nob_cmd_append(&cmd, "-Wno-macro-redefined");
    nob_cmd_append(&cmd, "-D_GNU_SOURCE");
    nob_cmd_append(&cmd, "-Iinclude");
    return nob_cmd_run(&cmd, 0);
}


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

    if (argc >= 2 && 0 == strcmp("clean", argv[1])) {
        if (nob_file_exists("gfu-iselgen/isel_source.h")) {
            if (!nob_delete_file("gfu-iselgen/isel_source.h")) {
                return 1;
            }
        }

        if (nob_file_exists("gfu-as/isel_tables.c")) {
            if (!nob_delete_file("gfu-as/isel_tables.c")) {
                return 1;
            }
        }

        if (nob_file_exists("include/gamefu/as/x/mnemonics.h")) {
            if (!nob_delete_file("include/gamefu/as/x/mnemonics.h")) {
                return 1;
            }
        }

        if (nob_file_exists("bios.gfu")) {
            if (!nob_delete_file("bios.gfu")) {
                return 1;
            }
        }

        if (nob_file_exists("gfu-sx/default_bios.h")) {
            if (!nob_delete_file("gfu-sx/default_bios.h")) {
                return 1;
            }
        }

        if (!delete_directory_recursively("out")) return 1;

        return 0;
    }

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

    /* Run ISel to generate 'gfu-as/isel_tables.c' and 'as/x/mnemonics.h'. */
    nob_cmd_append(&cmd, "out/gfu-iselgen");
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Build 'gfu-as' assembler. */
    gfu_cmd_link("out/gfu-as" EXE, "gfu-as/main.c");
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Run AS to generate 'bios.gfu'. */
    nob_cmd_append(&cmd, "out/gfu-as", "-o", "bios.gfu");
    nob_cmd_append(&cmd, "lib/bios/boot.fus");
    if (!nob_cmd_run(&cmd, 0)) {
        return 1;
    }

    /* Run HX to generate 'gfu-sx/default_bios.h'. */
    nob_cmd_append(&cmd, "out/gfu-hx", "bios.gfu");
    nob_cmd_append(&cmd, "-i", "-n", "gfusx_default_bios");
    if (!nob_cmd_run(&cmd, .stdout_path = "gfu-sx/default_bios.h")) {
        return 1;
    }

    /* Build GLFW static library. */
    if (!gfu_cmd_glfw3_obj("out/o/glfw3.o", "gfu-sx/glfw3/glfw3.c")) return 1;
    if (!gfu_cmd_glfw3_obj("out/o/glfw3_linux.o", "gfu-sx/glfw3/glfw3_linux.c")) return 1;
    if (!gfu_cmd_glfw3_obj("out/o/glfw3_null.o", "gfu-sx/glfw3/glfw3_null.c")) return 1;
    if (!gfu_cmd_glfw3_obj("out/o/glfw3_osmesa.o", "gfu-sx/glfw3/glfw3_osmesa.c")) return 1;
    if (!gfu_cmd_glfw3_obj("out/o/glfw3_win32.o", "gfu-sx/glfw3/glfw3_win32.c")) return 1;
    if (!gfu_cmd_glfw3_obj("out/o/glfw3_wl.o", "gfu-sx/glfw3/glfw3_wl.c")) return 1;
    if (!gfu_cmd_glfw3_obj("out/o/glfw3_x11.o", "gfu-sx/glfw3/glfw3_x11.c")) return 1;

    gfu_cmd_ar(&cmd, "out/libglfw3" LIB);
    nob_cmd_append(&cmd, "out/o/glfw3.o");
    nob_cmd_append(&cmd, "out/o/glfw3_linux.o");
    nob_cmd_append(&cmd, "out/o/glfw3_null.o");
    nob_cmd_append(&cmd, "out/o/glfw3_osmesa.o");
    nob_cmd_append(&cmd, "out/o/glfw3_win32.o");
    nob_cmd_append(&cmd, "out/o/glfw3_wl.o");
    nob_cmd_append(&cmd, "out/o/glfw3_x11.o");
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Build 'gfu-hx' emulator. */
    gfu_cmd_link("out/gfu-sx" EXE, "gfu-sx/main.c");
#if __GAMEFU_HOSTCC_MSVC__
#else
    nob_cmd_append(&cmd, "-lm");
    nob_cmd_append(&cmd, "-DGFUSX_PLATFORM_GLFW");
    nob_cmd_append(&cmd, "-ggdb", "-fsanitize=address");
#endif
    nob_cmd_append(&cmd, "out/libglfw3" LIB);
    if (!nob_cmd_run(&cmd, 0)) return 1;

    /* Perfect. */
    return 0;
}
