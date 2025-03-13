#version 150

#define PI 3.1415926535897932384626433832795

in vec2  vTexCoord;
out vec4 outColor;

uniform sampler2D screenQuadTex;
//uniform vec2 resolution;
uniform float chromAbb = 0.02;
uniform float vignette = 0.2;

void main() {
    // Offset and distance from screen center
    vec2 offset = vTexCoord - vec2(0.5, 0.5);
    float dist = length(offset);

    // Chromatic abbaration
    float r = texture(screenQuadTex, vTexCoord+2.0*offset*chromAbb).r;
    float g = texture(screenQuadTex, vTexCoord-2.0*offset*chromAbb).g;
    float b = texture(screenQuadTex, vTexCoord).b;
    outColor = vec4(r, g, b, 1.0);

    // Vignette
    outColor.rgb *= 1.0 - (2.0*dist)*vignette;

    // Scanline
    outColor.rgb *= 0.4 + 0.6 * (1.0 + sin(500.0*vTexCoord.y*PI));
}
