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
    constexpr const glm::vec3 cube_positions[]{
            {0.0f,  0.0f,  0.0f},
            {2.0f,  5.0f,  -15.0f},
            {-1.5f, -2.2f, -2.5f},
            {-3.8f, -2.0f, -12.3f},
            {2.4f,  -0.4f, -3.5f},
            {-1.7f, 3.0f,  -7.5f},
            {1.3f,  -2.0f, -2.5f},
            {1.5f,  2.0f,  -2.5f},
            {1.5f,  0.2f,  -1.5f},
            {-1.3f, 1.0f,  -1.5}
    };

    constexpr const glm::vec3 point_lights_positions[]{
            {0.7f,  0.2f,  2.0f},
            {2.3f,  -3.3f, -4.0f},
            {-4.0f, 2.0f,  -12.0f},
            {0.0f,  0.0f,  -3.0}
    };

    constexpr static const float vertices[]{
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    Runner::Runner(int width, int height,
                   float near, float far,
                   const char *title,
                   std::string_view cube_vertex_shader_path,
                   std::string_view cube_fragment_shader_path,
                   std::string_view light_vertex_shader_path,
                   std::string_view light_fragment_shader_path,
                   std::string_view wooden_box_diffuse_tex_path,
                   std::string_view wooden_box_specular_tex_path)
            : _width(width),
              _height(height),
              _near{near},
              _far{far},
              _title(title),
              _window{nullptr},
              _cube_shader{std::make_unique<shared::shader>()},
              _light_shader{std::make_unique<shared::shader>()},
              _light_vertex_shader_path{light_vertex_shader_path},
              _light_fragment_shader_path{light_fragment_shader_path},
              _cube_vertex_shader_path{cube_vertex_shader_path},
              _cube_fragment_shader_path{cube_fragment_shader_path},
              _wooden_box_diffuse_tex_path{wooden_box_diffuse_tex_path},
              _wooden_box_specular_tex_path{wooden_box_specular_tex_path} {
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

        // init IMGUI.
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        // set up platform / renderer bindings.
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        ImGui::StyleColorsDark();

        // compile and link shaders.
        opengl_status_checker::check_max_shader_attributes();

        _cube_shader->compile_and_link(
                {
                        _cube_shader->compile_vertex_shader(_cube_vertex_shader_path.data()),
                        _cube_shader->compile_fragment_shader(_cube_fragment_shader_path.data())
                }
        );

        _light_shader->compile_and_link(
                {
                        _light_shader->compile_vertex_shader(_light_vertex_shader_path.data()),
                        _light_shader->compile_fragment_shader(_light_fragment_shader_path.data())
                }
        );

        // create and bind VBO.
        unsigned vbo;
        glGenBuffers(1, &vbo);

        // create and bind VAO.
        unsigned cube_vao;
        glGenVertexArrays(1, &cube_vao);

        unsigned light_vao;
        glGenVertexArrays(1, &light_vao);

        // bind the VAO first, then bind and set vertex buffer(s),
        // then configure vertex buffer(s).
        glBindVertexArray(cube_vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo); // vbo bind.
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // linking vertex attributes.
        const size_t cube_vertex_attrib_size = 8 * sizeof(float);

        const size_t start_offset = 0;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size,
                              reinterpret_cast<void *> (start_offset));
        glEnableVertexAttribArray(0);

        const size_t offset_1 = 3 * sizeof(float);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size,
                              reinterpret_cast<void *> (start_offset + offset_1));
        glEnableVertexAttribArray(1);

        const size_t offset_2 = 3 * sizeof(float);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size,
                              reinterpret_cast<void *> (start_offset + offset_1 + offset_2));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.

        glBindVertexArray(light_vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo); // vbo bind.

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size, reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.

        glBindVertexArray(0);


