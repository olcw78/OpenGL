#include "runner.h"

#include <fmt/format.h>

namespace {
    constexpr const char *shader_dir_prefix =
            "/Users/highp/workspace/graphics/opengl/04_textures/shader/";
    constexpr const char *texture_dir_prefix =
            "/Users/highp/workspace/graphics/opengl/shared/texture/";

    // brick wall texture
    std::string wall_tex_src = fmt::format(FMT_STRING("{}{}"), texture_dir_prefix, "wall.jpg");
    std::string awesomeface_tex_src = fmt::format(FMT_STRING("{}{}"), texture_dir_prefix, "awesomeface.png");

    // texture mapping
    std::string texture_mapping_vertex_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
                                                                "texture mapping/vert.glsl");
    std::string texture_mapping_fragment_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
                                                                  "texture mapping/frag.glsl");

    // texture mapping vertex color tinted
    std::string texture_mapping_vertex_color_tinted_vertex_shader_src = fmt::format(FMT_STRING("{}{}"),
                                                                                    shader_dir_prefix,
                                                                                    "texture mapping vertex color tinted/vert.glsl");
    std::string texture_mapping_vertex_color_tinted_fragment_shader_src = fmt::format(FMT_STRING("{}{}"),
                                                                                      shader_dir_prefix,
                                                                                      "texture mapping vertex color tinted/frag.glsl");

    // texture blending
    std::string texture_blending_vertex_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
                                                                 "texture blending/vert.glsl");
    std::string texture_blending_fragment_shader_src = fmt::format(FMT_STRING("{}{}"), shader_dir_prefix,
                                                                   "texture blending/frag.glsl");
}

int main() {
    auto runner = Runner(1024, 768, "04. textures",
                         ::texture_blending_vertex_shader_src,
                         ::texture_blending_fragment_shader_src,
                         ::wall_tex_src,
                         ::awesomeface_tex_src);
    runner.init(false);
}