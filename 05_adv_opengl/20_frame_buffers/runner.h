//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_RUNNER_H
#define OPENGL_RUNNER_H

#include <string_view>
#include <memory>
#include <initializer_list>
#include <vector>

#include "shader/shader.h"

#ifndef __FWD_DECL_GLFW_WINDOW__
#define __FWD_DECL_GLFW_WINDOW__
/* fwd decl */ struct GLFWwindow {
};
#endif


namespace highp {
    class Runner final {
    public:
        explicit Runner(int width, int height,
                        float near, float far,
                        const char *title,
                        std::string_view box_vertex_shader_path,
                        std::string_view box_fragment_shader_path,
                        std::string_view screen_vertex_shader_path,
                        std::string_view screen_fragment_shader_path,
                        std::string_view box_diffuse_tex_path);

        virtual ~Runner();

        int init(bool enable_wireframe);

    private:
        static void on_resize_frame_buffer(GLFWwindow *window, int width, int height);

        static void process_input(GLFWwindow *window);

        static void on_receive_mouse_event_impl(GLFWwindow *window, double x, double y);

        static void on_receive_scroll_event(GLFWwindow *window, double x, double y);

    public:

    private:
        int _width;
        int _height;
        float _near;
        float _far;
        const char *_title;

        GLFWwindow *_window;

        std::unique_ptr<shared::shader> _box_shader;
        std::unique_ptr<shared::shader> _screen_shader;

        std::string_view _box_vertex_shader_path;
        std::string_view _box_fragment_shader_path;

        std::string_view _screen_vertex_shader_path;
        std::string_view _screen_fragment_shader_path;

        std::string_view _box_diffuse_tex_path;
    };
}


#endif //OPENGL_RUNNER_H
