//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_SHADER_ASSETS_H
#define OPENGL_SHADER_ASSETS_H

#include <vector>
#include <string>
#include <fmt/core.h>
#include <fmt/format.h>

#include "e_shader_type.h"

namespace highp::assets::shader {
    using shader_paths = std::vector<std::string>;

    static constexpr const char *shader_dir_prefix =
            "/Users/highp/workspace/graphics/opengl/assets/shader/";

    static std::string make_source_path(const char *dir_name, const char *shader_name);

    shader_paths get_shader_paths(e_shader_type shader_type);
}

#endif //OPENGL_SHADER_ASSETS_H
