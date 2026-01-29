/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

#if defined(_MSC_VER) && !defined(__clang__)
#  define nob_cc_flags(cmd) nob_cmd_append(cmd, \
    "/W4", "/nologo", "/D_CRT_SECURE_NO_WARNINGS", \
    "/Iinclude")
#else
#  define nob_cc_flags(cmd) nob_cmd_append(cmd, \
    "-std=c99", \
    "-Wall", "-Wextra", \
    "-Wreturn-type", \
    "-Wno-unused", \
    "-Wno-unused-parameter", \
    "-Wno-stringop-truncation", \
    "-Wenum-compare", \
    "-Wenum-conversion", \
    "-Wtype-limits", \
    "-Wparentheses", \
    "-Wno-unknown-warning-option", \
    "-Werror", "-pedantic", "-pedantic-errors", \
    "-Iinclude")
#endif

#define NOB_IMPLEMENTATION
#define NOB_WARN_DEPRECATED
#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_UNSTRIP_PREFIX
#include "nob.h"

#undef nob_shift
#define nob_shift(xs, xs_sz) ((xs_sz) == 0 ? NULL : ((xs_sz)--, *(xs)++))

#include "platform.h"
#include "directories.h"

#include "config.h"

Nob_File_Paths common_headers = {0};
Nob_File_Paths opcodes_headers = {0};
Nob_File_Paths bfd_headers = {0};
Nob_File_Paths hx_headers = {0};
Nob_File_Paths iselgen_headers = {0};
Nob_File_Paths as_headers = {0};
Nob_File_Paths cc_headers = {0};
Nob_File_Paths sx_headers = {0};

const char* common_lib = NULL;
const char* opcodes_lib = NULL;
const char* bfd_lib = NULL;
const char* hx_lib = NULL;
const char* readobj_lib = NULL;
const char* as_lib = NULL;
const char* cc_lib = NULL;
const char* sx_lib = NULL;

typedef struct Project {
    const char* name;
    const char* source_path;
    const char** depends_on;
} Project;

static bool
read_entire_dir_ext(const char* dir, const char* ext, Nob_File_Paths* out_paths)
{
    if (!nob_file_exists(dir)) return true;

    Nob_File_Paths all = {0};
    if (!nob_read_entire_dir(dir, &all)) goto fail;

    size_t ext_length = strlen(ext);

    for (size_t i = 0; i < all.count; i++) {
        const char* child = all.items[i];
        size_t child_length = strlen(child);

        if (*child == '.' || child_length < 1 + ext_length) continue;
        if (0 != strncmp(child + child_length - ext_length, ext, ext_length)) {
            continue;
        }

        nob_da_append(out_paths, nob_temp_sprintf("%s/%s", dir, child));
    }

    nob_da_free(all);
    return true;
fail:;
    nob_da_free(all);
    return false;
}

static bool
read_entire_dir_recursive_ext(
    const char* dir,
    const char* ext,
    Nob_File_Paths* out_paths
) {
    if (!nob_file_exists(dir)) return true;

    Nob_File_Paths all = {0};
    if (!nob_read_entire_dir(dir, &all)) goto fail;

    size_t ext_length = strlen(ext);

    for (size_t i = 0; i < all.count; i++) {
        const char* child = all.items[i];
        size_t child_length = strlen(child);

        if (*child == '.') continue;

        const char* child_path = nob_temp_sprintf("%s/%s", dir, child);
        if (NOB_FILE_DIRECTORY == nob_get_file_type(child_path)) {
            if (!read_entire_dir_recursive_ext(child_path, ext, out_paths)) {
                goto fail;
            }
        } else {
            if (child_length < 1 + ext_length) continue;
            if (0 != strncmp(
                child + child_length - ext_length, ext, ext_length)
            ) {
                continue;
            }
            nob_da_append(out_paths, child_path);
        }
    }

    nob_da_free(all);
    return true;
fail:;
    nob_da_free(all);
    return false;
}


