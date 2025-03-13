#ifndef MULTISAMPLEFRAMEBUFFE_H_
#define MULTISAMPLEFRAMEBUFFE_H_


#include "GenericFramebuffer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

class Shader;

class MultisampleFramebuffer : public GenericFramebuffer {
public:
    MultisampleFramebuffer(const glm::vec2 &resolution, bool useDepthStencil=false);
    ~MultisampleFramebuffer();

    void bind();
    void unbind();
    GLuint getGLTexture();
    void resolve();
private:
    glm::vec2 mResolution;
    GLuint mFBO;
    GLuint mTexColorBuffer;
    GLuint mDepthStencilRBO;    // A render buffer object
    GLuint mResolveFBO;
    GLuint mTexResolveColorBuffer;
    bool mUseDepthStencil;
};

#endif /* MULTISAMPLEFRAMEBUFFE_H_ */
