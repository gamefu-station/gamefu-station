#define GFU_NOB_IMPLEMENTATION
#include "gfu-nob.h"

bool build_obj(const char* src, const char* obj, Nob_File_Paths* include_paths, Nob_File_Paths* out_obj_files) {
    nob_log(NOB_INFO, ">  Compiling object '%s'.", obj);

    bool result = true;

    Nob_File_Paths include_files = {0};
    Nob_Cmd cmd = {0};

    for (size_t i = 0; i < include_paths->count; i++) {
        const char* include_path = include_paths->items[i];
        gfu_nob_try(false, gfu_nob_read_entire_dir_recursive_ext(include_path, ".h", &include_files));
    }

    nob_da_append(out_obj_files, obj);
    if (nob_file_exists(obj)) {
        int rebuild_status = nob_needs_rebuild1(obj, src);
        if (rebuild_status < 0) nob_return_defer(false);

        if (0 == rebuild_status) {
            rebuild_status = nob_needs_rebuild(obj, include_files.items, include_files.count);
            if (rebuild_status < 0) nob_return_defer(false);
        }

        if (0 == rebuild_status) nob_return_defer(true);
    }

    nob_cc(&cmd);
    nob_cc_output(&cmd, obj);
    nob_cmd_append(&cmd, "-c");
    nob_cc_inputs(&cmd, src);
    for (size_t i = 0; i < include_paths->count; i++) {
        nob_cmd_append(&cmd, nob_temp_sprintf("-I%s", include_paths->items[i]));
    }
    nob_cc_flags(&cmd);

    if (!nob_cmd_run_sync(cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);
    nob_da_free(include_files);
    return result;
}

bool link_exe(const char* exe, Nob_File_Paths* obj_files, Nob_File_Paths* lib_files) {
    nob_log(NOB_INFO, ">  Linking executable '%s'.", exe);

    bool result = true;

    Nob_Cmd cmd = {0};

    int rebuild_status = nob_needs_rebuild(exe, obj_files->items, obj_files->count);
    if (rebuild_status < 0) nob_return_defer(false);
    if (0 == rebuild_status) nob_return_defer(true);

    gfu_nob_ld(&cmd);
    gfu_nob_ld_output(&cmd, exe);
    nob_da_append_many(&cmd, obj_files->items, obj_files->count);
    nob_da_append_many(&cmd, lib_files->items, lib_files->count);
    gfu_nob_ld_flags(&cmd);

    if (!nob_cmd_run_sync(cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);
    return result;
}

typedef enum build_kind {
    BUILD_EXE,
    BUILD_STATIC,
    BUILD_DYNAMIC,
} build_kind;

typedef struct project {
    const char* name;
    build_kind kind;
    Nob_File_Paths source_paths;
    Nob_File_Paths include_paths;
    Nob_File_Paths libraries;
} project;

static project choir = {0};
static project fuld = {0};
static project fuasm = {0};
static project fucc = {0};
static project gfusx = {0};

static bool build_project(project p) {
    nob_log(NOB_INFO, ">> Building project '%s'.", p.name);

    if (p.source_paths.count == 0) {
        nob_log(NOB_INFO, "   Nothing to build");
        return true;
    }

    bool result = true;

    Nob_File_Paths obj_files = {0};

    for (size_t i = 0; i < p.source_paths.count; i++) {
        const char* source_path = p.source_paths.items[i];
        Nob_String_View source_name = gfu_nob_sv_file_name(nob_sv_from_cstr(source_path));
        gfu_nob_try(false, build_obj(source_path, nob_temp_sprintf(".build/"SV_Fmt".o", SV_Arg(source_name)), &p.include_paths, &obj_files));
    }

    const char* outfile;
    if (p.kind == BUILD_EXE) {
        outfile = gfu_nob_exe(nob_temp_sprintf(".build/%s", p.name));
        gfu_nob_try(false, link_exe(outfile, &obj_files, &p.libraries));
    } else if (p.kind == BUILD_STATIC) {
        outfile = gfu_nob_lib_a(nob_temp_sprintf(".build/%s", p.name));
        nob_log(NOB_ERROR, "Can't build a static library yet.");
        nob_return_defer(false);
    } else {
        outfile = gfu_nob_lib_so(nob_temp_sprintf(".build/%s", p.name));
        nob_log(NOB_ERROR, "Can't build a dynamic library yet.");
        nob_return_defer(false);
    }

defer:;
    nob_da_free(obj_files);

    if (result) {
        nob_log(NOB_INFO, "   Success!");
    }

    return result;
}

static bool build_choir() {
    nob_log(NOB_INFO, ">> Building project 'choir'.");
    if (!gfu_nob_cd("./third-party/choir")) return false;
    bool result = true;

    Nob_Cmd cmd = {0};
    nob_cc(&cmd);
    nob_cc_output(&cmd, gfu_nob_exe("nob"));
    nob_cc_inputs(&cmd, "nob.c");

    if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_return_defer(false);
    }

    cmd.directory = gfu_nob_get_cwd();
#ifdef _WIN32
    nob_cmd_append(&cmd, nob_temp_sprintf("%s\\nob.exe", cmd.directory));
#else
    nob_cmd_append(&cmd, nob_temp_sprintf("%s/nob", cmd.directory));
#endif

    if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);

    if (!gfu_nob_cd("../..")) return false;
    if (result) {
        nob_log(NOB_INFO, "   Success!");
    }

    return result;
}

