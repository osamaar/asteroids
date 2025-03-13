#include "ShakeFilter.h"
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

ShakeFilter::ShakeFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader) {
    registerCommonShaderUniforms(*mShader);
}

ShakeFilter::~ShakeFilter() {

}


void ShakeFilter::renderContent() {
    renderFramebuffer(mInputFramebuffer, *mShader);
}

