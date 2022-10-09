//
// Created by 이윤상 on 2022/10/09.
//

#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <array>

namespace highp::shared {
    /* static class */
    class texture final {
    public:
        texture() = delete;

        ~texture() = delete;

    public:
        static void set_flip_vertically(bool need_flip_vertically) noexcept;

        static unsigned
        load_texture_2d(const char *src,
                        std::array<std::pair<unsigned, int>, 2> wrapping_param_st,
                        std::array<std::pair<unsigned, int>, 2> filtering_param_min_mag,
                        bool use_mipmap = false);
    };
}


#endif //OPENGL_TEXTURE_H
