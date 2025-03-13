#include "PassthroughFilter.h"
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

PassthroughFilter::PassthroughFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader) {
    registerCommonShaderUniforms(*mShader);
}

PassthroughFilter::~PassthroughFilter() {

}


void PassthroughFilter::renderContent() {
    renderFramebuffer(mInputFramebuffer, *mShader);
}

