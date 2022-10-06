#include "runner.h"

#include <fmt/format.h>

namespace {
    constexpr const char *shader_dir_prefix =
            "/Users/highp/workspace/graphics/opengl/03_shaders/shader/";

    // constant color
//    std::string constant_color_vertex_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
//                                                               "constant_color/vert.glsl");
//    std::string constant_color_fragment_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
//                                                                 "constant_color/frag.glsl");

    // vertex color
//    std::string vertex_color_vertex_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
//                                                             "vertex_color/vert.glsl");
//    std::string vertex_color_fragment_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
//                                                               "vertex_color/frag.glsl");

    // dynamic color
//    std::string dynamic_color_vertex_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
//                                                              "dynamic_color/vert.glsl");
//    std::string dynamic_color_fragment_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
//                                                                "dynamic_color/frag.glsl");

    // vertex color2
    std::string vertex_color2_vertex_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
                                                                "vertex_color2/vert.glsl");
    std::string vertex_color2_fragment_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
                                                                  "vertex_color2/frag.glsl");
}

int main() {
    auto runner = Runner(1024, 768, "03. shaders",
                         ::vertex_color2_vertex_shader_src,
                         ::vertex_color2_fragment_shader_src);
    runner.init(false);
}