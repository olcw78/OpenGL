#version 410 core
layout (location = 0) in vec3 __pos;
layout (location = 1) in vec3 __normal;

out vec3 goraud_fragment_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 light_pos;

uniform float specular_strength = 0.5;
uniform float ambient_strength = 0.1;
uniform float shininess = 32;

void main() {

    vec4 h_pos = vec4(__pos, 1.0);
    gl_Position = projection * view * model * h_pos;

    vec3 normal = mat3(model) * __normal;

    vec3 frag_pos = vec3(model * h_pos);

    // calculate specular.
    vec3 n = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);

    // calc diffuse.
    float diff = max(dot(n, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    // calculate ambient.
    vec3 ambient = ambient_strength * light_color;

    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, n);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    vec3 specular = specular_strength * spec * light_color;

    // compose result.
    goraud_fragment_color = (ambient + diffuse + specular);
}