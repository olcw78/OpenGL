#include "runner.h"
#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets;

int main() {
    auto const &shader_paths1 = get_shader_paths(e_shader_asset_type::texture_mapping);
    auto const &shader_paths2 = get_shader_paths(e_shader_asset_type::texture_mapping);

    highp::Runner(
            1920, 1080,
            0.1f, 300.0f,
            "18. alpha blending",
            shader_paths1[0],
            shader_paths1[1],
            shader_paths2[0],
            shader_paths2[1],
            texture::wooden_box_diffuse_tex_src,
            texture::blending_transparent_window_tex_src
    ).init(false);
}