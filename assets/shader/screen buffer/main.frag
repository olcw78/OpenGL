#version 410 core
out vec4 frag_color;
in vec2 uv;

uniform sampler2D screen_tex;

void main() {
    frag_color = texture(screen_tex, uv);
}