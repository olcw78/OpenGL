#include "runner.h"

#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets::texture;

int main() {
//    const auto shader_paths1 = get_shader_paths(e_shader_type::diffuse_lighting);
    const auto shader_path2 = get_shader_paths(e_shader_type::constant_color)[1];
//    const auto shader_paths3 = get_shader_paths(e_shader_type::blinn_phong_lighting);
    const auto shader_paths4 = get_shader_paths(e_shader_type::goraud_lighting);

    auto runner = highp::Runner(1024, 768, "08. basic lighting",
                                shader_paths4[0],
                                shader_paths4[1],
                                shader_paths4[0],
                                shader_path2
    );
    runner.init(false);
}