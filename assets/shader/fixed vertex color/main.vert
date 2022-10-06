#version 410 core
// in
layout (location = 0) in vec3 in_pos;

// out
out vec4 vertex_color;

void main() {
    gl_Position = vec4(in_pos.xyz, 1.0);
    vertex_color = vec4(0.5, 0.0, 0.0, 1.0);
}
