/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


/* just for the bootstrapping tool, don't echo. */
#define NOB_NO_ECHO
#include "build/build.h"

#include "build/platform.h"
#include "build/directories.h"


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

                const char* child_path = nob_temp_sprintf(
                    "%s/%s", path, children.items[i]
                );
                if (!delete_directory_recursively(child_path)) {
                    nob_return_defer(false);
                }
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
            nob_log(
                NOB_WARNING, "TODO: Deleting symlinks is not supported yet."
            );
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


static bool
bootstrap_build_tool(const char* exe_path, const char* source_path) {
    bool result = false;

    const char* build_nob_h = BUILD_DIR BUILD_PATH_SEP "nob.h";
    const char* build_config_h = BUILD_DIR BUILD_PATH_SEP "config.h";
    const char* build_directories_h = BUILD_DIR BUILD_PATH_SEP "directories.h";
    const char* build_platform_h = BUILD_DIR BUILD_PATH_SEP "platform.h";

    const char* exe_deps[] = {
        source_path, build_nob_h, build_config_h,
        build_directories_h, build_platform_h
    };

    bool needs_rebuild = nob_needs_rebuild(
        exe_path, exe_deps, sizeof(exe_deps) / sizeof(*exe_deps)
    );

    if (needs_rebuild) {
        if (!nob_mkdir_if_not_exists(BUILD_OUTPUT_DIR)) goto fail;
        Nob_Cmd cmd = {0};
        if (!BUILD_BOOTSTRAP(&cmd, exe_path, source_path)) goto fail;
    }

success:;
    result = true;
fail:;
    return result;
}

static bool
run_build(int argc, char** argv) {
    bool result = false;

    const char* build_src = BUILD_DIR BUILD_PATH_SEP "build.c";
    const char* build_exe =
        "." BUILD_PATH_SEP BUILD_OUTPUT_DIR
        BUILD_PATH_SEP "build" BUILD_EXE_EXT;
    if (!bootstrap_build_tool(build_exe, build_src)) goto fail;

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, build_exe);
    nob_da_append_many(&cmd, argv, (size_t) argc);
    if (!nob_cmd_run(&cmd, 0)) goto fail;

success:;
    result = true;
fail:;
    return result;
}

static bool
delete_file_if_exists(const char* path) {
    if (!nob_file_exists(path)) return true;
    return nob_delete_file(path);
}

static bool
clean(void) {
    if (!delete_file_if_exists(BUILD_DIR BUILD_PATH_SEP "config.h")) {
        return false;
    }
    if (!delete_file_if_exists("gfu-iselgen/isel_source.h")) return false;
    if (!delete_file_if_exists("gfu-as/isel_tables.c")) return false;
    if (!nob_file_exists(BUILD_OUTPUT_DIR)) return true;
    return delete_directory_recursively(BUILD_OUTPUT_DIR);
}

static void
print_help(const char* program_name) {
    fprintf(stdout, "Usage: %s [options..]\n", program_name);
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "    -h, --help          Show this help info\n");
    fprintf(stdout, "    --clean             Delete existing build artifacts first\n");
}


int
main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "build/nob.h", "build/build.h");

    int result = 1;
    Nob_Cmd cmd = {0};

    const char* program_name = shift(argv, argc);
    const char* command = NULL;
    if (argc > 0 && **argv != '-') command = nob_shift(&argv, &argc);

    for (int i = 0; i < argc; i++) {
        const char* opt = argv[i];
        if (0 == strcmp(opt, "--help") || 0 == strcmp(opt, "-?")) {
            print_help(program_name);
            goto success;
        }
    }

    bool is_config_command = command != NULL && 0 == strcmp(command, "config");
    bool is_clean_command = command != NULL && 0 == strcmp(command, "clean");
    if (
        (!nob_file_exists(build_config_h) || is_config_command) &&
        !is_clean_command
    ) {
        if (!configure(program_name, &argc, &argv, build_config_h)) goto fail;
        if (is_config_command) goto success;
    }
    if (!is_clean_command) assert(nob_file_exists(build_config_h));

    if (command == NULL || 0 == strcmp(command, "build")) {
        if (!run_build(argc, argv)) goto fail;
    } else if (0 == strcmp(command, "clean")) {
        if (!clean()) goto fail;
    } else {
        nob_log(NOB_ERROR, "Unrecognized command '%s'.", command);
        goto fail;
    }

success:;
    result = 0;
fail:;
    nob_cmd_free(cmd);
    return result;
}
