#include "helpers.h"
#include "Framebuffer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <iostream>

using namespace std;

Framebuffer::Framebuffer(const glm::vec2 &resolution, bool useDepthStencil)
        : mUseDepthStencil(useDepthStencil) {
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    // Generate and bind a color texture
    glGenTextures(1, &mTexColorBuffer);
    glBindTexture(GL_TEXTURE_2D, mTexColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) resolution.x, (GLsizei) resolution.y,
            0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, mTexColorBuffer, 0);

    // Generate and bind a depth/stencil render buffer object
    if (useDepthStencil) {
        glGenRenderbuffers(1, &mDepthStencilRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                (GLsizei) resolution.x, (GLsizei) resolution.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Attach RBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER, mDepthStencilRBO);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "Error constructing Framebuffer!" << endl;

    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

Framebuffer::~Framebuffer() {
    unbind();
    glDeleteTextures(1, &mTexColorBuffer);
    if (mUseDepthStencil) {
        glDeleteRenderbuffers(1, &mDepthStencilRBO);
    }
    glDeleteFramebuffers(1, &mFBO);
}

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);  
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

GLuint Framebuffer::getGLTexture() {
    return mTexColorBuffer;
}
