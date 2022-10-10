//
// Created by 이윤상 on 2022/10/06.
//

#include "shader.h"
#include "../fstream_guard/fstream_guard.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <fmt/core.h>

#include <algorithm>

namespace highp::shared {
    int shader::is_success = false;
    char shader::info_log[512]{' ',};

    shader::~shader() {
        glDeleteProgram(_shader_program);
    }

    void shader::validate_shader_compilation(unsigned shader_id, e_shader_type shader_type) {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader::is_success);

        if (!shader::is_success) {
            glGetShaderInfoLog(shader_id, 512, nullptr, shader::info_log);

            std::string shader_kind;
            switch (shader_type) {
                case e_shader_type::vertex:
                    shader_kind = "vertex shader";
                    break;

                case e_shader_type::fragment:
                    shader_kind = "fragment shader";
                    break;

                case e_shader_type::geometry:
                    shader_kind = "geometry shader";
                    break;

                case e_shader_type::tesselation:
                    shader_kind = "tesselation shader";
                    break;
            }

            fmt::print("[error][{} shader] compile shader failed: {}\n",
                       shader_kind,
                       shader::info_log
            );
        }
    }

    void shader::validate_shader_linkage(unsigned int program_id) {
        glGetProgramiv(program_id, GL_LINK_STATUS, &shader::is_success);

        if (!shader::is_success) {
            glGetProgramInfoLog(program_id, 512, nullptr, shader::info_log);
            fmt::print("[error][shader program] linking shader failed: {}\n", shader::info_log);
        }
    }

    unsigned shader::compile_vertex_shader(const char *src) {
        const std::string as_str{fstream_guard()(src)};
        const char *shader_src = as_str.c_str();

        const unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &shader_src, nullptr);
        glCompileShader(vertex_shader);
        shader::validate_shader_compilation(vertex_shader, e_shader_type::vertex);

        return vertex_shader;
    }

    unsigned shader::compile_fragment_shader(const char *src) {
        const std::string as_str{fstream_guard()(src)};
        const char *shader_src = as_str.c_str();

        const unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &shader_src, nullptr);
        glCompileShader(fragment_shader);
        shader::validate_shader_compilation(fragment_shader, e_shader_type::fragment);

        return fragment_shader;
    }

    void shader::compile_and_link(std::list<const unsigned> const &shaders) {
        // link shaders to the shader _shader_program.
        this->_shader_program = glCreateProgram();

        std::for_each(std::begin(shaders), std::end(shaders), [this](const unsigned int shader) {
            glAttachShader(this->_shader_program, shader);
        });

        glLinkProgram(_shader_program);
        shader::validate_shader_linkage(_shader_program);

        std::for_each(std::begin(shaders), std::end(shaders), [this](unsigned int const shader) {
            glDeleteShader(shader);
        });
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

    void shader::set_mat4(std::string_view name, glm::mat4 mat) const {
        glUniformMatrix4fv(glGetUniformLocation(_shader_program, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void shader::set_mat3(std::string_view name, glm::mat3 mat) const {
        glUniformMatrix3fv(glGetUniformLocation(_shader_program, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void shader::set_mat2(std::string_view name, glm::mat2 mat) const {
        glUniformMatrix2fv(glGetUniformLocation(_shader_program, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void shader::set_vec4(std::string_view name, glm::vec4 vec) const {
        glUniform4fv(glGetUniformLocation(_shader_program, name.data()), 1, glm::value_ptr(vec));
    }

    void shader::set_vec3(std::string_view name, glm::vec3 vec) const {
        glUniform3fv(glGetUniformLocation(_shader_program, name.data()), 1, glm::value_ptr(vec));
    }

    void shader::set_vec2(std::string_view name, glm::vec2 vec) const {
        glUniform2fv(glGetUniformLocation(_shader_program, name.data()), 1, glm::value_ptr(vec));
    }

}

