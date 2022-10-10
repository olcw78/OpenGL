#version 410 core
layout (location = 0) in vec3 __pos;
layout (location = 1) in vec2 __uv;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    uv = __uv;
    gl_Position = projection * view * model * vec4(__pos, 1.0);
}