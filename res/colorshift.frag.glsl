#version 150

uniform vec2 offset;

in vec2  vTexCoord;
out vec4 outColor;

uniform sampler2D screenQuadTex;

void main() {
    float r = texture(screenQuadTex, vTexCoord+offset).r;
    float g = texture(screenQuadTex, vTexCoord-offset).g;
    float b = texture(screenQuadTex, vTexCoord).b;
    outColor = vec4(r, g, b, 1.0);
}
