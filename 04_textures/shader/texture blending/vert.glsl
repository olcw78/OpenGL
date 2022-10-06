#version 410 core
layout (location = 0) in vec3 __pos;
layout (location = 1) in vec3 __color;
layout (location = 2) in vec2 __uv;

out vec3 color;
out vec2 uv;

void main() {
    gl_Position = vec4(__pos.xyz, 1.0);
    uv = __uv;
}