#include "helpers.h"
#include "MultisampleFramebuffer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <iostream>

using namespace std;

MultisampleFramebuffer::MultisampleFramebuffer(const glm::vec2 &resolution, bool useDepthStencil)
        : mResolution(resolution)
        , mUseDepthStencil(useDepthStencil) {
    int numSamples = 8;
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    // Generate and bind a color texture
    glGenTextures(1, &mTexColorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTexColorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GL_RGBA8,
        (GLsizei) resolution.x, (GLsizei) resolution.y, true);
    checkGLErrors(__FILE__, __LINE__);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D_MULTISAMPLE, mTexColorBuffer, 0);

    // Generate and bind a depth/stencil render buffer object
    if (useDepthStencil) {
        glGenRenderbuffers(1, &mDepthStencilRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilRBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, GL_DEPTH24_STENCIL8,
                (GLsizei) resolution.x, (GLsizei) resolution.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Attach RBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER, mDepthStencilRBO);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "Error constructing MultisampleFramebuffer!" << endl;

    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  



    glGenFramebuffers(1, &mResolveFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mResolveFBO);

    // Generate and bind a color texture
    glGenTextures(1, &mTexResolveColorBuffer);
    glBindTexture(GL_TEXTURE_2D, mTexResolveColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) resolution.x, (GLsizei) resolution.y,
            0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    checkGLErrors(__FILE__, __LINE__);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, mTexResolveColorBuffer, 0);
    checkGLErrors(__FILE__, __LINE__);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "Error constructing resolve Framebuffer!" << endl;

    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  





}

MultisampleFramebuffer::~MultisampleFramebuffer() {
    unbind();
    glDeleteTextures(1, &mTexColorBuffer);
    glDeleteTextures(1, &mTexResolveColorBuffer);
    if (mUseDepthStencil) {
        glDeleteRenderbuffers(1, &mDepthStencilRBO);
    }
    glDeleteFramebuffers(1, &mFBO);
    glDeleteFramebuffers(1, &mResolveFBO);
}

void MultisampleFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);  
}

void MultisampleFramebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

GLuint MultisampleFramebuffer::getGLTexture() {
    return mTexResolveColorBuffer;
}

void MultisampleFramebuffer::resolve() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mResolveFBO);
    glBlitFramebuffer(0, 0, (GLint)mResolution.x, (GLint)mResolution.y, 0, 0,
        (GLint)mResolution.x, (GLint)mResolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    checkGLErrors(__FILE__, __LINE__);
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

