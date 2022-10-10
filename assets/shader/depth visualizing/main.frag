#version 410 core
out vec4 frag_color;

float near = 0.1f;
float far = 100.0f;

float linearised_depth(float depth) {
    float ndc_z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - ndc_z * (far - near));
}

void main() {
    float depth = linearised_depth(gl_FragCoord.z) / far;
    frag_color = vec4(vec3(depth), 1.0);
}