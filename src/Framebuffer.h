#ifndef FRAMEBUFFE_H_
#define FRAMEBUFFE_H_


#include "GenericFramebuffer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

class Shader;

class Framebuffer : public GenericFramebuffer {
public:
    Framebuffer(const glm::vec2 &resolution, bool useDepthStencil=false);
    ~Framebuffer();

    void bind();
    void unbind();
    GLuint getGLTexture();
private:
    GLuint mFBO;
    GLuint mTexColorBuffer;
    GLuint mDepthStencilRBO;    // A render buffer object
    bool mUseDepthStencil;
};

#endif /* FRAMEBUFFE_H_ */
