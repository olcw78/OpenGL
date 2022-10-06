//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_RUNNER_H
#define OPENGL_RUNNER_H

#include <string_view>
#include <memory>
#include "shader/shader.h"

class Runner final {
public:
    explicit Runner(int width, int height, const char *title,
                    std::string_view vertex_shader_src_path_abs,
                    std::string_view fragment_shader_src_path_abs);

    virtual ~Runner();

    int init(bool enable_wireframe);

private:
    static void on_resize_frame_buffer(struct GLFWwindow *window, int width, int height);

    static void process_input(struct GLFWwindow *window);

public:

private:
    int _width;
    int _height;
    const char *_title;

    struct GLFWwindow *_window;
    std::unique_ptr<shader> _shader;
};


#endif //OPENGL_RUNNER_H
