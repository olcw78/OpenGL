#include "runner.h"

#include "assets_inc.h"

using namespace highp::assets::shader;
using namespace highp::assets::texture;

int main() {
    auto shader_paths = get_shader_paths(e_shader_type::three_d);
    auto runner = Runner(1024, 768, "05. transformations",
                         shader_paths[0],
                         shader_paths[1],
                         wall_tex_src,
                         awesomeface_tex_src);
    runner.init(false);
}