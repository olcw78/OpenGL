#version 410 core

out vec4 frag_color;
in vec2 uv;

uniform sampler2D screen_tex;

void main() {
    vec4 tex_col = texture(screen_tex, uv);
    frag_color = vec4(vec3(1.0 - tex_col), 1);
}