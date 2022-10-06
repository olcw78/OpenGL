//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_TEXTURE_ASSETS_H
#define OPENGL_TEXTURE_ASSETS_H

#include <string>

namespace highp::assets::texture {
    static constexpr const char *texture_dir_prefix =
            "/Users/highp/workspace/graphics/opengl/assets/texture/";

    static std::string make_source_path(const char *texture_name);

    extern std::string wall_tex_src;
    extern std::string awesomeface_tex_src;
}



#endif //OPENGL_TEXTURE_ASSETS_H
