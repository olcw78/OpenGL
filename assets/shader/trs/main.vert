#version 410 core
layout (location = 0) in vec3 __pos;
layout (location = 1) in vec2 __uv;

out vec2 uv;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(__pos, 1.0);
    uv = __uv;
}