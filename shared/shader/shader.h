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
#include "e_shader_type.h"

namespace highp::shared {
    class shader final {
    public:
        ~shader();

    public:
        static unsigned compile_vertex_shader(const char *src);

        static unsigned compile_fragment_shader(const char *src);

        static void validate_shader_compilation(unsigned shader_id, e_shader_type shader_type);

        static void validate_shader_linkage(unsigned program_id);

    public:
        void compile_and_link(std::list<const unsigned> const &shaders);

        void use();

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

    private:
        static int is_success;
        static char info_log[512];
    };
}

#endif //OPENGL_SHADER_H