static const char*
get_file_name(const char* path)
{
#ifdef _WIN32
    const char* slash_pos = strrchr(path, '/');
    const char* backslash_pos = strrchr(path, '\\');
    if (backslash_pos > slash_pos) slash_pos = backslash_pos;
#else
    const char* slash_pos = strrchr(path, '/');
#endif
    if (slash_pos == NULL) return path;
    return slash_pos + 1;
}

static const char*
get_file_name_without_extension(const char* path)
{
    const char* file_name = get_file_name(path);
    const char* dot_pos = strrchr(file_name, '.');
    if (dot_pos == NULL) return file_name;

    size_t length = dot_pos - file_name;
    char* result = nob_temp_alloc(length + 1);
    memcpy(result, file_name, length);
    result[length] = 0;
    return result;
}

static void
cmd_append_sanitizer(Nob_Cmd* cmd) {
    if (CONFIG_SANITIZER != NULL) {
#if defined(_WIN32)
#elif defined(__clang__)
        nob_cmd_append(cmd,
            nob_temp_sprintf(
                "-fsanitize=%s,undefined,leak,integer", (char*) CONFIG_SANITIZER
            )
        );
#else
        nob_cmd_append(cmd,
            nob_temp_sprintf(
                "-fsanitize=%s,undefined,leak", (char*) CONFIG_SANITIZER
            )
        );
#endif
    }
}

static bool
build_object(
    Nob_File_Paths* out_object_paths,
    const char* project_name,
    const char* project_path,
    const char* source_name,
    Nob_File_Paths dependencies
) {
    bool result = false;

    const char* object_directory_path = nob_temp_sprintf(
        BUILD_OUTPUT_DIR BUILD_PATH_SEP "o" BUILD_PATH_SEP "%s",
        project_name
    );
    const char* object_path = nob_temp_sprintf(
        "%s" BUILD_PATH_SEP "%s.o",
        object_directory_path, source_name
    );
    nob_da_append(out_object_paths, object_path);

    if (!nob_mkdir_if_not_exists(object_directory_path)) goto fail;

    const char* source_path = nob_temp_sprintf(
        "%s" BUILD_PATH_SEP "%s.c",
        project_path, source_name
    );

    if (!nob_file_exists(source_path)) {
        nob_log(NOB_ERROR, "Could not find source '%s'.", source_path);
        goto fail;
    }

    bool needs_rebuild = false;
    needs_rebuild |= nob_needs_rebuild1(object_path, source_path);
    needs_rebuild |= nob_needs_rebuild(
        object_path, dependencies.items, dependencies.count
    );

    if (!needs_rebuild) goto success;

    Nob_Cmd cmd = {0};
    nob_cc(&cmd);
    nob_cc_output(&cmd, object_path);
#if defined(_WIN32)
    nob_cmd_append(&cmd, "/c");
#else
    nob_cmd_append(&cmd, "-c");
#endif
    nob_cc_inputs(&cmd, source_path);
    nob_cc_flags(&cmd);
    cmd_append_sanitizer(&cmd);
    if (!nob_cmd_run(&cmd, 0)) goto fail;

success:;
    result = true;
fail:;
    return result;
}

static bool
archive_library(
    const char** out_library_path,
    const char* project_name,
    Nob_File_Paths object_paths
) {
    bool result = false;

    const char* library_path = nob_temp_sprintf(
        BUILD_OUTPUT_DIR BUILD_PATH_SEP "lib" BUILD_PATH_SEP "lib%s.a",
        project_name
    );
    *out_library_path = library_path;

    if (!nob_mkdir_if_not_exists(BUILD_OUTPUT_DIR BUILD_PATH_SEP "lib")) {
        goto fail;
    }

    bool needs_rebuild = false;
    needs_rebuild |= nob_needs_rebuild(
        library_path, object_paths.items, object_paths.count);

    if (!needs_rebuild) goto success;

    Nob_Cmd cmd = {0};
#if defined(_WIN32)
    nob_cmd_append(
        &cmd, "lib", "/NOLOGO", nob_temp_sprintf("/OUT:%s", library_path)
    );
#else
    nob_cmd_append(&cmd, "ar", "rcs", library_path);
#endif
    nob_da_append_many(&cmd, object_paths.items, object_paths.count);
    if (!nob_cmd_run(&cmd, 0)) goto fail;

success:;
    result = true;
fail:;
    return result;
}

