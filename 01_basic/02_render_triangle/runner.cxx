//
// Created by 이윤상 on 2022/10/06.
//

#include "runner.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "fmt/core.h"

#include "fstream_guard/fstream_guard.h"


Runner::Runner(int width, int height, const char *title, const char *vertex_shader_src_path_abs,
               const char *fragment_shader_src_path_abs) : _width(width), _height(height), _title(title) {
    vertex_shader_str = std::move(fstream_guard()(vertex_shader_src_path_abs));
    vertex_shader_src = vertex_shader_str.c_str();

    fragment_shader_str = std::move(fstream_guard()(fragment_shader_src_path_abs));
    fragment_shader_src = fragment_shader_str.c_str();

}

Runner::~Runner() {
    glfwTerminate();
}

int Runner::init(bool enable_wireframe) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
    if (_window == nullptr) {
        fmt::print("[error] failed to create GLFW window\n");
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(_window);

    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        fmt::print("[error] failed to init GLAD\n");

        return -1;
    }

    glViewport(0, 0, _width, _height);

    glfwSetFramebufferSizeCallback(_window, Runner::on_resize_frame_buffer);

#pragma region Render Triangle

    unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
    glCompileShader(vertex_shader);

    int is_success;
    char info_log[512]{' ',};
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_success);
    if (!is_success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        fmt::print("[error][compiling vertex shader] failed: {}\n", info_log);
    }

    unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_success);
    if (!is_success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        fmt::print("[error][compiling fragment shader] failed: {}\n", info_log);
    }

    unsigned shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &is_success);
    if (!is_success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        fmt::print("[error][linking shader] failed: {}\n", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // create and bind VBO.
    constexpr const float vertices[] = {
            0.5f, 0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f, 0.5f, 0.0f   // top left
    };

    constexpr const unsigned indices[]{
            0, 1, 3,
            1, 2, 3
    };

    // create and bind VAO.
    unsigned vao;
    glGenVertexArrays(1, &vao);

    unsigned vbo;
    glGenBuffers(1, &vbo);

    unsigned ebo;
    glGenBuffers(1, &ebo);

    // bind the VAO first, then bind and set vertex buffer(s),
    // then configure vertex buffer(s).
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // linking vertex attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind vao after so other vao calls won't modify this VAO.
    glBindVertexArray(0);


#pragma endregion Render Triangle

    while (!glfwWindowShouldClose(_window)) {
        process_input(_window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (enable_wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(_window);
        glfwPollEvents(); // poll IO events.
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);

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