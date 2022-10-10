#include "runner.h"
#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets::texture;

int main() {
    auto const &shader_paths1 = get_shader_paths(e_shader_asset_type::texture_mapping);
    auto const &shader_paths2 = get_shader_paths(e_shader_asset_type::single_color);

    std::string const &diffuse_tex_src = wooden_box_diffuse_tex_src;

    auto runner = highp::Runner(
            1920, 1080,
            0.1f, 300.0f,
            "17. stencil testing",
            shader_paths1[0],
            shader_paths1[1],
            shader_paths2[0],
            shader_paths2[1],
            diffuse_tex_src
    );
    runner.init(false);
}