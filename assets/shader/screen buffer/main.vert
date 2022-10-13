#version 410 core
layout (location = 0) in vec2 __pos;
layout (location = 1) in vec2 __uv;

out vec2 uv;

void main() {
    gl_Position = vec4(__pos.xy, 0, 1);
    uv = __uv;
}