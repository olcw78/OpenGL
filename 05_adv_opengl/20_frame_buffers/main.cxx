#include "runner.h"
#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets;

int main() {
    auto const &shader_paths1 = get_shader_paths(e_shader_asset_type::texture_mapping);
//    auto const &shader_paths_inversion = get_shader_paths(e_shader_asset_type::inversion);
//    auto const &shader_paths_grayscale = get_shader_paths(e_shader_asset_type::grayscale);
    auto const &shader_paths_sharpen_kernel = get_shader_paths(e_shader_asset_type::sharpen);

    highp::Runner(
            1024, 768,
            0.1f, 300.0f,
            "20. frame buffers",
            shader_paths1[0],
            shader_paths1[1],
            shader_paths_sharpen_kernel[0],
            shader_paths_sharpen_kernel[1],
            texture::wooden_box_diffuse_tex_src
    ).init(false);
}