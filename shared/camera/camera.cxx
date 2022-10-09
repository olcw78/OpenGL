//
// Created by 이윤상 on 2022/10/07.
//

#include "camera.h"
#include "time/time.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

namespace highp::shared {
#pragma region field

    float camera::sensitivity = 0.05f;
    float camera::fov = 45.0f;
    float camera::camera_speed = 3.0f;

    float camera::_last_x = 1024 / 2.0f;
    float camera::_last_y = 768 / 2.0f;
    float camera::_yaw = -90.0f;
    float camera::_pitch = 0.0f;
    bool camera::_first_mouse = true;

    glm::vec3 camera::camera_front = {0, 0, -1};
    glm::vec3 camera::camera_pos = {0, 0, 3};
    glm::vec3 camera::_camera_up = {0, 1, 0};

#pragma endregion field

    glm::mat4 camera::get_view_matrix() {
        return glm::lookAt(
                camera::camera_pos,
                camera::camera_pos + camera::camera_front,
                camera::_camera_up
        );
    }

    void camera::on_update(GLFWwindow *window) {
        const float final_camera_speed = camera::camera_speed * shared::time::get_delta_time();

        // move forward
        if (::glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_pos += final_camera_speed * camera_front;
        }

        // move backward
        if (::glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_pos -= final_camera_speed * camera_front;
        }

        // move to the left
        if (::glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_pos -= final_camera_speed * glm::normalize(glm::cross(camera_front, _camera_up));
        }

        // move to the right
        if (::glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_pos += final_camera_speed * glm::normalize(glm::cross(camera_front, _camera_up));
        }
    }

    void camera::on_update_mouse(GLFWwindow *window, double x, double y) {
        if (_first_mouse) {
            _last_x = static_cast<float>(x);
            _last_y = static_cast<float>(y);
            _first_mouse = false;
        }

        const float x_offset = (static_cast<float>(x) - _last_x) * sensitivity;
        const float y_offset = (_last_y - static_cast<float>(y)) * sensitivity;
        _last_x = x;
        _last_y = y;

        _yaw += x_offset;
        _pitch += y_offset;

        // clamp yaw and pitch
        if (_pitch > 89.0f) {
            _pitch = 89.0f;
        }

        if (_pitch < -89.0f) {
            _pitch = -89.0f;
        }

        const glm::vec3 direction{
                cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
                sin(glm::radians(_pitch)),
                sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
        };
        camera_front = glm::normalize(direction);
    }

    void camera::on_update_scroll(GLFWwindow *window, double x, double y) {
        fov -= static_cast<float>(y);
        if (fov < 1.0f) {
            fov = 1.0f;
        }

        if (fov > 75.0f) {
            fov = 75.0f;
        }
    }

#pragma region accessor

    void camera::set_sensitivity(float new_sensitivity) {
        camera::sensitivity = new_sensitivity;
    }

    float camera::get_sensitivity() {
        return camera::sensitivity;
    }

    void camera::set_fov(float new_fov) {
        camera::fov = new_fov;
    }

    float camera::get_fov() {
        return camera::fov;
    }

    void camera::set_camera_speed(float new_camera_speed) {
        camera::camera_speed = new_camera_speed;
    }

    float camera::get_camera_speed() {
        return camera::camera_speed;
    }

    glm::vec3 const &camera::get_camera_pos() {
        return camera::camera_pos;
    }

    void camera::set_camera_pos(glm::vec3 new_camera_pos) {
        camera::camera_pos = new_camera_pos;
    }

    glm::vec3 const &camera::get_camera_front() {
        return camera::camera_front;
    }

#pragma endregion accessor

}