#version 410 core
out vec4 final_color;

in vec3 normal;
in vec3 frag_pos;
in vec2 uv;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main() {
    // diffuse
    vec3 diffuse_tex_color = texture(material.diffuse, uv).rgb;

    vec3 n = normalize(normal);
    vec3 d = normalize(light.pos - frag_pos);
    float diff = max(dot(n, d), 0.0);

    vec3 diffuse = diff * light.diffuse * diffuse_tex_color;

    // specular;
    vec3 specular_tex_color = texture(material.specular, uv).rgb;

    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-d, n);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 specular = spec * light.specular * specular_tex_color;

    // ambient;
    vec3 ambient = light.ambient * diffuse_tex_color;

    // gather the fragments.
    vec3 result = ambient + diffuse + specular;

    final_color = vec4(result, 1.0);
}