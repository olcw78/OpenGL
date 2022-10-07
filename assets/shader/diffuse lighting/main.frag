#version 410 core

in vec3 normal;
in vec3 frag_pos;

out vec4 final_color;

uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_pos;

float ambient_strength = 0.1;

void main() {
    // calculate lighting.
    vec3 n = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);

    float diff = max(dot(n, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    // calculate ambient.
    vec3 ambient = ambient_strength * light_color;

    // compose result.
    vec3 result = (ambient + diffuse) * object_color;

    final_color = vec4(result, 1.0);
}