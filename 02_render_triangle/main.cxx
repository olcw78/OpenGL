#include "runner.h"

#include <fmt/format.h>

namespace {
    constexpr const char *shader_dir_prefix =
            "/Users/highp/workspace/graphics/opengl/02_render_triangle/shader/";

    std::string constant_color_vertex_shader_src = fmt::format("{}{}", shader_dir_prefix, "vert.glsl");
    std::string constant_color_fragment_shader_src = fmt::format("{}{}", shader_dir_prefix, "frag.glsl");
}

int main() {
    auto runner = Runner(1024, 768, "02. render triangle",
                         constant_color_vertex_shader_src.c_str(),
                         constant_color_fragment_shader_src.c_str());
    runner.init(false);
}