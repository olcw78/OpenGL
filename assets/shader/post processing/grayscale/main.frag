#version 410 core

out vec4 frag_color;
in vec2 uv;

uniform sampler2D screen_tex;

void main() {
    frag_color = texture(screen_tex, uv);
    float average = (0.2126 * frag_color.r + 0.7152 * frag_color.g + 0.0722 * frag_color.b) / 3.0;
    frag_color = vec4(vec3(average), 1);
}