static bool
link_executable(
    const char* exe_path,
    Nob_File_Paths object_paths,
    Nob_File_Paths library_paths
) {
    bool result = false;

    bool needs_rebuild = false;
    needs_rebuild |= nob_needs_rebuild(
        exe_path, object_paths.items, object_paths.count
    );
    needs_rebuild |= nob_needs_rebuild(
        exe_path, library_paths.items, library_paths.count
    );

    if (!needs_rebuild) goto success;

    Nob_Cmd cmd = {0};
#if defined(_WIN32)
    nob_cmd_append(
        &cmd, "link", "/NOLOGO", nob_temp_sprintf("/OUT:%s", library_path)
    );
#else
    nob_cmd_append(&cmd, "cc", "-o", exe_path);
#endif
    nob_da_append_many(&cmd, object_paths.items, object_paths.count);
    nob_da_append_many(&cmd, library_paths.items, library_paths.count);
    nob_cc_flags(&cmd);
    cmd_append_sanitizer(&cmd);
    if (!nob_cmd_run(&cmd, 0)) goto fail;

success:;
    result = true;
fail:;
    return result;
}

static bool
build_common(void) {
    bool result = false;

    Nob_File_Paths common_deps = {0};

    /* Build library. */
    nob_da_append_many(
        &common_deps, common_headers.items, common_headers.count
    );
    nob_da_append_many(
        &common_deps, common_headers.items, common_headers.count
    );

    Nob_File_Paths common_sources = {0};
    if (!read_entire_dir_recursive_ext("gfu-common", ".c", &common_sources)) {
        goto fail;
    }

    Nob_File_Paths common_objects = {0};
    for (size_t i = 0; i < common_sources.count; i++) {
        const char* source_name =
            get_file_name_without_extension(common_sources.items[i]);
        if (!build_object(
            &common_objects, "common", "gfu-common", source_name, common_deps)
        ) {
            goto fail;
        }
    }
    if (!archive_library(&common_lib, "gfu-common", common_objects)) goto fail;

success:;
    result = true;
fail:;
    return result;
}

static bool
build_opcodes(void) {
    bool result = false;

    Nob_File_Paths opcodes_deps = {0};

    /* Build library. */
    nob_da_append_many(
        &opcodes_deps, opcodes_headers.items, opcodes_headers.count
    );
    nob_da_append_many(
        &opcodes_deps, opcodes_headers.items, opcodes_headers.count
    );

    Nob_File_Paths opcodes_sources = {0};
    if (!read_entire_dir_recursive_ext("gfu-opcodes", ".c", &opcodes_sources)) {
        goto fail;
    }

    Nob_File_Paths opcodes_objects = {0};
    for (size_t i = 0; i < opcodes_sources.count; i++) {
        const char* source_name =
            get_file_name_without_extension(opcodes_sources.items[i]);
        if (
            !build_object(
                &opcodes_objects, "opcodes", "gfu-opcodes",
                source_name, opcodes_deps
            )
        ) {
            goto fail;
        }
    }
    if (!archive_library(&opcodes_lib, "gfu-opcodes", opcodes_objects)) {
        goto fail;
    }

success:;
    result = true;
fail:;
    return result;
}

static bool
build_bfd(void) {
    bool result = false;

    Nob_File_Paths bfd_deps = {0};

    /* Build library. */
    nob_da_append_many(&bfd_deps, bfd_headers.items, bfd_headers.count);
    nob_da_append_many(&bfd_deps, bfd_headers.items, bfd_headers.count);

    Nob_File_Paths bfd_sources = {0};
    if (!read_entire_dir_recursive_ext("gfu-bfd", ".c", &bfd_sources)) {
        goto fail;
    }

    Nob_File_Paths bfd_objects = {0};
    for (size_t i = 0; i < bfd_sources.count; i++) {
        const char* source_name =
            get_file_name_without_extension(bfd_sources.items[i]);
        if (
            !build_object(
                &bfd_objects, "bfd", "gfu-bfd",
                source_name, bfd_deps
            )
        ) {
            goto fail;
        }
    }
    if (!archive_library(&bfd_lib, "gfu-bfd", bfd_objects)) goto fail;

success:;
    result = true;
fail:;
    return result;
}

