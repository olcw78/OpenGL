#version 410 core
in vec2 uv;

out vec4 frag_col;

uniform sampler2D tex;

void main() {
    vec4 col = texture(tex, uv);
    if (col.a < 0.1)
    discard;

    frag_col = col;
}