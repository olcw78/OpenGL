//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <string>

class shader final {
public:
    shader() = default;

    shader(std::string_view vertex_shader_path, std::string_view fragment_shader_path);

    ~shader();

public:
    void use();

    void compile_and_link();

    void check_error_from_result(unsigned id, bool is_compile_or_link = true);

    void set_bool(std::string_view name, bool value) const;

    void set_float(std::string_view name, float value) const;

    void set_int(std::string_view name, int value) const;

private:
    unsigned _shader_program;

    std::string _vertex_shader_code;
    std::string _fragment_shader_code;
};


#endif //OPENGL_SHADER_H
