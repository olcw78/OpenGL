//
// Created by 이윤상 on 2022/10/07.
//

#include "time.h"

namespace highp::shared {
    float time::delta_time = 0.0f;
    float time::last_frame = 0.0f;

    void time::tick(std::function<double()> glfw_get_time_fn) {
        const float current_time = static_cast<float>(glfw_get_time_fn());
        time::delta_time = current_time - last_frame;
        time::last_frame = current_time;
    }

    float time::get_delta_time() {
        return time::delta_time;
    }
}