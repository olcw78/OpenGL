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
    constexpr static const float box_vertices[]{
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

    constexpr const float transparent_vertices[]{
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
            1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

            0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
            1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.5f, 0.0f, 1.0f, 0.0f
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
                   std::string_view grass_vertex_shader_path,
                   std::string_view grass_fragment_shader_path,
                   std::string_view box_diffuse_tex_path,
                   std::string_view grass_tex_path)
            : _width(width),
              _height(height),
              _near{near},
              _far{far},
              _title(title),
              _window{nullptr},
              _box_shader{std::make_unique<shared::shader>()},
              _box_vertex_shader_path{box_vertex_shader_path},
              _box_fragment_shader_path{box_fragment_shader_path},
              _grass_shader{std::make_unique<shared::shader>()},
              _grass_vertex_shader_path{grass_vertex_shader_path},
              _grass_fragment_shader_path{grass_fragment_shader_path},
              _box_diffuse_tex_path{box_diffuse_tex_path},
              _grass_tex_path{grass_tex_path} {
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
        glEnable(GL_CULL_FACE);
//        glCullFace(GL_BACK);
//        glCullFace(GL_CCW);

        glfwSetFramebufferSizeCallback(_window, Runner::on_resize_frame_buffer);

        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(_window, Runner::on_receive_mouse_event_impl);

        glfwSetScrollCallback(_window, Runner::on_receive_scroll_event);

        // compile and link shaders.
        opengl_status_checker::check_max_shader_attributes();

        _box_shader->compile_and_link(
                {
                        shared::shader::compile_vertex_shader(_box_vertex_shader_path.data()),
                        shared::shader::compile_fragment_shader(_box_fragment_shader_path.data())
                }
        );

        _grass_shader->compile_and_link(
                {
                        shared::shader::compile_vertex_shader(_grass_vertex_shader_path.data()),
                        shared::shader::compile_fragment_shader(_grass_fragment_shader_path.data())
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

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
        unsigned grass_vao;
        glGenVertexArrays(1, &grass_vao);

        // bind the VAO first, then bind and set vertex buffer(s),
        // then configure vertex buffer(s).
        glBindVertexArray(grass_vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo2); // vbo bind.
        glBufferData(GL_ARRAY_BUFFER, sizeof(transparent_vertices), transparent_vertices, GL_STATIC_DRAW);

        // linking vertex attributes.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.
        glBindVertexArray(0);

        unsigned vbo3;
        glGenBuffers(1, &vbo3);

        unsigned plane_vao;
        glGenVertexArrays(1, &plane_vao);

        glBindVertexArray(plane_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo3);
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

#pragma region texture

//        shared::texture::set_flip_vertically(true);

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

        const unsigned grass_diffuse_tex = shared::texture::load_texture_2d(
                _grass_tex_path.data(),
                {
                        std::make_pair(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE),
                        std::make_pair(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
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

        _grass_shader->use();
        _grass_shader->set_int("tex", 0);

        while (!glfwWindowShouldClose(_window)) {

            // calculate delta time.
            shared::time::tick(glfwGetTime);

            process_input(_window);

            glClearColor(0, 0, 0, 0);

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // 1. sfactor: how the source is computed.
            // 2. dfactor: how the destination is computed.

//            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
            // 1. srcRGB
            // 2. dstRGB
            // 3. srcAlpha
            // 4. dstAlpha

//            glBlendEquation(GL_FUNC_ADD);

            // glDepthMask(GL_FALSE); - disable writing depth buffer.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (enable_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            const glm::mat4 proj = glm::perspective(glm::radians(shared::camera::get_fov()),
                                                    static_cast<float>(_width) / static_cast<float>(_height),
                                                    _near,
                                                    _far);
            glm::mat4 cube_model{1.0f};

#pragma region draw objects with texture

            _box_shader->use();
            _box_shader->set_mat4("view", shared::camera::get_view_matrix());
            _box_shader->set_mat4("projection", proj);

            // draw plane with texture
            glBindVertexArray(grass_vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, grass_diffuse_tex);

            glm::mat4 grass_model{1.0f};
            for (glm::vec3 const &pos: grass_positions) {
                grass_model = glm::translate(grass_model, pos);
                _box_shader->set_mat4("model", grass_model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }

            glBindVertexArray(0);

            // draw plane.
            glBindVertexArray(plane_vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, box_diffuse_tex);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            // draw boxes with texture
            glBindVertexArray(cube_vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, box_diffuse_tex);

            cube_model = glm::translate(cube_model, {-1.0f, 0.0f, -1.0f});
            _box_shader->set_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            cube_model = glm::mat4{1.0f};
            cube_model = glm::translate(cube_model, {2.0f, 0.0f, 0.0f});
            _box_shader->set_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);

#pragma endregion draw objects with texture

            glfwSwapBuffers(_window);
            glfwPollEvents(); // poll IO events.
        }

        glDeleteVertexArrays(1, &cube_vao);
        glDeleteVertexArrays(1, &grass_vao);
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
