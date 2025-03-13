#include "ColorShiftFilter.h"
#include "helpers.h"
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

ColorShiftFilter::ColorShiftFilter(Shader &shader,
        const glm::vec2 &resolution)
        : Filter(resolution)
        , mShader(&shader)
        , mLastOffset(0.0, 0.0)
        , mNextOffset(0.0, 0.0)
        , mCurrOffset(0.0, 0.0)
        , mDirection(1.0, 0.0)
        , mSwingDir(true)
        , mRadius(0.0)
        , mElapsed(0)
        , mFreq(10.0) {
    registerCommonShaderUniforms(*mShader);
    mShader->registerUniform("offset", 1);
}

ColorShiftFilter::~ColorShiftFilter() {

}

void ColorShiftFilter::renderContent() {
    mShader->bind();
    setCommonShaderUniforms(*mShader);
    mShader->setUniform<glm::vec2>(1, mCurrOffset);
    //mShader->setUniform<glm::vec2>(1, glm::vec2(0.0));
    renderFramebuffer(mInputFramebuffer, *mShader);
}

void ColorShiftFilter::init(double radius, double rotation) {
    if (radius > mRadius) {
        mRadius = radius;
        double rotDeg = glm::radians(-rotation);
        mDirection = glm::dvec2(glm::cos(rotDeg), -glm::sin(rotDeg));
        mDirection = glm::normalize(mDirection);
        pickNextOffset();
        //mElapsed = 0;
    }
}

void ColorShiftFilter::update(int dt) {
    mElapsed += dt;
    double waveLen = 1000.0/mFreq;

    // Pick a new target
    if (mElapsed >= waveLen) {
        pickNextOffset();
    }

    // Move towards next offset
    mCurrOffset = glm::mix(mLastOffset, mNextOffset, mElapsed/waveLen);

    mRadius -= 0.01*dt/1000.0;

    // Reached target
    fmt::print("ColorShiftFilter::mRadius: {:}\n", mRadius);
    if (mRadius <= 0.0) {
        mRadius = 0.0;
        mNextOffset = glm::dvec2(0.0, 0.0);
    }
}

void ColorShiftFilter::pickNextOffset() {
    mLastOffset = mCurrOffset;
    mElapsed = 0;

    if (mSwingDir) {
        mNextOffset = mDirection*mRadius;
    } else {
        mNextOffset = -mDirection*mRadius;
    }

    mSwingDir = !mSwingDir;
    //cout << mLastOffset.x << ", " << mLastOffset.y << " - ";
    //cout << mNextOffset.x << ", " << mNextOffset.y << endl;
    //cout << "pick next offset" << endl;
}

