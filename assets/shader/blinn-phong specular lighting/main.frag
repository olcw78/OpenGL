#version 410 core

in vec3 normal;
in vec3 frag_pos;

out vec4 final_color;

uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_pos;

uniform vec3 view_pos;

uniform float ambient_strength = 0.1;
uniform float specular_strength = 0.5;
uniform float shininess = 32;

void main() {
    // calculate lighting.
    vec3 n = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);

    float diff = max(dot(n, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    // calculate specular.
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, n);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    vec3 specular = specular_strength * spec * light_color;

    // calculate ambient.
    vec3 ambient = ambient_strength * light_color;

    // compose result.
    vec3 result = (ambient + diffuse + specular) * object_color;

    final_color = vec4(result, 1.0);
}