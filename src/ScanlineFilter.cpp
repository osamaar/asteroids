#include "ScanlineFilter.h"
#include "helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

ScanlineFilter::ScanlineFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader)
        , mChromAbb(0.001f)
        , mVignette(0.4f) {
    registerCommonShaderUniforms(*mShader);
    mShader->registerUniform("chromAbb", 1);
    mShader->registerUniform("vignette", 2);
}

ScanlineFilter::~ScanlineFilter() {

}

void ScanlineFilter::renderContent() {
    mShader->bind();
    setCommonShaderUniforms(*mShader);
    mShader->setUniform<float>(1, mChromAbb);
    mShader->setUniform<float>(2, mVignette);
    renderFramebuffer(mInputFramebuffer, *mShader);
}
