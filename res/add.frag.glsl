#version 330

in vec2  vTexCoord;
out vec4 outColor;

uniform sampler2D screenQuadTex;
uniform sampler2D addTex;
uniform float factor = 1.0;

void main() {
    outColor = texture2D(screenQuadTex, vTexCoord) +
               texture2D(addTex, vTexCoord)*factor;

    //outColor = vec4(sum.rgb, 1.0) + texture(screenQuadTex, vTexCoord);
}