static bool
build_hx(void) {
    bool result = false;

    Nob_File_Paths hx_deps = {0};

    /* Build library. */
    nob_da_append_many(&hx_deps, common_headers.items, common_headers.count);
    nob_da_append_many(&hx_deps, hx_headers.items, hx_headers.count);

    Nob_File_Paths hx_objects = {0};
    if (!build_object(&hx_objects, "hx", "gfu-hx", "hx", hx_deps)) {
        goto fail;
    }
    if (!archive_library(&hx_lib, "gfu-hx", hx_objects)) goto fail;

    /* Build executable. */
    hx_objects.count = 0;
    if (!build_object(&hx_objects, "hx", "gfu-hx", "main", hx_deps)) {
        goto fail;
    }

    hx_deps.count = 0;
    nob_da_append(&hx_deps, common_lib);
    nob_da_append(&hx_deps, hx_lib);
    if (!link_executable(
        BUILD_OUTPUT_DIR BUILD_PATH_SEP "hx", hx_objects, hx_deps)
    ) {
        goto fail;
    }

success:;
    result = true;
fail:;
    return result;
}

static bool
build_readobj(void) {
    bool result = false;

    Nob_File_Paths readobj_deps = {0};

    /* Build executable. */
    nob_da_append_many(
        &readobj_deps, common_headers.items, common_headers.count
    );
    nob_da_append_many(&readobj_deps, hx_headers.items, hx_headers.count);
    nob_da_append_many(&readobj_deps, bfd_headers.items, bfd_headers.count);

    Nob_File_Paths readobj_objects = {0};
    if (
        !build_object(
            &readobj_objects, "readobj", "gfu-readobj",
            "main", readobj_deps
        )
    ) {
        goto fail;
    }

    readobj_deps.count = 0;
    nob_da_append(&readobj_deps, common_lib);
    nob_da_append(&readobj_deps, hx_lib);
    nob_da_append(&readobj_deps, bfd_lib);
    if (
        !link_executable(
            BUILD_OUTPUT_DIR BUILD_PATH_SEP "readobj",
            readobj_objects, readobj_deps
        )
    ) {
        goto fail;
    }

success:;
    result = true;
fail:;
    return result;
}

static bool
build_iselgen(void) {
    bool result = false;

    Nob_File_Paths iselgen_deps = {0};

    /* Build sources. */
    if (
        nob_needs_rebuild1("gfu-iselgen/isel_source.h", "gfu-iselgen/isel.txt")
    ) {
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, \
            (BUILD_OUTPUT_DIR BUILD_PATH_SEP "hx"), "gfu-iselgen/isel.txt", \
            "-i", "-n", "isel"
        );
        if (!nob_cmd_run(&cmd, .stdout_path = "gfu-iselgen/isel_source.h")) {
            goto fail;
        }
    }

    /* Build executable. */
    nob_da_append_many(
        &iselgen_deps, common_headers.items, common_headers.count
    );
    nob_da_append_many(&iselgen_deps, hx_headers.items, hx_headers.count);
    nob_da_append_many(
        &iselgen_deps, iselgen_headers.items, iselgen_headers.count
    );

    Nob_File_Paths iselgen_objects = {0};
    if (
        !build_object(
            &iselgen_objects, "iselgen", "gfu-iselgen", "main", iselgen_deps
        )
    ) {
        goto fail;
    }

    iselgen_deps.count = 0;
    nob_da_append(&iselgen_deps, common_lib);
    if (
        !link_executable(
            BUILD_OUTPUT_DIR BUILD_PATH_SEP "iselgen",
            iselgen_objects, iselgen_deps
        )
    ) {
        goto fail;
    }

