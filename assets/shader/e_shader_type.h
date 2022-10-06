//
// Created by 이윤상 on 2022/10/07.
//

#ifndef OPENGL_E_SHADER_TYPE_H
#define OPENGL_E_SHADER_TYPE_H

namespace highp::assets::shader {
    enum class e_shader_type {
        constant_color,
        vertex_color,
        uniform,
        fixed_vertex_color,
        texture_mapping,
        texture_mapping_vertex_color_tinted,
        texture_blending
    };
}

#endif //OPENGL_E_SHADER_TYPE_H
