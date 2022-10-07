#include "runner.h"
#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets::texture;

int main() {
    const auto shader_paths2 = get_shader_paths(e_shader_type::constant_color);
    const auto shader_paths1 = get_shader_paths(e_shader_type::blinn_phong_lighting);

    auto runner = highp::Runner(1024, 768, "09. materials",
                                shader_paths1[0],
                                shader_paths1[1],
                                shader_paths2[0],
                                shader_paths2[1]
    );
    runner.init(false);
}