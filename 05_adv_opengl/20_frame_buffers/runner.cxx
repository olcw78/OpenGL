//
// Created by 이윤상 on 2022/10/06.
//

#include "fmt/core.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>


#include "runner.h"

#include "opengl_status_checker/opengl_status_checker.h"
#include "stb/stb_image.h"
#include "camera/camera.h"
#include "time/time.h"
#include "texture/texture.h"

namespace highp {
    constexpr static const float cube_vertices[]{
            // positions          // texture Coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    constexpr const float plane_vertices[]{
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
            5.0f, -0.5f, -5.0f, 2.0f, 2.0f
    };

    constexpr const float quad_vertices[]{
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };

    const std::vector<glm::vec3> grass_positions{
            {-1.5f, 0.0f, -0.48f},
            {1.5f,  0.0f, 0.51f},
            {0.0f,  0.0f, 0.7f},
            {-0.3f, 0.0f, -2.3f},
            {0.5f,  0.0f, -0.6}
    };

    Runner::Runner(int width, int height,
                   float near, float far,
                   const char *title,
                   std::string_view box_vertex_shader_path,
                   std::string_view box_fragment_shader_path,
                   std::string_view screen_vertex_shader_path,
                   std::string_view screen_fragment_shader_path,
                   std::string_view box_diffuse_tex_path)
            : _width(width),
              _height(height),
              _near{near},
              _far{far},
              _title(title),
              _window{nullptr},
              _box_shader{std::make_unique<shared::shader>()},
              _box_vertex_shader_path{box_vertex_shader_path},
              _box_fragment_shader_path{box_fragment_shader_path},
              _screen_shader{std::make_unique<shared::shader>()},
              _screen_vertex_shader_path{screen_vertex_shader_path},
              _screen_fragment_shader_path{screen_fragment_shader_path},
              _box_diffuse_tex_path{box_diffuse_tex_path} {
        //
    }

    Runner::~Runner() {
        glfwTerminate();
    }

    int Runner::init(bool enable_wireframe) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        _window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
        if (_window == nullptr) {
            fmt::print("[error] failed to create GLFW window\n");
            glfwTerminate();

            return -1;
        }

        glfwMakeContextCurrent(_window);

        if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
            fmt::print("[error] failed to init GLAD\n");

