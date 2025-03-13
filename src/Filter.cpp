#include "helpers.h"
#include "Filter.h"
#include "Framebuffer.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <iostream>

// Syntactic sugar!
const unsigned int UNIFORM_KEY_0 = 0;

using namespace std;

Filter::Filter(const glm::vec2 &resolution)
        : mResolution(resolution)
        , mInputFramebuffer(resolution) {
    // define quad and index data to buffer and forget
    GLfloat quad[] = {
                    // pos,    tex
                    -1.0f, -1.0f,   0.0f,  0.0f,
                     1.0f, -1.0f,   1.0f,  0.0f,
                     1.0f,  1.0f,   1.0f,  1.0f,
                    -1.0f,  1.0f,   0.0f,  1.0f,
	};
    GLint indices[] = {
                    0, 1, 2,
                    0, 2, 3,
    };

    // Generate
    glGenVertexArrays(1, &mVAO);
    checkGLErrors(__FILE__, __LINE__);
    glGenBuffers(1, &mVBO);
    checkGLErrors(__FILE__, __LINE__);
    glGenBuffers(1, &mEBO);
    checkGLErrors(__FILE__, __LINE__);

    // Bind
    glBindVertexArray(mVAO);
    checkGLErrors(__FILE__, __LINE__);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    checkGLErrors(__FILE__, __LINE__);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    checkGLErrors(__FILE__, __LINE__);

    // Using Explicit attribute locations.
    GLuint posAttrib = 0;
    GLuint texAttrib = 1;

    // Set attribute pointers.
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
            4*sizeof(GLfloat), NULL);
    checkGLErrors(__FILE__, __LINE__);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
        4*sizeof(GLfloat), (const GLvoid *)(2*sizeof(GLfloat)));
    checkGLErrors(__FILE__, __LINE__);

    // Enable attributes
    glEnableVertexAttribArray(posAttrib);
    checkGLErrors(__FILE__, __LINE__);
    glEnableVertexAttribArray(texAttrib);
    checkGLErrors(__FILE__, __LINE__);

    // Buffer vert and index data.
    // pos & uv
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);
    // pos index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices), indices, GL_DYNAMIC_DRAW);
    
    // Unbind VBO _ONLY_
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Filter::~Filter() {
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);
}

void Filter::bind() {
    bindFramebuffer(mInputFramebuffer);
}

 void Filter::unbind() {
    unbindFramebuffer(mInputFramebuffer);
}

void Filter::registerCommonShaderUniforms(Shader &shader) {
    assert(shader.registerUniform("screenQuadTex", UNIFORM_KEY_0));
}

void Filter::setCommonShaderUniforms(Shader &shader) {
    // Using explicit uniform loc: texture at loc 0 -> (tex unit 0)
    //mShader->setUniformAtLoc<GLuint>(0, 0);
     shader.setUniform<GLuint>(UNIFORM_KEY_0, 0);

}
void Filter::bindFramebuffer(Framebuffer &fb) {
    fb.bind();

    // call glViewport adjust resolution to framebuffer size.
    glViewport(0, 0, (GLsizei) mResolution.x, (GLsizei) mResolution.y);
}

void Filter::unbindFramebuffer(Framebuffer &fb) {
    fb.unbind();
}

void Filter::renderFramebuffer(Framebuffer &fb, Shader &shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb.getGLTexture());

    shader.bind();

    setCommonShaderUniforms(shader);
    glBindVertexArray(mVAO);

    updateParameters();

    // Render framebuffer quad
    // attributes: pos, texture coord, uniforms: none
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//void Filter::process() {
//
//}