success:;
    result = true;
fail:;
    return result;
}

static bool
build_as(void) {
    bool result = false;

    Nob_File_Paths as_deps = {0};

    /* Build sources. */
    if (
        nob_needs_rebuild1(
            "gfu-as/isel_tables.c",
            BUILD_OUTPUT_DIR BUILD_PATH_SEP "iselgen"
        ) ||
        nob_needs_rebuild1(
            "include/gamefu/as/x/mnemonics.h",
            BUILD_OUTPUT_DIR BUILD_PATH_SEP "iselgen"
        )
    ) {
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, \
            (BUILD_OUTPUT_DIR BUILD_PATH_SEP "iselgen")
        );
        if (!nob_cmd_run(&cmd, 0)) goto fail;
    }

    /* Build library. */
    nob_da_append_many(&as_deps, common_headers.items, common_headers.count);
    nob_da_append_many(&as_deps, opcodes_headers.items, opcodes_headers.count);
    nob_da_append_many(&as_deps, bfd_headers.items, bfd_headers.count);
    nob_da_append_many(&as_deps, as_headers.items, as_headers.count);

    Nob_File_Paths as_sources = {0};
    if (!read_entire_dir_recursive_ext("gfu-as", ".c", &as_sources)) {
        goto fail;
    }

    Nob_File_Paths as_objects = {0};
    for (size_t i = 0; i < as_sources.count; i++) {
        const char* source_name =
            get_file_name_without_extension(as_sources.items[i]);
        if (0 == strcmp(source_name, "main")) continue;
        if (!build_object(
            &as_objects, "as", "gfu-as", source_name, as_deps)
        ) {
            goto fail;
        }
    }
    if (!archive_library(&as_lib, "gfu-as", as_objects)) goto fail;

    /* Build executable. */
    as_objects.count = 0;
    if (!build_object(&as_objects, "as", "gfu-as", "main", as_deps)) {
        goto fail;
    }

    as_deps.count = 0;
    nob_da_append(&as_deps, common_lib);
    nob_da_append(&as_deps, opcodes_lib);
    nob_da_append(&as_deps, bfd_lib);
    nob_da_append(&as_deps, as_lib);
    if (!link_executable(
        BUILD_OUTPUT_DIR BUILD_PATH_SEP "as", as_objects, as_deps)
    ) {
        goto fail;
    }

success:;
    result = true;
fail:;
    return result;
}

int
main(int argc, char** argv) {
    int result = 1;

    if (!nob_mkdir_if_not_exists(BUILD_OUTPUT_DIR BUILD_PATH_SEP "o")) {
        goto fail;
    }

    if (!nob_mkdir_if_not_exists("include/gamefu/as")) {
        goto fail;
    }

    if (!nob_mkdir_if_not_exists("include/gamefu/as/x")) {
        goto fail;
    }

    nob_da_append(&common_headers, "include/gamefu/common.h");
    if (!read_entire_dir_ext("include/gamefu/common", ".h", &common_headers)) {
        goto fail;
    }

    nob_da_append(&opcodes_headers, "include/gamefu/opcodes.h");
    if (
        !read_entire_dir_ext("include/gamefu/opcodes", ".h", &opcodes_headers)
    ) {
        goto fail;
    }

    if (!read_entire_dir_ext("include/gamefu/bfd", ".h", &bfd_headers)) {
        goto fail;
    }

    nob_da_append(&hx_headers, "include/gamefu/hx.h");
    if (!read_entire_dir_ext("include/gamefu/hx", ".h", &hx_headers)) goto fail;

    if (
        !read_entire_dir_ext("include/gamefu/iselgen", ".h", &iselgen_headers)
    ) {
        goto fail;
    }

    if (!build_common()) goto fail;
    if (!build_opcodes()) goto fail;
    if (!build_bfd()) goto fail;
    if (!build_hx()) goto fail;
    if (!build_readobj()) goto fail;
    if (!build_iselgen()) goto fail;
    if (!build_as()) goto fail;

success:;
    result = 0;
fail:;
    return result;
}
