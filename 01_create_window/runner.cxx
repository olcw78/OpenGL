//
// Created by 이윤상 on 2022/10/06.
//

#include "runner.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>


int Runner::init(int width, int height, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto *pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (pWindow == nullptr) {
        fmt::print("[error] failed to create GLFW window\n");
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        fmt::print("[error] failed to init GLAD\n");

        return -1;
    }

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(pWindow, Runner::on_resize_frame_buffer);

    while (!glfwWindowShouldClose(pWindow)) {
        process_input(pWindow);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void Runner::on_resize_frame_buffer(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Runner::process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