static bool build_fuld() {
    bool result = true;
    gfu_nob_try(false, build_project(fuld));
defer:;
    return result;
}

static bool build_fuasm() {
    bool result = true;
    gfu_nob_try(false, build_project(fuasm));
defer:;
    return result;
}

static bool build_fucc() {
    bool result = true;
    gfu_nob_try(false, build_project(fucc));
defer:;
    return result;
}

static bool build_gfusx() {
    bool result = true;
    gfu_nob_try(false, build_project(gfusx));
defer:;
    return result;
}

static bool clean(bool commit) {
    bool result = true;

    Nob_File_Paths artifacts = {0};
    gfu_nob_try(false, gfu_nob_read_entire_dir_recursive(".build", &artifacts));

    for (size_t i = 0; i < artifacts.count; i++) {
        const char* artifact_path = artifacts.items[i];
        if (commit) {
            nob_log(NOB_INFO, "Removing '%s'.", artifact_path);
            remove(artifact_path);
        } else {
            nob_log(NOB_INFO, "Would remove '%s'.", artifact_path);
        }
    }

    if (commit) {
        nob_log(NOB_INFO, "Removing '.build'.");
        remove(".build/");
    } else {
        nob_log(NOB_INFO, "Would remove '.build'.");
    }

    nob_log(NOB_INFO, "Use 'nob clean commit' to commit to removing these files.");

defer:;
    return result;
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    // handle clean first, separately
    if (argc >= 2 && 0 == strcmp("clean", argv[1])) {
        bool commit = argc >= 3 && 0 == strcmp("commit", argv[2]);
        return clean(commit) ? 0 : 1;
    }

    int result = 0;

    fuld.name = "fuld";
    fuld.kind = BUILD_EXE;
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fuld/lib", ".c", &fuld.source_paths));
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fuld/src", ".c", &fuld.source_paths));
    nob_da_append(&fuld.include_paths, "include");
    nob_da_append(&fuld.include_paths, "third-party/kos");
    nob_da_append(&fuld.include_paths, "third-party/elf");
    nob_da_append(&fuld.include_paths, "third-party/choir/include");
    nob_da_append(&fuld.include_paths, "fuld/include");
    nob_da_append(&fuld.libraries, gfu_nob_lib_a("third-party/choir/.build/libchoir"));

    fuasm.name = "fuasm";
    fuasm.kind = BUILD_EXE;
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fuasm/lib", ".c", &fuasm.source_paths));
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fuasm/src", ".c", &fuasm.source_paths));
    nob_da_append(&fuasm.include_paths, "include");
    nob_da_append(&fuasm.include_paths, "third-party/kos");
    nob_da_append(&fuasm.include_paths, "third-party/elf");
    nob_da_append(&fuasm.include_paths, "third-party/choir/include");
    nob_da_append(&fuasm.include_paths, "fuasm/include");
    nob_da_append(&fuasm.libraries, gfu_nob_lib_a("third-party/choir/.build/libchoir"));

    fucc.name = "fucc";
    fucc.kind = BUILD_EXE;
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fuld/lib", ".c", &fucc.source_paths));
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fuasm/lib", ".c", &fucc.source_paths));
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fucc/lib", ".c", &fucc.source_paths));
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("fucc/src", ".c", &fucc.source_paths));
    nob_da_append(&fucc.include_paths, "include");
    nob_da_append(&fucc.include_paths, "third-party/kos");
    nob_da_append(&fucc.include_paths, "third-party/elf");
    nob_da_append(&fucc.include_paths, "third-party/choir/include");
    nob_da_append(&fucc.include_paths, "fuld/include");
    nob_da_append(&fucc.include_paths, "fuasm/include");
    nob_da_append(&fucc.include_paths, "fucc/include");
    nob_da_append(&fucc.libraries, gfu_nob_lib_a("third-party/choir/.build/libchoir"));

    gfusx.name = "gfusx";
    gfusx.kind = BUILD_EXE;
    gfu_nob_try(1, gfu_nob_read_entire_dir_recursive_ext("gfusx/src", ".c", &gfusx.source_paths));
    nob_da_append(&gfusx.include_paths, "include");
    nob_da_append(&gfusx.include_paths, "gfusx/include");
    nob_da_append(&gfusx.include_paths, "third-party/kos");
    nob_da_append(&gfusx.include_paths, "third-party/elf");
    nob_da_append(&gfusx.include_paths, "gfusx/include");

    gfu_nob_try(1, nob_mkdir_if_not_exists(".build"));

    if (argc >= 2) {
        const char* cmd = argv[1];
        if (0 == strcmp("gfusx", cmd)) {
            return build_gfusx() ? 0 : 1;
        }
    }

    gfu_nob_try(1, build_choir());
    gfu_nob_try(1, build_fuld());
    gfu_nob_try(1, build_fuasm());
    gfu_nob_try(1, build_fucc());
    gfu_nob_try(1, build_gfusx());

defer:;
    return result;
}
