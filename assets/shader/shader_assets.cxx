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

    shader_paths get_shader_paths(e_shader_asset_type shader_asset_type) {
        switch (shader_asset_type) {
            case e_shader_asset_type::constant_color:
                return {
                        make_source_path("constant color", "main.vert"),
                        make_source_path("constant color", "main.frag")
                };

            case e_shader_asset_type::vertex_color:
                return {
                        make_source_path("vertex color", "main.vert"),
                        make_source_path("vertex color", "main.frag")
                };

            case e_shader_asset_type::uniform:
                return {
                        make_source_path("uniform", "main.vert"),
                        make_source_path("uniform", "main.frag")
                };

            case e_shader_asset_type::fixed_vertex_color:
                return {
                        make_source_path("fixed vertex color", "main.vert"),
                        make_source_path("fixed vertex color", "main.frag")
                };

            case e_shader_asset_type::texture_mapping:
                return {
                        make_source_path("texture mapping", "main.vert"),
                        make_source_path("texture mapping", "main.frag")
                };

            case e_shader_asset_type::texture_mapping_vertex_color_tinted:
                return {
                        make_source_path("texture mapping vertex color tinted", "main.vert"),
                        make_source_path("texture mapping vertex color tinted", "main.frag")
                };

            case e_shader_asset_type::texture_mapping_lighting:
                return {
                        make_source_path("texture mapping lighting", "main.vert"),
                        make_source_path("texture mapping lighting", "main.frag")
                };

            case e_shader_asset_type::texture_blending:
                return {
                        make_source_path("texture blending", "main.vert"),
                        make_source_path("texture blending", "main.frag")
                };

            case e_shader_asset_type::trs:
                return {
                        make_source_path("trs", "main.vert"),
                        make_source_path("trs", "main.frag")
                };

            case e_shader_asset_type::three_d:
                return {
                        make_source_path("3d", "main.vert"),
                        make_source_path("3d", "main.frag")
                };

            case e_shader_asset_type::constant_light:
                return {
                        make_source_path("constant light", "main.vert"),
                        make_source_path("constant light", "main.frag")
                };

            case e_shader_asset_type::single_color:
                return {
                        make_source_path("constant light", "main.vert"),
                        make_source_path("single color", "main.frag")
                };

            case e_shader_asset_type::light_ambient_only:
                return {
                        make_source_path("ambient lighting", "main.vert"),
                        make_source_path("ambient lighting", "main.frag")
                };

            case e_shader_asset_type::diffuse_lighting:
                return {
                        make_source_path("diffuse lighting", "main.vert"),
                        make_source_path("diffuse lighting", "main.frag")
                };

            case e_shader_asset_type::blinn_phong_lighting:
                return {
                        make_source_path("blinn-phong specular lighting", "main.vert"),
                        make_source_path("blinn-phong specular lighting", "main.frag"),
                };

            case e_shader_asset_type::goraud_lighting:
                return {
                        make_source_path("goraud lighting", "main.vert"),
                        make_source_path("goraud lighting", "main.frag")
                };

            case e_shader_asset_type::directional_light:
                return {
                        make_source_path("directional light", "main.vert"),
                        make_source_path("directional light", "main.frag")
                };

            case e_shader_asset_type::point_light:
                return {
                        make_source_path("point light", "main.vert"),
                        make_source_path("point light", "main.frag")
                };

            case e_shader_asset_type::spot_light:
                return {
                        make_source_path("spot light", "main.vert"),
                        make_source_path("spot light", "main.frag")
                };

            case e_shader_asset_type::multiple_lights:
                return {
                        make_source_path("multiple lights", "main.vert"),
                        make_source_path("multiple lights", "main.frag"),
                };

            case e_shader_asset_type::depth_visualizing:
                return {
                        make_source_path("depth visualizing", "main.vert"),
                        make_source_path("depth visualizing", "main.frag")
                };

            case e_shader_asset_type::alpha_cutout:
                return {
                        make_source_path("alpha cutout", "main.vert"),
                        make_source_path("alpha cutout", "main.frag")
                };

            case e_shader_asset_type::screen_buffer:
                return {
                        make_source_path("screen buffer", "main.vert"),
                        make_source_path("screen buffer", "main.frag")
                };

            case e_shader_asset_type::inversion:
                return {
                        make_source_path("screen buffer", "main.vert"),
                        make_source_path("post processing/inversion", "main.frag")
                };

            case e_shader_asset_type::grayscale:
                return {
                        make_source_path("screen buffer", "main.vert"),
                        make_source_path("post processing/grayscale", "main.frag")
                };

            case e_shader_asset_type::sharpen:
                return {
                        make_source_path("screen buffer", "main.vert"),
                        make_source_path("post processing/kernel", "sharpen.frag")
                };
        }
    }
}