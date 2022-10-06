#version 410 core
out vec4 frag_color;

in vec3 our_color;
in vec2 uv;

// On peut assigner une valuer d'une position au sampler texture
// donc on peut mettre multiple de textures d'un coup dans le fragment shader.
//
// Cette position de texture s'appelle << texture unit >>.
// L'unite texture par défaut est au 0 lequel est le texture activé, alors qu'on n'avait pas besoin d'assigner
// à la position qu'avant.
//
// le but principal de l'unite texture, il nous fait utiliser plus d'un texture dans nôs shaders.
// de plus qu'on puisse lier les multiples textures d'un coup au long qu'on active l'un correct.
// ainsi que le << glBindTexture >>, on peut activer l'unite texture en utilisant << glActiveTexture>> en y passant.
// À partir de ce que le GL_TEXTURE0 s'est par défaut activé, on n'a pas eu besoin d'activer aucunes unites textures qu'avant.
//
// L'OpenGL doit avoir au minimum 16 unites textures puis on peut préciser de GL_TEXTURE0 à GL_TEXTURE16,
// par ailleurs, GL_TEXTURE8 est la même que GL_TEXTURE0 + 8 quel est util quand l'on répéte sur plusieurs unités de texture.
uniform sampler2D texture_wall;
uniform sampler2D texture_awesomeface;

void main() {
    frag_color = mix(
        texture(texture_wall, uv),
        texture(texture_awesomeface, uv),
        0.2
    ) * vec4(our_color, 1.0);
}