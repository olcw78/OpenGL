#version 410 core
layout (location = 0) in vec3 __pos;
layout (location = 1) in vec3 __normal;

out vec3 normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    normal = mat3(model) * __normal;

    vec4 h_pos = vec4(__pos, 1.0);

    frag_pos = vec3(model * h_pos);
    gl_Position = projection * view * model * h_pos;
}