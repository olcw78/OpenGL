#version 410 core

layout (location = 0) in vec3 __pos;
layout (location = 1) in vec3 __normal;
layout (location = 2) in vec2 __uv;

out vec3 frag_pos;
out vec3 normal;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    frag_pos = vec3(model * vec4(__pos, 1));
    normal = mat3(transpose(inverse(model))) * __normal;
    uv = __uv;

    gl_Position = projection * view * vec4(frag_pos, 1.0);
}