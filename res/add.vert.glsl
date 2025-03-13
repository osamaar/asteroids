#version 330


layout (location = 0) in vec2 vertPos;
layout (location = 1) in vec2 texCoord;

out vec2 vTexCoord;


void main() {
    vTexCoord = texCoord;
    gl_Position = vec4(vertPos, 0.0, 1.0);
}
