#include "BlurFilter.h"
#include "Framebuffer.h"
#include "GenericFramebuffer.h"
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>

// Syntactic sugar!
const unsigned int UNIFORM_KEY_1 = 1;
const unsigned int UNIFORM_KEY_2 = 2;

using namespace std;

BlurFilter::BlurFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader)
        , mAlterFramebuffer(resolution)
        , mCurrentTarget(&mInputFramebuffer)
        , mIterations(1) {
    mShader->bind();
    registerCommonShaderUniforms(*mShader);
    mShader->registerUniform("resolution", UNIFORM_KEY_1);
    mShader->registerUniform("direction", UNIFORM_KEY_2);
    mShader->unbind();
}

BlurFilter::~BlurFilter() {

}

void BlurFilter::unbind() {
    mCurrentTarget->unbind();
}

void BlurFilter::process() {
    mInputFramebuffer.resolve();
    mInputFramebuffer.bind();
    // IMPORTANT: Point to input framebuffer as active framebuffer.
    mCurrentTarget = &mInputFramebuffer;

    // Set up.
    glViewport(0, 0, (GLsizei) mResolution.x, (GLsizei) mResolution.y);
    glDisable(GL_DEPTH_TEST);

    mShader->bind();    // Bind only once since all draw calls use it.
    setCommonShaderUniforms(*mShader);
    // resolution
    mShader->setUniform<glm::vec2>(UNIFORM_KEY_1, mResolution);

    // Fully blur n-1 times. For mIterations=1, this loop never executes.
    for (int i = 0; i < mIterations-1; i++) {
        // set direction uniform to (1.0, 0.0) -> horizontal pass
        mShader->setUniform<glm::vec2>(UNIFORM_KEY_2, glm::vec2(1.0, 0.0));
        renderPass();

        // set direction uniform to (0.0, 1.0) -> vertical pass
        mShader->setUniform<glm::vec2>(UNIFORM_KEY_2, glm::vec2(0.0, 1.0));
        renderPass();
    }

    // set direction uniform to (1.0, 0.0) -> horizontal pass
    mShader->setUniform<glm::vec2>(UNIFORM_KEY_2, glm::vec2(1.0, 0.0));
    renderPass();
}

void BlurFilter::renderContent() {
    mShader->bind();    // Bind only once since all draw calls use it.
    setCommonShaderUniforms(*mShader);
    // resolution
    mShader->setUniform<glm::vec2>(UNIFORM_KEY_1, mResolution);

    // render one last vertical pass to default framebuffer
    // set direction uniform to (0.0, 1.0) -> vertical pass
    mShader->setUniform<glm::vec2>(UNIFORM_KEY_2, glm::vec2(0.0, 1.0));
    renderFramebuffer(*mCurrentTarget, *mShader);
}

GLuint BlurFilter::getGLTexture() {
    return mCurrentTarget->getGLTexture();
}

void BlurFilter::alternateFramebuffers() {
    mCurrentTarget->unbind();

    if (mCurrentTarget == &mInputFramebuffer) {
        mCurrentTarget = &mAlterFramebuffer;
    } else {
        mCurrentTarget = &mInputFramebuffer;
    }

    mCurrentTarget->bind();
}

void BlurFilter::renderPass() {
    GenericFramebuffer *oldTarget = mCurrentTarget;

    // Switch to the other framebuffer
    alternateFramebuffers();

    // Render old framebuffer to new framebuffer
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    renderFramebuffer(*oldTarget, *mShader);
}


void BlurFilter::setIterations(int n) {
    mIterations = max(1, n);
}

