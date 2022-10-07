//
// Created by 이윤상 on 2022/10/07.
//

#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include <glm/glm.hpp>

#ifndef __FWD_DECL_GLFW_WINDOW__
#define __FWD_DECL_GLFW_WINDOW__
/* fwd decl */ struct GLFWwindow {
};
#endif

namespace highp::shared {
    /* static */ class camera {
    public:
        camera() = delete;

        ~camera() = delete;

    public:
        static glm::mat4 get_view_matrix();

        static void on_update(GLFWwindow *window, float dt);

        static void on_update_mouse(GLFWwindow *window, double x, double y);

        static void on_update_scroll(GLFWwindow *window, double x, double y);

#pragma region accessor

    public:
        static float get_camera_speed();

        static void set_camera_speed(float new_camera_speed);

        static float get_fov();

        static void set_fov(float new_fov);

        static float get_sensitivity();

        static void set_sensitivity(float new_sensitivity);

#pragma endregion accessor

#pragma region field

    private:
        static float sensitivity;
        static float fov;
        static float camera_speed;

        static glm::vec3 _camera_pos;
        static glm::vec3 _camera_up;
        static glm::vec3 _camera_front;

        static bool _first_mouse;

        static float _yaw;
        static float _pitch;

        static float _last_x;
        static float _last_y;

#pragma endregion field
    };
}

#endif //OPENGL_CAMERA_H
