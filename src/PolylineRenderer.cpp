/*
 * PolylineRenderer.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: Osama Arafa
 */

#include "PolylineRenderer.h"
#include "Polyline.h"
#include "helpers.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <cstddef>

using namespace std;

PolylineRenderer::PolylineRenderer(Shader &shader, const glm::vec2 &resolution)
        : mShader(&shader) {
    glGenVertexArrays(1, &mVAO);
    checkGLErrors(__FILE__, __LINE__);
    glGenBuffers(1, &mVBO);
    checkGLErrors(__FILE__, __LINE__);

    ///////////////////////////////////////////////////////////////////////////
    shader.bind();
    glBindVertexArray(mVAO);
    checkGLErrors(__FILE__, __LINE__);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    checkGLErrors(__FILE__, __LINE__);

    if (!mShader->registerUniform("mvp", UniformMVP))
        cerr << "Error: Failed to register uniform: mvp" << endl;
    checkGLErrors(__FILE__, __LINE__);

    if (!mShader->registerUniform("plColor", UniformPLColor))
        cerr << "Error: Failed to register uniform: plColor" << endl;
    checkGLErrors(__FILE__, __LINE__);

    // Attribute locations.
    GLuint posAttrib = glGetAttribLocation(shader.getId(), "vertPos");
    checkGLErrors(__FILE__, __LINE__);

    // Set attribute pointers.
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                          2*sizeof(GLfloat), NULL);
    checkGLErrors(__FILE__, __LINE__);

    // Enable attributes
    glEnableVertexAttribArray(posAttrib);
    checkGLErrors(__FILE__, __LINE__);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    shader.unbind();
    glBindVertexArray(0);
    ///////////////////////////////////////////////////////////////////////////

    mProj = glm::ortho(0.0f, resolution.x, resolution.y, 0.0f, -1.0f, 1.0f);
}

PolylineRenderer::~PolylineRenderer() {
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
}

void PolylineRenderer::render(Polyline& pl, const glm::mat4 &view) {
    //renderNonTransformed(pl, view);
    renderTransformed(pl, view);
}

void PolylineRenderer::begin() {
	mShader->bind();
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindVertexArray(mVAO);
}

void PolylineRenderer::end() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	mShader->unbind();
}

void PolylineRenderer::renderNonTransformed(Polyline & pl, const glm::mat4 & view) {
    size_t nPoint = pl.points.size();
    if (nPoint < 2) return;
    size_t nSeg = pl.closed? nPoint : nPoint-1;
    size_t nVert = pl.closed? nSeg*4 + 2 : nSeg*4;
    vector<GLfloat> v(nVert * 2);
    //GLfloat v[nVert*2];
    glm::vec2 v1, v2, v3, v4;

    for (size_t i = 0; i < nSeg; i++) {
        size_t iNext = (i+1)%nPoint;
        glm::vec2 dir(glm::normalize(pl.points[iNext] - pl.points[i]));
        glm::vec2 norm(-dir.y, dir.x);
        const glm::vec2 &pi = pl.points[i];
        const glm::vec2 &pn = pl.points[iNext];

        v1 = pi - norm*(float)pl.width/2.0f;
        v2 = pi + norm*(float)pl.width/2.0f;
        v3 = pn - norm*(float)pl.width/2.0f;
        v4 = pn + norm*(float)pl.width/2.0f;

        v[i*8]     = v1.x; v[i*8+1]   = v1.y;
        v[i*8+2]   = v2.x; v[i*8+3]   = v2.y;
        v[i*8+4]   = v3.x; v[i*8+5]   = v3.y;
        v[i*8+6]   = v4.x; v[i*8+7]   = v4.y;
    }

    if (pl.closed) {
        // close last cap
        glm::vec2 dir(glm::normalize(pl.points[1] - pl.points[0]));
        glm::vec2 norm(-dir.y, dir.x);
        const glm::vec2 &pi = pl.points[0];
        const glm::vec2 &pn = pl.points[1];
        v1 = pl.points[0] - norm*(float)pl.width/2.0f;
        v2 = pl.points[0] + norm*(float)pl.width/2.0f;
        v[nVert*2-4] = v1.x; v[nVert*2-3] = v1.y;
        v[nVert*2-2] = v2.x; v[nVert*2-1] = v2.y;
    }

    glm::vec3 roshift(pl.pivot(), 0.0f);
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(pl.pos, 0.0));
//    model = glm::translate(model, glm::vec3(pl.pos, 0.0f));
    model = glm::translate(model, roshift);
    model = glm::rotate(model, (float)pl.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3((float)pl.scale));
    model = glm::translate(model, -1.0f*roshift);

    glm::mat4 mvp = mProj*view*model;     // polyline model transform is identity

    mShader->setUniform<glm::mat4>(UniformMVP, mvp);
    mShader->setUniform<glm::vec4>(UniformPLColor, pl.color);

    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(GLfloat), &v[0], GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, nVert);
}