#pragma region texture

        shared::texture::set_flip_vertically(true);

        // wooden box diffuse
        const unsigned wooden_box_diffuse_tex = shared::texture::load_texture_2d(
                _wooden_box_diffuse_tex_path.data(),
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

        // wooden box specular
        const unsigned wooden_box_specular_tex = shared::texture::load_texture_2d(
                _wooden_box_specular_tex_path.data(),
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

        _cube_shader->use();

        _cube_shader->set_int("material.diffuse", 0);
        _cube_shader->set_int("material.specular", 1);
        _cube_shader->set_float("material.shininess", 64.0f);

        // directional light shader parameters
        _cube_shader->set_vec3("dir_light.direction", {-0.2f, -0.3f, -1.0f});

        _cube_shader->set_vec3("dir_light.ambient", {0.02f, 0.02f, 0.02f});
        _cube_shader->set_vec3("dir_light.diffuse", {0.2f, 0.2f, 0.2f});
        _cube_shader->set_vec3("dir_light.specular", {1.0f, 1.0f, 1.0f});

        // point light shader parameters
        for (auto i = 0; i < 4; ++i) {
            _cube_shader->set_vec3(fmt::format("point_lights[{}].position", i), point_lights_positions[i]);

            _cube_shader->set_float(fmt::format("point_lights[{}].constant", i), 1.0f);
            _cube_shader->set_float(fmt::format("point_lights[{}].linear", i), 0.09f);
            _cube_shader->set_float(fmt::format("point_lights[{}].quadratic", i), 0.032f);

            _cube_shader->set_vec3(fmt::format("point_lights[{}].ambient", i), {0.02f, 0.02f, 0.02f});
            _cube_shader->set_vec3(fmt::format("point_lights[{}].diffuse", i), {0.7f, 0.7f, 0.7f});
            _cube_shader->set_vec3(fmt::format("point_lights[{}].specular", i), {1.0f, 1.0f, 1.0f});
        }

        // spot light shader parameters
        _cube_shader->set_vec3("spot_light.ambient", {0.2f, 0.2f, 0.2f});
        _cube_shader->set_vec3("spot_light.diffuse", {1.2f, 1.2f, 1.2f});
        _cube_shader->set_vec3("spot_light.specular", {3.0f, 3.0f, 3.0f});

        _cube_shader->set_float("spot_light.inner_cutoff", glm::cos(glm::radians(12.5f)));
        _cube_shader->set_float("spot_light.outer_cutoff", glm::cos(glm::radians(17.5f)));

        _cube_shader->set_float("spot_light.constant", 1.0f);
        _cube_shader->set_float("spot_light.linear", 0.09f);
        _cube_shader->set_float("spot_light.quadratic", 0.032f);

        while (!glfwWindowShouldClose(_window)) {

            // calculate delta time.
            shared::time::tick(glfwGetTime);

            process_input(_window);

            glClearColor(0, 0, 0, 0);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (enable_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            // feed inputs to dear IMGUI, start new frame.
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Render GUI.
            ImGui::Begin("Demo Window");
            ImGui::Button("Hello!");
            ImGui::End();

            // Render dear IMGUI into screen.
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            const auto elapsed_time = static_cast<float>(glfwGetTime());


            const glm::mat4 proj = glm::perspective(glm::radians(shared::camera::get_fov()),
                                                    static_cast<float>(_width) / static_cast<float>(_height),
                                                    _near,
                                                    _far);

//            light_pos = {
//                    -2.0f * sin(elapsed_time),
//                    0,
//                    -1.5f * cos(elapsed_time)
//            };

            // draw cubes.
            _cube_shader->use();

            _cube_shader->set_vec3("spot_light.position", shared::camera::get_camera_pos());
            _cube_shader->set_vec3("spot_light.direction", shared::camera::get_camera_front());

            _cube_shader->set_vec3("view_pos", shared::camera::get_camera_pos());

            _cube_shader->set_mat4("view", shared::camera::get_view_matrix());
            _cube_shader->set_mat4("projection", proj);

            glBindVertexArray(cube_vao);

            // active diffuse / specular textures.
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wooden_box_diffuse_tex);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, wooden_box_specular_tex);

            for (auto i = 0; i < sizeof(cube_positions) / sizeof(glm::vec3); ++i) {
                const float next_step_angle = 5.0f * (i + 1) * elapsed_time;

                glm::mat4 cube_model{1.0f};
                cube_model = glm::translate(cube_model, cube_positions[i]);
                cube_model = glm::rotate(cube_model, glm::radians(next_step_angle), {0.3f, 0.5f, 0.0f});

                _cube_shader->set_mat4("model", cube_model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // draw lights.
            _light_shader->use();
            _light_shader->set_mat4("view", shared::camera::get_view_matrix());
            _light_shader->set_mat4("projection", proj);

            glBindVertexArray(light_vao);

            for (auto i = 0; i < sizeof(point_lights_positions) / sizeof(glm::vec3); ++i) {
                glm::mat4 model{1.0f};
                model = glm::translate(model, point_lights_positions[i]);
                model = glm::scale(model, {0.2f, 0.2f, 0.2f});
                _light_shader->set_mat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glfwSwapBuffers(_window);
            glfwPollEvents(); // poll IO events.
        }

        glDeleteVertexArrays(1, &cube_vao);
        glDeleteVertexArrays(1, &light_vao);
        glDeleteBuffers(1, &vbo);

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

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

//        shared::camera::on_update(window);
    }

    void Runner::on_receive_mouse_event_impl(GLFWwindow *window, double x, double y) {
//        shared::camera::on_update_mouse(window, x, y);
    }

    void Runner::on_receive_scroll_event(GLFWwindow *window, double x, double y) {
//        shared::camera::on_update_scroll(window, x, y);
    }
}
