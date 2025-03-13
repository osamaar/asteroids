#include "AddFilter.h"
#include "helpers.h"
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

AddFilter::AddFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader)
        , mFactor(1.0)
        , mAddFramebuffer(resolution) {
    mShader->bind();
    registerCommonShaderUniforms(*mShader);
    mShader->registerUniform("addTex", 1);
    mShader->registerUniform("factor", 2);
    mShader->unbind();
}

AddFilter::~AddFilter() {

}


void AddFilter::renderContent() {
    bool success;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mAddFramebuffer.getGLTexture());
    checkGLErrors(__FILE__, __LINE__);

    mShader->bind();
    success = mShader->setUniform<GLuint>(1, 1);
    success = mShader->setUniform<GLfloat>(2, mFactor);
    checkGLErrors(__FILE__, __LINE__);

    renderFramebuffer(mInputFramebuffer, *mShader);
}

void AddFilter::setFactor(float factor) {
    mFactor = factor;
}

void AddFilter::bindAddFramebuffer() {
    bindFramebuffer(mAddFramebuffer);
}

