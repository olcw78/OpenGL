//
// Created by 이윤상 on 2022/10/06.
//

#include "fmt/core.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

#include "runner.h"

#include "opengl_status_checker/opengl_status_checker.h"
#include "stb/stb_image.h"
#include "camera/camera.h"
#include "time/time.h"
#include "texture/texture.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace highp {
    constexpr static const float vertices[]{
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

    Runner::Runner(int width, int height,
                   float near, float far,
                   const char *title,
                   std::string_view texture_mapping_vertex_shader_path,
                   std::string_view texture_mapping_fragment_shader_path,
                   std::string_view single_color_vertex_shader_path,
                   std::string_view single_color_fragment_shader_path,
                   std::string_view diffuse_tex_path)
            : _width(width),
              _height(height),
              _near{near},
              _far{far},
              _title(title),
              _window{nullptr},
              _texture_mapping_shader{std::make_unique<shared::shader>()},
              _single_color_shader{std::make_unique<shared::shader>()},
              _texture_mapping_vertex_shader_path{texture_mapping_vertex_shader_path},
              _texture_mapping_fragment_shader_path{texture_mapping_fragment_shader_path},
              _single_color_vertex_shader_path{single_color_vertex_shader_path},
              _single_color_fragment_shader_path{single_color_fragment_shader_path},
              _diffuse_tex_path{diffuse_tex_path} {
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

        glViewport(0, 0, _width, _height);

        glfwSetFramebufferSizeCallback(_window, Runner::on_resize_frame_buffer);

        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(_window, Runner::on_receive_mouse_event_impl);

        glfwSetScrollCallback(_window, Runner::on_receive_scroll_event);

        // https://learnopengl.com/Advanced-OpenGL/Depth-testing
//        glEnable(GL_DEPTH_TEST);

        // depth test functions.
        // glDepthFunc(GL_ALWAYS); // depth test always passes.
        // glDepthFunc(GL_NEVER); // never passes.

        /* default */
        // glDepthFunc(GL_LESS); // passes if the fragment's depth is LESS than the previously stored depth value.

        // glDepthFunc(GL_EQUAL); // passes if the fragment's depth is EQUAL than the previously stored depth value.
        // glDepthFunc(GL_LEQUAL); // GL_LESS + GL_EQUAL
        // glDepthFunc(GL_GREATER); // passes if GREATER than the previous value.
        //glDepthFunc(GL_NOTEQUAL);
        // glDepthFunc(GL_GEQUAL);

        // https://learnopengl.com/Advanced-OpenGL/Stencil-testing
//        glEnable(GL_STENCIL_TEST);



        // compile and link shaders.
        opengl_status_checker::check_max_shader_attributes();

        _texture_mapping_shader->compile_and_link(
                {
                        shared::shader::compile_vertex_shader(_texture_mapping_vertex_shader_path.data()),
                        shared::shader::compile_fragment_shader(_texture_mapping_fragment_shader_path.data())
                }
        );

        _single_color_shader->compile_and_link(
                {
                        shared::shader::compile_vertex_shader(_single_color_vertex_shader_path.data()),
                        shared::shader::compile_fragment_shader(_single_color_fragment_shader_path.data())
                }
        );

        // create and bind VBO.
        unsigned vbo;
        glGenBuffers(1, &vbo);

        // create and bind VAO.
        unsigned cube_vao;
        glGenVertexArrays(1, &cube_vao);

        // bind the VAO first, then bind and set vertex buffer(s),
        // then configure vertex buffer(s).
        glBindVertexArray(cube_vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo); // vbo bind.
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // linking vertex attributes.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.
        glBindVertexArray(0);

        // create and bind VBO.
        unsigned vbo2;
        glGenBuffers(1, &vbo2);

        // create and bind VAO.
        unsigned plane_vao;
        glGenVertexArrays(1, &plane_vao);

        // bind the VAO first, then bind and set vertex buffer(s),
        // then configure vertex buffer(s).
        glBindVertexArray(plane_vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo2); // vbo bind.
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

        // linking vertex attributes.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.
        glBindVertexArray(0);

#pragma region texture

        shared::texture::set_flip_vertically(true);

        // wooden box diffuse
        const unsigned diffuse_tex = shared::texture::load_texture_2d(
                _diffuse_tex_path.data(),
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

        _texture_mapping_shader->use();
        _texture_mapping_shader->set_int("tex", 0);

        while (!glfwWindowShouldClose(_window)) {

            // calculate delta time.
            shared::time::tick(glfwGetTime);

            process_input(_window);

            glClearColor(0, 0, 0, 0);

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            // glDepthMask(GL_FALSE); - disable writing depth buffer.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes).

            if (enable_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            const glm::mat4 proj = glm::perspective(glm::radians(shared::camera::get_fov()),
                                                    static_cast<float>(_width) / static_cast<float>(_height),
                                                    _near,
                                                    _far);

#pragma region draw objects with texture
            glm::mat4 cube_model{1.0f};

            _texture_mapping_shader->use();
            _texture_mapping_shader->set_mat4("view", shared::camera::get_view_matrix());
            _texture_mapping_shader->set_mat4("projection", proj);

            // draw plane with texture
            glBindVertexArray(plane_vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuse_tex);

            _texture_mapping_shader->set_mat4("model", glm::mat4{1.0f});
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);

            // always pass stencil test so the outline is always drawn on the plane.
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);

            // draw boxes with texture
            glBindVertexArray(cube_vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuse_tex);

            cube_model = glm::translate(cube_model, {-1.0f, 0.0f, -1.0f});
            _texture_mapping_shader->set_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            cube_model = glm::mat4{1.0f};
            cube_model = glm::translate(cube_model, {2.0f, 0.0f, 0.0f});
            _texture_mapping_shader->set_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);

#pragma endregion draw objects with texture

#pragma region draw single colors

            // draw single color boxes
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);

            glDisable(GL_DEPTH_TEST);

            _single_color_shader->use();

            _single_color_shader->set_mat4("view", shared::camera::get_view_matrix());
            _single_color_shader->set_mat4("projection", proj);

            glBindVertexArray(cube_vao);

            cube_model = glm::mat4{1.0f};
            cube_model = glm::translate(cube_model, {-1.0f, 0.0f, -1.0f});
            cube_model = glm::scale(cube_model, {1.05f, 1.05f, 1.05f});
            _single_color_shader->set_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            cube_model = glm::mat4{1.0f};
            cube_model = glm::translate(cube_model, {2.0f, 0.0f, 0.0f});
            cube_model = glm::scale(cube_model, {1.05f, 1.05f, 1.05f});
            _single_color_shader->set_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);

            glEnable(GL_DEPTH_TEST);

#pragma endregion draw single colors

            glfwSwapBuffers(_window);
            glfwPollEvents(); // poll IO events.
        }

        glDeleteVertexArrays(1, &cube_vao);
        glDeleteVertexArrays(1, &plane_vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vbo2);

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
