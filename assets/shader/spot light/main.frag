#version 410 core

in vec3 frag_pos;
in vec3 normal;
in vec2 uv;

out vec4 final_color;

uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
} material;

uniform struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 pos;
    vec3 dir;
    float cutoff;
    float outer_cutoff;

    float constant;
    float linear;
    float quadratic;
} light;

uniform vec3 view_pos;

void main() {
    vec3 diffuse_tex_col = texture(material.diffuse, uv).rgb;
    vec3 specular_tex_col = texture(material.specular, uv).rgb;

    // ambient
    vec3 ambient = light.ambient * diffuse_tex_col;

    // spot light
    vec3 result;
    vec3 light_dir = normalize(light.pos - frag_pos);
    float theta = dot(light_dir, normalize(-light.dir));

    //    if (theta < light.cutoff) {
    //        result = ambient;
    //    } else {
    //        // diffuse
    //        vec3 n = normalize(normal);
    //        float diff = max(dot(n, light_dir), 0);
    //
    //        vec3 diffuse = diff * light.diffuse * diffuse_tex_col;
    //
    //        // specular
    //        vec3 view_dir = normalize(view_pos - frag_pos);
    //        vec3 reflect_dir = reflect(-light_dir, n);
    //        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    //
    //        vec3 specular = spec * light.specular * specular_tex_col;
    //
    //        // attenuation
    //        float distance = length(light.pos - frag_pos);
    //        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    //
    //        diffuse *= attenuation;
    //        specular *= attenuation;
    //
    //        // soft edge
    //        float epsilon = light.cutoff - light.outer_cutoff;
    //        float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
    //
    //        diffuse *= intensity;
    //        specular *= intensity;
    //
    //        result = ambient + diffuse + specular;
    //    }

    // diffuse
    vec3 n = normalize(normal);
    float diff = max(dot(n, light_dir), 0);

    vec3 diffuse = diff * light.diffuse * diffuse_tex_col;

    // specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, n);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 specular = spec * light.specular * specular_tex_col;

    // attenuation
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    diffuse *= attenuation;
    specular *= attenuation;

    // soft edge
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    result = ambient + diffuse + specular;

    final_color = vec4(result, 1);
}