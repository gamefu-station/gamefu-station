#ifndef GFU_NOB_H_
#define GFU_NOB_H_

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE 1

#if defined(_MSC_VER) && !defined(__clang__)
#    define nob_cc_flags(...) // TODO: Add MSVC flags.
#else
#    define nob_cc_flags(cmd) nob_cmd_append((cmd), "-std=c23", "-Wall", "-Wextra", \
         "-Wno-unused", "-Wno-gnu-zero-variadic-macro-arguments", "-Wno-trigraphs", \
         "-Wno-unused-parameter", "-Wno-unused-function", "-Wno-unused-variable",   \
         "-Werror", "-ggdb", "-Werror=return-type", "-pedantic", "-pedantic-errors", "-fsanitize=address")
#endif

#if defined(_MSC_VER)
#    if defined(__clang__)
#        define gfu_nob_ld(cmd) nob_cmd_append((cmd), "clang")
#    else
#        define gfu_nob_ld(cmd) nob_cmd_append((cmd), "link.exe")
#    endif
#else
#    define gfu_nob_ld(cmd) nob_cmd_append((cmd), "ld")
#endif

#if defined(_MSC_VER)
#    if defined(__clang__)
#        define gfu_nob_ld_flags(cmd) nob_cmd_append((cmd), "-fsanitize=address")
#    else
#        define gfu_nob_ld_flags(cmd) nob_cmd_append((cmd), "/nologo")
#    endif
#else
#    define gfu_nob_ld_flags(cmd) nob_cmd_append((cmd), "-fsanitize=address")
#endif

#if defined(_MSC_VER)
#    if defined(__clang__)
#        define gfu_nob_ld_output(cmd, output_path) nob_cmd_append((cmd), "-o", (output_path))
#    else
#        define gfu_nob_ld_output(cmd, output_path) nob_cmd_append((cmd), nob_temp_sprintf("/out:%s", (output_path)))
#    endif
#else
#    define gfu_nob_ld_output(cmd, output_path) nob_cmd_append((cmd), "-o", (output_path))
#endif

#define gfu_nob_ld_inputs(cmd, ...) nob_cmd_append((cmd), __VA_ARGS__)

#if defined(_WIN32)
#    define gfu_nob_exe(exe_path) nob_temp_sprintf("%s.exe", (exe_path))
#else
#    define gfu_nob_exe(exe_path) exe_path
#endif

#include "nob.h"

bool gfu_nob_read_entire_dir_recursive(const char* dir, Nob_File_Paths* paths);

#endif /* GFU_NOB_H_ */

#ifdef GFU_NOB_IMPLEMENTATION
#undef GFU_NOB_IMPLEMENTATION

#define NOB_IMPLEMENTATION
#include "nob.h"
#undef NOB_IMPLEMENTATION

bool gfu_nob_read_entire_dir_recursive(const char* dir, Nob_File_Paths* paths) {
    bool result = true;

    Nob_File_Paths this_paths = {0};
    if (!nob_read_entire_dir(dir, &this_paths)) {
        nob_return_defer(false);
    }

    for (size_t i = 2; i < this_paths.count; i++) {
        const char* child_name = this_paths.items[i];
        const char* child_path = nob_temp_sprintf("%s/%s", dir, child_name);

        Nob_File_Type child_file_type = nob_get_file_type(child_path);
        if (child_file_type == NOB_FILE_DIRECTORY) {
            gfu_nob_read_entire_dir_recursive(child_path, paths);
        } else if (child_name[0] != '.') {
            nob_da_append(paths, child_path);
        }
    }

defer:;
    return result;
}

#endif /* GFU_NOB_IMPLEMENTATION */
