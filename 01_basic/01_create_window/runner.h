//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_RUNNER_H
#define OPENGL_RUNNER_H


class Runner {
public:
    static int init(int width, int height, const char *title);

private:
    Runner() = delete;

    ~Runner() = delete;

    static void on_resize_frame_buffer(struct GLFWwindow *window, int width, int height);

    static void process_input(Runner::GLFWwindow *window);
};


#endif //OPENGL_RUNNER_H
