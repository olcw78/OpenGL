#include "runner.h"

#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets::texture;

int main() {
    const auto shader_paths = get_shader_paths(e_shader_type::constant_light);
    auto runner = highp::Runner(1024, 768, "07. colors",
                                shader_paths[0],
                                shader_paths[1]);
    runner.init(false);
}