#version 410 core
out vec4 frag_color;

in vec3 color;
in vec2 uv;

uniform sampler2D texture_wall;
uniform sampler2D texture_awesomeface;

void main() {
    frag_color = mix(
        texture(texture_wall, uv),
        texture(texture_awesomeface, uv),
        0.4
    );
}