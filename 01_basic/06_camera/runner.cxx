//
// Created by 이윤상 on 2022/10/06.
//


#include "fmt/core.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "runner.h"

#include "opengl_status_checker/opengl_status_checker.h"
#include "stb/stb_image.h"
#include "camera/camera.h"
#include "time/time.h"

namespace highp {
    Runner::Runner(int width, int height, const char *title,
                   std::string_view vertex_shader_src_path_abs,
                   std::string_view fragment_shader_src_path_abs,
                   std::string_view wall_texture_path_abs,
                   std::string_view awesomeface_texture_path_abs)
            : _width(width),
              _height(height),
              _title(title),
              _cube_shader{std::make_unique<shader>(
                      vertex_shader_src_path_abs,
                      fragment_shader_src_path_abs
              )},
              _wall_texture_path{wall_texture_path_abs},
              _awesomeface_texture_path{awesomeface_texture_path_abs} {
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

//    const on_receive_mouse_event event = std::bind(&Runner::on_receive_mouse_event_impl, this, std::placeholders::_1,
//                                                   std::placeholders::_2, std::placeholders::_3);


        glfwSetCursorPosCallback(_window, Runner::on_receive_mouse_event_impl);
        glfwSetScrollCallback(_window, Runner::on_receive_scroll_event);

        opengl_status_checker::check_max_shader_attributes();

        this->_cube_shader->compile_and_link();

        constexpr const glm::vec3 cubePositions[]{
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f, 3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f, 2.0f, -2.5f),
                glm::vec3(1.5f, 0.2f, -1.5f),
                glm::vec3(-1.3f, 1.0f, -1.5f)
        };

        // create and bind VBO.
        constexpr const float vertices[]{
                // 3 - pos    // 2 - uv
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

//    constexpr const unsigned indices[]{
//            0, 1, 3,
//            1, 2, 3
//    };

        // create and bind VAO.
        unsigned vao;
        glGenVertexArrays(1, &vao);

        unsigned vbo;
        glGenBuffers(1, &vbo);

//    unsigned ebo;
//    glGenBuffers(1, &ebo);

        // bind the VAO first, then bind and set vertex buffer(s),
        // then configure vertex buffer(s).
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // linking vertex attributes.

        // vertex color2

        // 1. __pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // 2. __color
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
//    glEnableVertexAttribArray(1);

//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
//    glEnableVertexAttribArray(2);

        // 3. __uv
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // unbind vao after so other vao calls won't modify this VAO.
        glBindVertexArray(0);

        // gen and bind texture 1.
        stbi_set_flip_vertically_on_load(true);

        int width;
        int height;
        int nr_channels;

        // gen and bind texture 2.
        unsigned texture_awesomeface;
        glGenTextures(1, &texture_awesomeface);
        glBindTexture(GL_TEXTURE_2D, texture_awesomeface);

        // set wrapping params.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture_wall filtering params.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const char *awesomeface_texture_file_path = _awesomeface_texture_path.data();
        unsigned char *data_awesomeface = stbi_load(awesomeface_texture_file_path, &width, &height, &nr_channels, 0);

        if (data_awesomeface != nullptr) {
            // awesome face image has transparency thus make sure using alpha channel on loading it.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_awesomeface);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            fmt::print("[error][gen texture_wall] fail to load texture_wall at {}\n", awesomeface_texture_file_path);
        }

        stbi_image_free(data_awesomeface);

        // make sure using shader _shader_program.
        _cube_shader->use();
        _cube_shader->set_int("texture_awesomeface", 0);

        // camera
        glm::vec3 camera_pos{0, 0, 3.0f};
        const glm::vec3 camera_target{0, 0, 0};
        const glm::vec3 camera_direction{glm::normalize(camera_pos - camera_target)};

        const glm::vec3 world_up{0, 1, 0};
        const glm::vec3 camera_right{glm::normalize(glm::cross(world_up, camera_direction))};
        const glm::vec3 camera_up{glm::cross(camera_direction, camera_right)};

        _cube_shader->use();

//    const float radius = 10.0f;

        while (!::glfwWindowShouldClose(_window)) {

            // calculate delta time.
            shared::time::tick(glfwGetTime);

            process_input(_window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (enable_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_awesomeface);

            _cube_shader->use();

            // camera
//        const float cam_x = static_cast<float>(sin(glfwGetTime()) * radius);
//        const float cam_z = static_cast<float>(cos(glfwGetTime()) * radius);
//        fmt::print("cam_x: {}, cam_z: {}\n", cam_x, cam_z);

//        const glm::mat4 view{glm::lookAt(
//                glm::vec3{cam_x, 0, cam_z},
//                glm::vec3{0, 0, 0},
//                world_up
//        )};


//        const glm::mat4 view{
//                glm::lookAt(camera_pos,
//                            camera_pos + camera_front,
//                            _camera_up)
//        };
            _cube_shader->set_mat4("view", shared::camera::get_view_matrix());

            const glm::mat4 proj = glm::perspective(glm::radians(shared::camera::get_fov()),
                                                    static_cast<float>(1024 / 768),
                                                    0.1f,
                                                    100.0f);
            _cube_shader->set_mat4("projection", proj);

            // render boxes.
            glBindVertexArray(vao);
            for (auto i = 0; i < 10; ++i) {
                glm::mat4 model{1.0f};
                model = glm::translate(model, cubePositions[i]);
                const float next_step_angle = 20.0f * (i + 1);
                model = glm::rotate(model, glm::radians(next_step_angle) * static_cast<float>(glfwGetTime()),
                                    glm::vec3{1, 0.3f, 0.5f});

                _cube_shader->set_mat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(_window);
            glfwPollEvents(); // poll IO events.
        }

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
//    glDeleteBuffers(1, &ebo);

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