#version 410 core

out vec4 final_color;

in vec3 goraud_fragment_color;

uniform vec3 object_color;

void main() {
    final_color = vec4(goraud_fragment_color * object_color, 1.0);
}