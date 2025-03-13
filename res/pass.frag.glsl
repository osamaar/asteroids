#version 150

in vec2  vTexCoord;
out vec4 outColor;

uniform sampler2D screenQuadTex;

void main() {
    outColor = texture(screenQuadTex, vTexCoord);
}
