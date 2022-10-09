#include "material.glsl"

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float inner_cutoff;
    float outer_cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calc_dir_light(
    Material material,
    DirectionalLight light,
    vec3 diffuse_tex_col,
    vec3 specular_tex_col,
    vec3 normal,
    vec3 view_dir
) {
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 ambient = light.ambient * diffuse_tex_col;
    vec3 diffuse = light.diffuse * diff * diffuse_tex_col;
    vec3 specular = light.specular * spec * specular_tex_col;

    return ambient + diffuse + specular;
}

vec3 calc_point_light(
    Material material,
    PointLight light,
    vec3 diffuse_tex_col,
    vec3 specular_tex_col,
    vec3 normal,
    vec3 frag_pos,
    vec3 view_dir
) {
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * diffuse_tex_col;
    vec3 diffuse = light.diffuse * diff * diffuse_tex_col * attenuation;
    vec3 specular = light.specular * spec * specular_tex_col * attenuation;

    return ambient + diffuse + specular;
}

vec3 calc_spot_light(
    Material material,
    SpotLight light,
    vec3 diffuse_tex_col,
    vec3 specular_tex_col,
    vec3 normal,
    vec3 frag_pos,
    vec3 view_dir
) {
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // cutoff
    float theta = dot(light.direction, normalize(-light.direction));
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0, 1);

    vec3 ambient = light.ambient * diffuse_tex_col;
    vec3 diffuse = light.diffuse * diff * diffuse_tex_col * intensity;
    vec3 specular = light.specular * spec * specular_tex_col * intensity;

    return ambient + diffuse + specular;
}

