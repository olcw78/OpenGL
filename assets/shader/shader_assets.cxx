//
// Created by 이윤상 on 2022/10/07.
//

#include "shader_assets.h"

#include <fmt/core.h>
#include <fmt/format.h>

namespace highp::assets::shader {
    std::string make_source_path(const char *dir_name, const char *shader_name) {
        return fmt::format(FMT_STRING("{}{}/{}"), shader_dir_prefix, dir_name, shader_name);
    }

    shader_paths get_shader_paths(e_shader_type shader_type) {
        switch (shader_type) {
            case e_shader_type::constant_color:
                return {
                        make_source_path("constant color", "main.vert"),
                        make_source_path("constant color", "main.frag")
                };

            case e_shader_type::vertex_color:
                return {
                        make_source_path("vertex color", "main.vert"),
                        make_source_path("vertex color", "main.frag")
                };

            case e_shader_type::uniform:
                return {
                        make_source_path("uniform", "main.vert"),
                        make_source_path("uniform", "main.frag")
                };

            case e_shader_type::fixed_vertex_color:
                return {
                        make_source_path("fixed vertex color", "main.vert"),
                        make_source_path("fixed vertex color", "main.frag")
                };

            case e_shader_type::texture_mapping:
                return {
                        make_source_path("texture mapping", "main.vert"),
                        make_source_path("texture mapping", "main.frag")
                };

            case e_shader_type::texture_mapping_vertex_color_tinted:
                return {
                        make_source_path("texture mapping vertex color tinted", "main.vert"),
                        make_source_path("texture mapping vertex color tinted", "main.frag")
                };

            case e_shader_type::texture_blending:
                return {
                        make_source_path("texture blending", "main.vert"),
                        make_source_path("texture blending", "main.frag")
                };

            case e_shader_type::trs:
                return {
                        make_source_path("trs", "main.vert"),
                        make_source_path("trs", "main.frag")
                };

            case e_shader_type::three_d:
                return {
                        make_source_path("3d", "main.vert"),
                        make_source_path("3d", "main.frag")
                };

            case e_shader_type::constant_light:
                return {
                        make_source_path("constant light", "main.vert"),
                        make_source_path("constant light", "main.frag")
                };

            case e_shader_type::light_ambient_only:
                return {
                        make_source_path("ambient lighting", "main.vert"),
                        make_source_path("ambient lighting", "main.frag")
                };

            case e_shader_type::diffuse_light:
                return {
                        make_source_path("diffuse lighting", "main.vert"),
                        make_source_path("diffuse lighting", "main.frag")
                };

            case e_shader_type::blinn_phong_specular_light:
                return {
                        make_source_path("blinn-phong specular lighting", "main.vert"),
                        make_source_path("blinn-phong specular lighting", "main.frag"),
                };
        }
    }
}