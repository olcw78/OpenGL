//
// Created by 이윤상 on 2022/10/06.
//

#include "runner.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "opengl_status_checker/opengl_status_checker.h"
#include "stb/stb_image.h"

Runner::Runner(int width, int height, const char *title,
               std::string_view vertex_shader_src_path_abs,
               std::string_view fragment_shader_src_path_abs,
               std::string_view wall_texture_path_abs,
               std::string_view awesomeface_texture_path_abs)
        : _width(width),
          _height(height),
          _title(title),
          _shader{std::make_unique<shader>(
                  vertex_shader_src_path_abs,
                  fragment_shader_src_path_abs
          )},
          _wall_texture_path{wall_texture_path_abs},
          _awesomeface_texture_path{awesomeface_texture_path_abs} {
    //
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

    opengl_status_checker::check_max_shader_attributes();

    this->_shader->compile_and_link();

    // create and bind VBO.
    constexpr const float vertices[]{
            // positions          // colors           // texture_wall coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // top left
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

    // vertex color2

    // 1. __pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // 2. __color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind vao after so other vao calls won't modify this VAO.
//    glBindVertexArray(0);

    // gen and bind texture 1.
    stbi_set_flip_vertically_on_load(true);

    unsigned texture_wall;
    glGenTextures(1, &texture_wall);
    glBindTexture(GL_TEXTURE_2D, texture_wall);

    // set wrapping params.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture_wall filtering params.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width;
    int height;
    int nr_channels;

    const char *wall_texture_file_path = _wall_texture_path.data();
    unsigned char *data_wall = stbi_load(wall_texture_file_path, &width, &height, &nr_channels, 0);

    if (data_wall != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_wall);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fmt::print("[error][gen texture_wall] fail to load texture_wall at {}\n", wall_texture_file_path);
    }

    stbi_image_free(data_wall);

    // gen and bind texture 2.
    unsigned texture_awesomeface;
    glGenTextures(1, &texture_awesomeface);
    glBindTexture(GL_TEXTURE_2D, texture_awesomeface);

    // set wrapping params.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture_wall filtering params.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const char *awesomeface_texture_file_path = _awesomeface_texture_path.data();
    unsigned char *data_awesomeface = stbi_load(awesomeface_texture_file_path, &width, &height, &nr_channels, 0);

    if (data_awesomeface != nullptr) {
        // awesome face image has transparency thus make sure using alpha channel on loading it.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_awesomeface);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fmt::print("[error][gen texture_wall] fail to load texture_wall at {}\n", awesomeface_texture_file_path);
    }

    stbi_image_free(data_awesomeface);

    // make sure using shader _shader_program.
    _shader->use();
    _shader->set_int("texture_wall", 0);
    _shader->set_int("texture_awesomeface", 1);

    while (!glfwWindowShouldClose(_window)) {
        process_input(_window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (enable_wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_wall);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_awesomeface);

        _shader->use();
        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(_window);
        glfwPollEvents(); // poll IO events.
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
//    glDeleteBuffers(1, &ebo);

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