//
// Created by 이윤상 on 2022/10/07.
//

#ifndef OPENGL_E_SHADER_ASSET_TYPE_H
#define OPENGL_E_SHADER_ASSET_TYPE_H

namespace highp::assets::shader {
    enum class e_shader_asset_type {
        constant_color,
        vertex_color,
        uniform,
        fixed_vertex_color,
        texture_mapping,
        texture_mapping_vertex_color_tinted,
        texture_mapping_lighting,
        texture_blending,
        trs,
        three_d,
        constant_light,
        light_ambient_only,
        diffuse_lighting,
        blinn_phong_lighting,
        goraud_lighting,
        directional_light,
        point_light,
        spot_light
    };
}

#endif //OPENGL_E_SHADER_ASSET_TYPE_H
