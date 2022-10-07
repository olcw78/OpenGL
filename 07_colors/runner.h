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
        explicit Runner(int width, int height, const char *title,
                        std::initializer_list<std::string_view> shader_paths);

        virtual ~Runner();

        int init(bool enable_wireframe);

    private:
        static void on_resize_frame_buffer(GLFWwindow *window, int width, int height);

        void process_input(GLFWwindow *window);

        static void on_receive_mouse_event_impl(GLFWwindow *window, double x, double y);

        static void on_receive_scroll_event(GLFWwindow *window, double x, double y);

    public:

    private:
        int _width;
        int _height;
        const char *_title;

        GLFWwindow *_window;
        std::unique_ptr<shader> _shader;

        std::vector<std::string_view> _vertex_shader_paths;
        std::vector<std::string_view> _fragment_shader_paths;
    };
}


#endif //OPENGL_RUNNER_H
