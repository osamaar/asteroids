#ifndef GENERICFRAMEBUFFE_H_
#define GENERICFRAMEBUFFE_H_


#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

class Shader;

class GenericFramebuffer {
public:
    virtual ~GenericFramebuffer() { }

    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual GLuint getGLTexture() = 0;
};

#endif /* FRAMEBUFFE_H_ */
