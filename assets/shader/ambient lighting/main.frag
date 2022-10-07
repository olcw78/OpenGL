#version 410 core

out vec4 final_color;

uniform vec3 light_color;
uniform vec3 object_color;

void main() {
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;
    vec3 result = ambient * object_color;

    final_color = vec4(result, 1);
}