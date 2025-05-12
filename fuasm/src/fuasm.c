#define KOS_IMPL
#include <kos.h>

#include <choir/core.h>
#include <gamefu/fuasm.h>

int main(int argc, char** argv) {
    int result = 0;
    choir_context_ref context = nullptr;
    choir_source_ref source = nullptr;

    context = choir_context_create();

    if (argc < 2) {
        choir_diag_issue(context, CHOIR_ERROR, "No source file provided.");
        kos_return_defer(1);
    }

    const char* source_path = argv[1];

    source = choir_source_read_from_file(context, source_path, 0);
    if (source == nullptr) {
        //choir_diag_issue(context, CHOIR_ERROR, "No source file provided.");
        kos_return_defer(1);
    }

    fuasm_lexer lexer = {
        .context = context,
        .source = source,
    };

    fuasm_token token = {0};
    while (token = fuasm_read_token(&lexer), token.kind != FUASM_TK_END_OF_FILE) {
        choir_diag_issue_source_bytes(context, CHOIR_INFO, source, token.begin, "%s", fuasm_token_kind_name_get(token.kind));
    }

defer:
    choir_context_destroy(context);
    return result;
}
