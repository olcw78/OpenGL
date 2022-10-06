#version 410 core
out vec4 frag_color;

uniform vec4 user_color;

void main() {
    frag_color = user_color;
}