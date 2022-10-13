#version 410 core
out vec4 frag_color;
in vec2 uv;

uniform sampler2D screen_tex;

const float OFFSET = 1.0 / 300.0;
#define ARRAY_SIZE 9

uniform vec2 offsets[9];
uniform float kernels[9];

void main() {

    vec3 sampled_fragments[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        sampled_fragments[i] = vec3(texture(screen_tex, uv + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        col += sampled_fragments[i] * kernels[i];
    }

    frag_color = vec4(col, 1.0);
}