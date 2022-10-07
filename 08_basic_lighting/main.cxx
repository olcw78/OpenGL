#include "runner.h"

#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets::texture;

int main() {
//    const auto shader_paths1 = get_shader_paths(e_shader_type::diffuse_light);
    const auto shader_path2 = get_shader_paths(e_shader_type::constant_color)[1];
    const auto shader_paths3 = get_shader_paths(e_shader_type::blinn_phong_specular_light);

    auto runner = highp::Runner(1024, 768, "08. basic lighting",
                                shader_paths3[0],
                                shader_paths3[1],
                                shader_paths3[0],
                                shader_path2
    );
    runner.init(false);
}