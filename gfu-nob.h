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
#        define nob_ar(cmd) nob_cmd_append((cmd), "llvm-lib.exe")
#    else
#        define nob_ar(cmd) nob_cmd_append((cmd), "lib.exe")
#    endif
#else
#    define nob_ar(cmd) nob_cmd_append((cmd), "ar", "rcs")
#endif

#define nob_ar_flags(cmd)

#if defined(_MSC_VER)
#    define nob_ar_output(cmd, output_path) nob_cmd_append((cmd), nob_temp_sprintf("/out:%s", (output_path)))
#else
#    define nob_ar_output(cmd, output_path) nob_cmd_append((cmd), (output_path))
#endif

#define nob_ar_inputs(cmd, ...) nob_cmd_append((cmd), __VA_ARGS__)

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

#if defined(_WIN32)
#    define gfu_nob_lib_a(lib_path) nob_temp_sprintf("%s.lib", (lib_path))
#else
#    define gfu_nob_lib_a(lib_path) nob_temp_sprintf("%s.a", (lib_path))
#endif

#if defined(_WIN32)
#    define gfu_nob_lib_so(lib_path) nob_temp_sprintf("%s.dll", (lib_path))
#else
#    define gfu_nob_lib_so(lib_path) nob_temp_sprintf("%s.so", (lib_path))
#endif

#include "nob.h"

#define gfu_nob_try(Result, Expr) do { if (!(Expr)) nob_return_defer(Result); } while (0)
#define gfu_nob_try_int(Result, Expr) do { if (0 != (Expr)) nob_return_defer(Result); } while (0)

bool gfu_nob_read_entire_dir_recursive(const char* dir, Nob_File_Paths* paths);

Nob_String_View gfu_nob_sv_file_name(Nob_String_View path);

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

bool gfu_nob_read_entire_dir_recursive_ext(const char* dir, const char* ext, Nob_File_Paths* paths) {
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
            gfu_nob_read_entire_dir_recursive_ext(child_path, ext, paths);
        } else if (child_name[0] != '.' && nob_sv_end_with(nob_sv_from_cstr(child_name), ext)) {
            nob_da_append(paths, child_path);
        }
    }

defer:;
    return result;
}

Nob_String_View gfu_nob_sv_file_name(Nob_String_View path) {
    size_t begin = path.count - 1;
    while (begin > 0) {
        if (path.data[begin] == '/' || path.data[begin] == '\\') {
            begin++;
            break;
        }
        begin--;
    }

    size_t end = begin + 1;
    while (end < path.count) {
        if (path.data[end] == '.')
            break;
        end++;
    }

    path.data += begin;
    path.count = end - begin;
    return path;
}

#endif /* GFU_NOB_IMPLEMENTATION */
