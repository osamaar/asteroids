#version 330

in vec2  vTexCoord;
out vec4 outColor;

uniform sampler2D screenQuadTex;
uniform float factor;

void main() {
    vec4 sum = texture2D(screenQuadTex, vTexCoord)*factor;

    outColor = vec4(sum.rgb, 1.0) + texture(screenQuadTex, vTexCoord);
}

