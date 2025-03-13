#ifndef ADDFILTER_H_
#define ADDFILTER_H_


#include "Filter.h"
#include "Shader.h"
#include "Framebuffer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

// Copies input to output.
class AddFilter : public Filter {
public:
    AddFilter(Shader &shader, const glm::vec2 &resolution);
    ~AddFilter();

    //virtual void bind();
    //virtual void unbind();
    virtual void renderContent();
    void setFactor(float factor);
    void bindAddFramebuffer();
private:
    Shader *mShader;
    GLfloat mFactor;
    Framebuffer mAddFramebuffer;
};


#endif /* ADDFILTER_H_ */
