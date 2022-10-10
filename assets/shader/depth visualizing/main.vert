#version 410 core
layout (location = 0) in vec3 __pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(__pos, 1.0);
}