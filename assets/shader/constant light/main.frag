#version 410 core
out vec4 final_color;

void main() {
    final_color = vec4(light_color * object_color, 1);
}