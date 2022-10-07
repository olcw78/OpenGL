//
// Created by 이윤상 on 2022/10/07.
//

#ifndef OPENGL_TIME_H
#define OPENGL_TIME_H

#include <functional>

namespace highp::shared {
    class time final {
    public:
        time() = delete;

        ~time() = delete;

    public:
        static void tick(std::function<double()> glfw_get_time_fn);

    public:
        static float get_delta_time();

    private:
        static float delta_time;
        static float last_frame;
    };
}


#endif //OPENGL_TIME_H
