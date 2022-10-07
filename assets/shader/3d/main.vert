#version 410 core
layout (location = 0) in vec3 __pos;
layout (location = 1) in vec2 __uv;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(__pos.xyz, 1.0);
    uv = __uv;
}