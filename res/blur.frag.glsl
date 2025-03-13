#version 330

in vec2  vTexCoord;
out vec4 outColor;

uniform sampler2D screenQuadTex;
uniform vec2 resolution;
uniform vec2 direction;       //1.0, 0.0 -> Hor, 0.0, 1.0 -> Ver

uniform float weights[5] = float[](0.2270270270, 0.1945945946, 0.1216216216,
                                  0.0540540541, 0.0162162162);

void main() {
    vec2 pixelStep = 1.0/resolution;

    vec4 sum = vec4(0.0);
    vec2 offset = vec2(0.0, 0.0);

    //apply blurring, using a 9-tap filter with predefined gaussian weights
    sum += texture2D(screenQuadTex, vTexCoord) *  weights[0];

    for (int i = 1; i < 5; i++) {
        offset = i * direction;
        sum += texture2D(screenQuadTex, vTexCoord + pixelStep * offset)
               * weights[i];
        sum += texture2D(screenQuadTex, vTexCoord - pixelStep * offset)
               * weights[i];
    }

    // discard alpha
    outColor = vec4(sum.rgb, 1.0);
}

