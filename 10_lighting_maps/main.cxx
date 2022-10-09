#include "runner.h"
#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets::texture;

int main() {
    auto const &shader_paths1 = get_shader_paths(e_shader_asset_type::texture_mapping_lighting);
    auto const &shader_paths2 = get_shader_paths(e_shader_asset_type::constant_color);

    std::string const &diffuse_tex_src = wooden_box_diffuse_tex_src;
    std::string const &specular_tex_src = wooden_box_specular_tex_src;

    auto runner = highp::Runner(1024, 768, "11. light casters",
                                shader_paths1[0],
                                shader_paths1[1],
                                shader_paths2[0],
                                shader_paths2[1],
                                diffuse_tex_src,
                                specular_tex_src
    );
    runner.init(false);
}