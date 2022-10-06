#include "runner.h"

#include "assets_inc.h"

using namespace highp::assets::shader;

int main() {
    auto shader_paths = get_shader_paths(e_shader_type::constant_color);
    auto runner = Runner(
            1024,
            768,
            "02. render triangle",
            shader_paths[0].c_str(),
            shader_paths[1].c_str());
    runner.init(false);
}