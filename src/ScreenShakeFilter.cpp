#include "ScreenShakeFilter.h"
#include "helpers.h"
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

ScreenShakeFilter::ScreenShakeFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader)
        , mOffset(0.0, 0.0)
        , mTargetOffset(0.0, 0.0)
        , mDuration(1000)
        , mElapsed(0)
        , mFreq(2.0) {
    registerCommonShaderUniforms(*mShader);
}

ScreenShakeFilter::~ScreenShakeFilter() {

}

void ScreenShakeFilter::renderContent() {
    renderFramebuffer(mInputFramebuffer, *mShader);
}

void ScreenShakeFilter::init(double radius) {
    if (radius > mRadius) mRadius = radius;
    mElapsed = 0;
}

void ScreenShakeFilter::update(int dt) {
    glm::dvec2 dirNorm = glm::normalize(mTargetOffset - mOffset);
    mOffset = mOffset + dirNorm*mSpeed;
    mElapsed += dt;
    if (mOffset == mTargetOffset) {
        double decay = mElapsed/mDuration;
        pickTargetOffset(mRadius*(1.0-decay));
    }
}

void ScreenShakeFilter::pickTargetOffset(double radius) {
    double angle = randRangeNaive(0.0, 1.0);
    mTargetOffset = radius * glm::dvec2(glm::cos(angle), glm::sin(angle));
    double dist = glm::length(mTargetOffset - mOffset);
    mSpeed = dist / (1000.0/mFreq);
}

