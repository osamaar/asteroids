#include "ScreenShakeFilter.h"
#include "helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

ScreenShakeFilter::ScreenShakeFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader)
        , mLastOffset(0.0, 0.0)
        , mNextOffset(0.0, 0.0)
        , mCurrOffset(0.0, 0.0)
        , mRadius(0.0)
        , mElapsed(0)
        , mFreq(20.0) {
    registerCommonShaderUniforms(*mShader);
    mShader->registerUniform("offset", 1);
}

ScreenShakeFilter::~ScreenShakeFilter() {

}

void ScreenShakeFilter::renderContent() {
    mShader->bind();
    setCommonShaderUniforms(*mShader);
    mShader->setUniform<glm::vec2>(1, mCurrOffset);
    //mShader->setUniform<glm::vec2>(1, glm::vec2(0.0));
    renderFramebuffer(mInputFramebuffer, *mShader);
}

void ScreenShakeFilter::init(double radius) {
    if (radius > mRadius) {
        mRadius = radius;
        pickNextOffset();
        //mElapsed = 0;
    }
}

void ScreenShakeFilter::update(int dt) {
    mElapsed += dt;
    double waveLen = 1000.0/mFreq;

    // Pick a new target
    if (mElapsed >= waveLen) {
        pickNextOffset();
    }

    // Move towards next offset
    mCurrOffset = glm::mix(mLastOffset, mNextOffset, mElapsed/waveLen);

    mRadius -= 0.1*dt/1000.0;

    // Reached target
    if (mRadius <= 0.0) {
        mRadius = 0.0;
        mNextOffset = glm::dvec2(0.0, 0.0);
    }
}

void ScreenShakeFilter::pickNextOffset() {
    mLastOffset = mCurrOffset;
    mElapsed = 0;
    double angle = randRangeNaive(0.0, 2*glm::pi<double>());
    mNextOffset = mRadius * glm::dvec2(glm::cos(angle), glm::sin(angle));
    double dist = glm::length(mNextOffset - mLastOffset);
}

