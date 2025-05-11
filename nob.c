#define GFU_NOB_IMPLEMENTATION
#include "gfu-nob.h"

bool build_obj(const char* src, const char* obj, Nob_File_Paths* include_files, Nob_File_Paths* out_obj_files) {
    bool result = true;

    Nob_Cmd cmd = {0};

    nob_da_append(out_obj_files, obj);
    if (nob_file_exists(obj)) {
        int rebuild_status = nob_needs_rebuild1(obj, src);
        if (rebuild_status < 0) nob_return_defer(false);

        if (0 == rebuild_status) {
            rebuild_status = nob_needs_rebuild(obj, include_files->items, include_files->count);
            if (rebuild_status < 0) nob_return_defer(false);
        }

        if (0 == rebuild_status) nob_return_defer(true);
    }

    nob_cc(&cmd);
    nob_cc_output(&cmd, obj);
    nob_cmd_append(&cmd, "-c");
    nob_cc_inputs(&cmd, src);
    nob_cmd_append(&cmd, "-Iinclude");
    nob_cmd_append(&cmd, "-Ithird-party/kos");
    nob_cmd_append(&cmd, "-Ithird-party/choir/include");
    nob_cc_flags(&cmd);

    if (!nob_cmd_run_sync(cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);
    return result;
}

bool link_exe(const char* exe, Nob_File_Paths* obj_files) {
    bool result = true;

    Nob_Cmd cmd = {0};

    int rebuild_status = nob_needs_rebuild(exe, obj_files->items, obj_files->count);
    if (rebuild_status < 0) nob_return_defer(false);
    if (0 == rebuild_status) nob_return_defer(true);

    gfu_nob_ld(&cmd);
    gfu_nob_ld_output(&cmd, exe);
    nob_da_append_many(&cmd, obj_files->items, obj_files->count);
    gfu_nob_ld_flags(&cmd);

    if (!nob_cmd_run_sync(cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);
    return result;
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    int result = 0;

    nob_log(NOB_INFO, "Hello, Nob!");

defer:;
    return result;
}