            return -1;
        }

        glfwSetFramebufferSizeCallback(_window, Runner::on_resize_frame_buffer);

        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(_window, Runner::on_receive_mouse_event_impl);

        glfwSetScrollCallback(_window, Runner::on_receive_scroll_event);

        glViewport(0, 0, _width, _height);

        // compile and link shaders.
        opengl_status_checker::check_max_shader_attributes();

        _box_shader->compile_and_link(
                {
                        shared::shader::compile_vertex_shader(_box_vertex_shader_path.data()),
                        shared::shader::compile_fragment_shader(_box_fragment_shader_path.data())
                }
        );

        _screen_shader->compile_and_link(
                {
                        shared::shader::compile_vertex_shader(_screen_vertex_shader_path.data()),
                        shared::shader::compile_fragment_shader(_screen_fragment_shader_path.data())
                }
        );

        unsigned vbo[3];
        glGenBuffers(3, vbo);

        // cube.
        unsigned cube_vao;
        glGenVertexArrays(1, &cube_vao);

        // bind the VAO first, then bind and set vertex buffer(s),
        // then configure vertex buffer(s).
        glBindVertexArray(cube_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // vbo bind.
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        // linking vertex attributes.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.
        glBindVertexArray(0);

        // plane
        unsigned plane_vao;
        glGenVertexArrays(1, &plane_vao);

        glBindVertexArray(plane_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // quad
        unsigned quad_vao;
        glGenVertexArrays(1, &quad_vao);

        glBindVertexArray(quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // frame buffer as texture color buffer.
        unsigned fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // texture color buffer.
        unsigned texture_color_buffer;
        glGenTextures(1, &texture_color_buffer);
        glBindTexture(GL_TEXTURE_2D, texture_color_buffer); // start binding
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // glBindTexture(GL_TEXTURE_2D, 0); // end binding

        // 1. target: fb type. (draw, read, both)
        // 2. attachment: attachment type.
        // 3. textarget: texture type.
        // 4. texture
        // 5. level: mipmap level.
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);

        // render buffer object for depth/stencil buffer.
        unsigned rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            fmt::print("[error][frame buffer] fb is not yet completed!\n");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // bind back to the default frameBuffer

#pragma region texture

        // shared::texture::set_flip_vertically(true);

        // wooden box diffuse
        const unsigned box_diffuse_tex = shared::texture::load_texture_2d(
                _box_diffuse_tex_path.data(),
                {
                        std::make_pair(GL_TEXTURE_WRAP_S, GL_REPEAT),
                        std::make_pair(GL_TEXTURE_WRAP_T, GL_REPEAT)
                },
                {
                        std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                        std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
                },
                true
        );

#pragma endregion texture

        _box_shader->use();
        _box_shader->set_int("tex", 0);

        _screen_shader->use();
        _screen_shader->set_int("screen_tex", 0);
        constexpr float offset = 1.0f / 300.0f;
        _screen_shader->set_vec2("offsets[0]", {-offset, offset});
        _screen_shader->set_vec2("offsets[1]", {0, offset});
        _screen_shader->set_vec2("offsets[2]", {offset, offset});
        _screen_shader->set_vec2("offsets[3]", {-offset, 0});
        _screen_shader->set_vec2("offsets[4]", {0, 0});
        _screen_shader->set_vec2("offsets[5]", {offset, 0});
        _screen_shader->set_vec2("offsets[6]", {-offset, -offset});
        _screen_shader->set_vec2("offsets[7]", {0, -offset});
        _screen_shader->set_vec2("offsets[8]", {offset, -offset});

        // set sharpen kernel.
//        _screen_shader->set_float("kernels[0]", -1.0f);
//        _screen_shader->set_float("kernels[1]", -1.0f);
//        _screen_shader->set_float("kernels[2]", -1.0f);
//        _screen_shader->set_float("kernels[3]", -1.0f);
//        _screen_shader->set_float("kernels[4]", 9.0f);
//        _screen_shader->set_float("kernels[5]", -1.0f);
//        _screen_shader->set_float("kernels[6]", -1.0f);
//        _screen_shader->set_float("kernels[7]", -1.0f);
//        _screen_shader->set_float("kernels[8]", -1.0f);

        // set blur kernel.
//        _screen_shader->set_float("kernels[0]", 1.0f / 16);
//        _screen_shader->set_float("kernels[1]", 2.0f / 16);
//        _screen_shader->set_float("kernels[2]", 1.0f / 16);
//        _screen_shader->set_float("kernels[3]", 2.0f / 16);
//        _screen_shader->set_float("kernels[4]", 4.0f / 16);
//        _screen_shader->set_float("kernels[5]", 2.0f / 16);
//        _screen_shader->set_float("kernels[6]", 1.0f / 16);
//        _screen_shader->set_float("kernels[7]", 2.0f / 16);
//        _screen_shader->set_float("kernels[8]", 1.0f / 16);

        // set edge detection.
        _screen_shader->set_float("kernels[0]", 1.0f);
        _screen_shader->set_float("kernels[1]", 1.0f);
        _screen_shader->set_float("kernels[2]", 1.0f);
        _screen_shader->set_float("kernels[3]", 1.0f);
        _screen_shader->set_float("kernels[4]", -8.0f);
        _screen_shader->set_float("kernels[5]", 1.0f);
        _screen_shader->set_float("kernels[6]", 1.0f);
        _screen_shader->set_float("kernels[7]", 1.0f);
        _screen_shader->set_float("kernels[8]", 1.0f);

        while (!glfwWindowShouldClose(_window)) {

            // calculate delta time.
            shared::time::tick(glfwGetTime);

            process_input(_window);

            if (enable_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma region first pass

            // bind the color frame buffer.
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glEnable(GL_DEPTH_TEST);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            const glm::mat4 proj = glm::perspective(glm::radians(shared::camera::get_fov()),
                                                    static_cast<float>(_width) / static_cast<float>(_height),
                                                    _near,
                                                    _far);
            glm::mat4 model{1.0f};

            _box_shader->use();
            _box_shader->set_mat4("model", model);
            _box_shader->set_mat4("view", shared::camera::get_view_matrix());
            _box_shader->set_mat4("projection", proj);

            // draw plane.
            glBindVertexArray(plane_vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, box_diffuse_tex);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            // draw boxes with texture
            glBindVertexArray(cube_vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, box_diffuse_tex);

            model = glm::translate(model, {-1.0f, 0.0f, -1.0f});
            _box_shader->set_mat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4{1.0f};
            model = glm::translate(model, {2.0f, 0.0f, 0.0f});
            _box_shader->set_mat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);

#pragma endregion first pass

#pragma region second pass

            // bind back to the default frame buffer.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            _screen_shader->use();

            glBindVertexArray(quad_vao);
            glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
            glDrawArrays(GL_TRIANGLES, 0, 6);

#pragma endregion second pass

            glfwSwapBuffers(_window);
            glfwPollEvents(); // poll IO events.
        }

        glDeleteVertexArrays(1, &cube_vao);
        glDeleteVertexArrays(1, &plane_vao);
        glDeleteVertexArrays(1, &quad_vao);
        glDeleteBuffers(3, vbo);
        glDeleteFramebuffers(1, &fbo);

        glfwTerminate();

        return 0;
    }

    void Runner::on_resize_frame_buffer(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Runner::process_input(GLFWwindow *window) {
        // close a window.
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        shared::camera::on_update(window);
    }

    void Runner::on_receive_mouse_event_impl(GLFWwindow *window, double x, double y) {
        shared::camera::on_update_mouse(window, x, y);
    }

    void Runner::on_receive_scroll_event(GLFWwindow *window, double x, double y) {
        shared::camera::on_update_scroll(window, x, y);
    }
}
