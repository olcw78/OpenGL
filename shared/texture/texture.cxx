//
// Created by 이윤상 on 2022/10/09.
//

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <fmt/core.h>

#include "texture.h"
#include "stb/stb_image.h"

namespace highp::shared {
    void texture::set_flip_vertically(bool need_flip_vertically) noexcept {
        stbi_set_flip_vertically_on_load(need_flip_vertically);
    }

    unsigned texture::load_texture_2d(const char *src,
                                      std::array<std::pair<unsigned int, int>, 2> wrapping_param_st,
                                      std::array<std::pair<unsigned int, int>, 2> filtering_param_min_mag,
                                      bool use_mipmap) {
        int width = 0;
        int height = 0;
        int nr_channels = 0;

        // gen tex.
        unsigned tex;
        glGenTextures(1, &tex);

        // load actual texture.
        unsigned char *data = stbi_load(src, &width, &height, &nr_channels, 0);
        if (data == nullptr) {
            stbi_image_free(data);
            fmt::print("[error][gen texture] fail to load a texture: at {}\n", src);

            return 0;
        }

        GLenum format;
        switch (nr_channels) {
            case 0:
                format = GL_RED;
                break;

            case 1:
                format = GL_GREEN;
                break;

            case 2:
                format = GL_BLUE;
                break;

            case 3:
                format = GL_RGB;
                break;

            case 4:
                format = GL_RGBA;
                break;

            default:
                break;
        }

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        if (use_mipmap) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        // set wrapping params.
        const auto [wrapping_s, wrapping_t] = wrapping_param_st;
        glTexParameteri(GL_TEXTURE_2D, std::get<0>(wrapping_s), std::get<1>(wrapping_s));
        glTexParameteri(GL_TEXTURE_2D, std::get<0>(wrapping_t), std::get<1>(wrapping_t));

        // set filtering params.
        const auto [filtering_min, filtering_param_mag] = filtering_param_min_mag;
        glTexParameteri(GL_TEXTURE_2D, std::get<0>(filtering_min), std::get<1>(filtering_min));
        glTexParameteri(GL_TEXTURE_2D, std::get<0>(filtering_param_mag), std::get<1>(filtering_param_mag));

//        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        return tex;
    }

}