void PolylineRenderer::renderTransformed(Polyline & pl, const glm::mat4 & view) {
    size_t nPoint = pl.points.size();
    if (nPoint < 2) return;
    size_t nSeg = pl.closed? nPoint : nPoint-1;
    size_t nVert = pl.closed? nSeg*4 + 2 : nSeg*4;
    vector<GLfloat> v(nVert * 2);
    //GLfloat v[nVert*2];
    glm::vec2 v1, v2, v3, v4;


    glm::vec3 roshift(pl.pivot(), 0.0f);
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(pl.pos, 0.0));
//    model = glm::translate(model, glm::vec3(pl.pos, 0.0f));
    model = glm::translate(model, roshift);
    model = glm::rotate(model, (float)-pl.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3((float)pl.scale));
    model = glm::translate(model, -1.0f*roshift);

    // TODO: Apply model transform.

    glm::mat4 mvp = mProj*view*glm::mat4(1.0f);

    for (size_t i = 0; i < nSeg; i++) {
        size_t iNext = (i+1)%nPoint;

        glm::vec2 pi = model*glm::vec4(pl.points[i], 0.0, 1.0);
        glm::vec2 pn = model*glm::vec4(pl.points[iNext], 0.0, 1.0);

        glm::vec2 dir(glm::normalize(pn - pi));
        glm::vec2 norm(-dir.y, dir.x);

        v1 = pi - norm*(float)pl.width/2.0f;
        v2 = pi + norm*(float)pl.width/2.0f;
        v3 = pn - norm*(float)pl.width/2.0f;
        v4 = pn + norm*(float)pl.width/2.0f;

        v[i*8]     = v1.x; v[i*8+1]   = v1.y;
        v[i*8+2]   = v2.x; v[i*8+3]   = v2.y;
        v[i*8+4]   = v3.x; v[i*8+5]   = v3.y;
        v[i*8+6]   = v4.x; v[i*8+7]   = v4.y;
    }

    if (pl.closed) {
        // close last cap

        glm::vec2 p0 = model*glm::vec4(pl.points[0], 0.0, 1.0);
        glm::vec2 p1 = model*glm::vec4(pl.points[1], 0.0, 1.0);

        glm::vec2 dir(glm::normalize(p1 - p0));
        glm::vec2 norm(-dir.y, dir.x);
        v1 = p0 - norm*(float)pl.width/2.0f;
        v2 = p0 + norm*(float)pl.width/2.0f;
        v[nVert*2-4] = v1.x; v[nVert*2-3] = v1.y;
        v[nVert*2-2] = v2.x; v[nVert*2-1] = v2.y;
    }

    mShader->setUniform<glm::mat4>(UniformMVP, mvp);
    mShader->setUniform<glm::vec4>(UniformPLColor, pl.color);

    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(GLfloat), &v[0], GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, nVert);
}
