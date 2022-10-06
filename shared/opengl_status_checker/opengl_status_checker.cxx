//
// Created by 이윤상 on 2022/10/06.
//

#include "opengl_status_checker.h"

#include<fmt/core.h>
#include<glad/gl.h>
#include<GLFW/glfw3.h>

void opengl_status_checker::check_max_shader_attributes() {
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    fmt::print("Maximum nr of vertex attributes supported: {}\n", nrAttributes);
}
