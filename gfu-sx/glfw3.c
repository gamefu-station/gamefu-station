#include "prologue.h"
#include "platform.h"
#include "vm.h"

#include <errno.h>
#include <stdio.h>

#define GLAD_GLES2_IMPLEMENTATION
#define GLAD_GLES2_USE_SYSTEM_EGL
#include "gles2.h"
#undef GLAD_GLES2_IMPLEMENTATION
#undef GLAD_GLES2_USE_SYSTEM_EGL

#include <GLFW/glfw3.h>

struct gfusx_platform {
    GLFWwindow* window;
};

#define GL(...)                                     \
    do {                                            \
        (__VA_ARGS__);                              \
        if (!check_gl_errors(__FILE__, __LINE__)) { \
            return_defer(false);                    \
        }                                           \
    } while (0)

static const char* gl_error_names[] = {
    [GL_NO_ERROR] = "No Error",
    [GL_INVALID_ENUM] = "Invalid Enum",
    [GL_INVALID_VALUE] = "Invalid Value",
    [GL_INVALID_OPERATION] = "Invalid Operation",
    [GL_INVALID_FRAMEBUFFER_OPERATION] = "Invalid Framebuffer Operation",
    [GL_OUT_OF_MEMORY] = "Out Of Memory",
};

static bool check_gl_errors(const char* file, int line) {
    bool has_errored = false;

    GLenum err;
    while(err = glGetError(), err != GL_NO_ERROR) {
        has_errored = true;
        fprintf(stderr, "OpenGL Error: %s:%d: %s\n", file, line, gl_error_names[err]);
    }

    return !has_errored;
}

static bool check_required_glfw_version(gfusx_t* vm) {
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    if (vm->verbose) fprintf(stderr, "Running against GLFW %i.%i.%i\n", major, minor, rev);

    if (major != 3 && minor < 4) {
        fprintf(stderr, "Expecting GLFW 3, at least version 3.4\n");
        return false;
    }

    return true;
}

bool gfusx_platform_init(gfusx_t* vm) {
    bool result = true;

    vm->platform = calloc(1, sizeof *vm->platform);

    if (!glfwInit()) {
        const char* description = NULL;
        glfwGetError(&description);
        fprintf(stderr, "Failed to initialize GLFW: %s\n", description);
        return_defer(false);
    }

    if (!check_required_glfw_version(vm)) {
        return_defer(false);
    }

    if (!vm->headless) {
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        GLFWwindow* window = vm->platform->window = glfwCreateWindow(640, 480, "GameFU Station", NULL, NULL);
        if (window == NULL) {
            const char* description = NULL;
            glfwGetError(&description);
            fprintf(stderr, "Failed to create window with GLFW: %s\n", description);
            return_defer(false);
        }

        glfwMakeContextCurrent(window);
        if (!gladLoadGLES2(glfwGetProcAddress)) {
            fprintf(stderr, "Failed to initialize OpenGL ES 2.0\n");
            return_defer(false);
        }
    }

defer:;
    return result;
}

void gfusx_platform_deinit(gfusx_t* vm) {
    if (!vm->headless) {
        if (vm->platform->window != nullptr) {
            glfwDestroyWindow(vm->platform->window);
        }

        glfwTerminate();
    }

    free(vm->platform);
    vm->platform = nullptr;
}

void gfusx_platform_main_loop(gfusx_t* vm) {
    GLFWwindow* window = vm->platform->window;

    int swap_interval = 1;
    glfwSwapInterval(swap_interval);

    double begin_time = glfwGetTime();
    double last_frame_time = begin_time;

    gfu_ulong_t cycle_overflow = 0;

    while (!vm->halt) {
        glfwPollEvents();

        if (!vm->headless) {
            if (
                glfwWindowShouldClose(window) ||
                glfwGetKey(window, GLFW_KEY_ESCAPE)
            ) {
                break;
            }

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            glViewport(0, 0, width, height);
            glClearColor(255, 0, 255, 255);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        double frame_time = glfwGetTime();
        double elapsed_time = frame_time - last_frame_time;
        last_frame_time = frame_time;

        gfu_ulong_t desired_cycle_count = (gfu_ulong_t)(elapsed_time * GFUSX_FU6109_CLOCK_SPEED);
        gfu_ulong_t target_cycles = vm->cpu.cycle + desired_cycle_count;

        while (!vm->halt && vm->cpu.cycle < target_cycles) {
            gfu_ulong_t target_block_cycles = vm->cpu.cycle + desired_cycle_count / 10;
            while (!vm->halt && vm->cpu.cycle < target_block_cycles) {
                gfusx_exec_block(vm);
            }

            gfusx_platform_sleep(vm, 0);
        }

        if (vm->cpu.cycle > target_cycles) {
            cycle_overflow = vm->cpu.cycle - target_cycles;
        } else cycle_overflow = 0;

        if (!vm->headless) {
            glfwSwapBuffers(window);
        }
    }
}

#undef GL
