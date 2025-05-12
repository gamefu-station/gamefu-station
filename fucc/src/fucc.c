#define KOS_IMPL
#include <kos.h>

#include <choir/core.h>

int main(int argc, char** argv) {
    int result = 0;
    choir_context_ref context = nullptr;
    choir_source_ref source = nullptr;

    context = choir_context_create();

defer:
    choir_context_destroy(context);
    return result;
}
