#include "AddFilter.h"
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

AddFilter::AddFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader)
        , mFactor(1.0) {
    registerCommonShaderUniforms(*mShader);
    mShader->registerUniform("factor", 1);
}

AddFilter::~AddFilter() {

}


void AddFilter::renderContent() {
    mShader->setUniform<GLfloat>(1, mFactor);
    renderFramebuffer(mInputFramebuffer, *mShader);
}

void AddFilter::setFactor(float factor) {
    mFactor = factor;
}

