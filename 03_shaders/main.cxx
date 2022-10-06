#include "runner.h"

#include <fmt/format.h>
#include "assets_inc.h"

using namespace highp::assets::shader;

int main() {
    auto shader_paths = get_shader_paths(e_shader_type::fixed_vertex_color);
    auto runner = Runner(1024, 768, "03. shader_paths", shader_paths[0], shader_paths[1]);
    runner.init(false);
}