#version 150


in vec2 vertPos;

uniform mat4 mvp;


void main() {
    gl_Position = mvp*vec4(vertPos, 0.0, 1.0);
}
