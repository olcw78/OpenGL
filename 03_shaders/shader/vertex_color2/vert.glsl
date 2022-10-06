#version 410 core
// in
layout (location = 0) in vec3 __pos;
layout (location = 1) in vec3 __color;

// out
out vec3 vertex_color;

void main() {
    gl_Position = vec4(__pos.xyz, 1.0);
    vertex_color = __color;
}
