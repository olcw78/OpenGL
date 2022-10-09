#version 410 core

in vec3 frag_pos;
in vec3 normal;
in vec2 uv;

out vec4 final_color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

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

    float constant;
    float linear;
    float quadratic;

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
);

vec3 calc_point_light(
    Material material,
    PointLight light,
    vec3 diffuse_tex_col,
    vec3 specular_tex_col,
    vec3 normal,
    vec3 frag_pos,
    vec3 view_dir
);

vec3 calc_spot_light(
    Material material,
    SpotLight light,
    vec3 diffuse_tex_col,
    vec3 specular_tex_col,
    vec3 normal,
    vec3 frag_pos,
    vec3 view_dir
);

uniform Material material;
uniform DirectionalLight dir_light;

#define NR_POINT_LIGHT_COUNT 4
uniform PointLight point_lights[NR_POINT_LIGHT_COUNT];

uniform SpotLight spot_light;

uniform vec3 view_pos;

void main() {
    // acquire
    vec3 diffuse_tex_col = texture(material.diffuse, uv).rgb;
    vec3 specular_tex_col = texture(material.specular, uv).rgb;

    vec3 n = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    vec3 final;

    final += calc_dir_light(
        material,
        dir_light,
        diffuse_tex_col,
        specular_tex_col,
        normal,
        view_dir
    );

    for (int i = 0; i < NR_POINT_LIGHT_COUNT; ++i) {
        final += calc_point_light(
            material,
            point_lights[i],
            diffuse_tex_col,
            specular_tex_col,
            normal,
            frag_pos,
            view_dir
        );
    }

    final += calc_spot_light(
        material,
        spot_light,
        diffuse_tex_col,
        specular_tex_col,
        normal,
        frag_pos,
        view_dir
    );

    final_color = vec4(final, 1.0);
}

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
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // cutoff
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * diffuse_tex_col * intensity;
    vec3 diffuse = light.diffuse * diff * diffuse_tex_col * intensity * attenuation;
    vec3 specular = light.specular * spec * specular_tex_col * intensity * attenuation;

    return ambient + diffuse + specular;
}

