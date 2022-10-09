//
// Created by 이윤상 on 2022/10/07.
//

#include "texture_assets.h"

#include <fmt/core.h>
#include <fmt/format.h>

namespace highp::assets::texture {
    std::string make_source_path(const char *texture_name) {
        return fmt::format(FMT_STRING("{}{}"), texture_dir_prefix, texture_name);
    }

    std::string wall_tex_src = make_source_path("wall.jpg");
    std::string awesomeface_tex_src = make_source_path("awesomeface.png");
    std::string wooden_box_diffuse_tex_src = make_source_path("wooden_box/diffuse.png");
    std::string wooden_box_specular_tex_src = make_source_path("wooden_box/specular.png");
}