//
// Created by 이윤상 on 2022/10/06.
//

#include "shader.h"
#include "../fstream_guard/fstream_guard.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <fmt/core.h>

shader::shader(std::string_view vertex_shader_path, std::string_view fragment_shader_path)
        : _vertex_shader_code{std::move(fstream_guard()(vertex_shader_path))},
          _fragment_shader_code{std::move(fstream_guard()(fragment_shader_path))} {
    //
}


shader::~shader() {
//    glDeleteVertexArrays(1, &vao);
//    glDeleteBuffers(1, &vbo);
//    glDeleteBuffers(1, &ebo);
    glDeleteProgram(_shader_program);
}

void shader::check_error_from_result(unsigned id, bool is_compile_or_link) {
    int is_success;
    char info_log[512]{' ',};

    if (is_compile_or_link) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &is_success);
        if (!is_success) {
            glGetShaderInfoLog(id, 512, nullptr, info_log);
            fmt::print("[error][shader] compile shader failed: {}\n", info_log);
        }
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &is_success);
        if (!is_success) {
            glGetProgramInfoLog(id, 512, nullptr, info_log);
            fmt::print("[error][_shader_program] linking shader failed: {}\n", info_log);
        }
    }
}

void shader::compile_and_link() {

    // compile vertex shader.
    const char *vertex_src = _vertex_shader_code.c_str();
    unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_src, nullptr);
    glCompileShader(vertex_shader);
    check_error_from_result(vertex_shader);

    // compile fragment shader.
    const char *fragment_src = _fragment_shader_code.c_str();
    unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_src, nullptr);
    glCompileShader(fragment_shader);
    check_error_from_result(fragment_shader);

    // link shaders to the shader _shader_program.
    this->_shader_program = glCreateProgram();
    glAttachShader(_shader_program, vertex_shader);
    glAttachShader(_shader_program, fragment_shader);
    glLinkProgram(_shader_program);
    check_error_from_result(_shader_program, false);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void shader::use() {
    glUseProgram(this->_shader_program);
}

void shader::set_bool(std::string_view name, bool value) const {
    glUniform1i(glGetUniformLocation(_shader_program, name.data()), static_cast<int>(value));
}

void shader::set_float(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(_shader_program, name.data()), value);
}

void shader::set_int(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(_shader_program, name.data()), value);
}

void shader::set_mat4(std::string_view name, glm::mat4 const& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(_shader_program, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
}



