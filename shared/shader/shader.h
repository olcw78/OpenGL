//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <list>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class shader final {
public:
    ~shader();

public:
    void use();

    unsigned compile_vertex_shader(const char *src) const;

    unsigned compile_fragment_shader(const char *src) const;

    void compile_and_link(std::list<const unsigned> const &shaders);

    void check_error_from_result(unsigned id, bool is_compile_or_link = true) const;

public:
    // primitive
    void set_bool(std::string_view name, bool value) const;

    void set_float(std::string_view name, float value) const;

    void set_int(std::string_view name, int value) const;

    // matrix
    void set_mat4(std::string_view name, glm::mat4 mat) const;

    void set_mat3(std::string_view name, glm::mat3 mat) const;

    void set_mat2(std::string_view name, glm::mat2 mat) const;

    // vector
    void set_vec4(std::string_view name, glm::vec4 vec) const;

    void set_vec3(std::string_view name, glm::vec3 vec) const;

    void set_vec2(std::string_view name, glm::vec2 vec) const;

private:
    unsigned _shader_program;

    std::string _vertex_shader_code;
    std::string _fragment_shader_code;
};

#endif //OPENGL_SHADER_H

