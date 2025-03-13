#ifndef BLURFILTER_H_
#define BLURFILTER_H_


#include "Filter.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "GenericFramebuffer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

// Perform a 9-tap aussian blur on input using linear sampling. Use 2 ping-pong
// framebuffers to get the desired bluriness.
class BlurFilter : public Filter {
public:
    BlurFilter(Shader &shader, const glm::vec2 &resolution);
    ~BlurFilter();

    virtual void unbind();
    virtual void process();
    virtual void renderContent();
    virtual GLuint getGLTexture();
    void setIterations(int n);
private:
    Shader *mShader;
    Framebuffer mAlterFramebuffer;
    GenericFramebuffer *mCurrentTarget;
    int mIterations;

    void alternateFramebuffers();
    void renderPass();
};


#endif /* BLURFILTER_H_ */
