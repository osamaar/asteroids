#ifndef PASSTHROUGHFILTER_H_
#define PASSTHROUGHFILTER_H_


#include "Filter.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

// Copies input to output.
class PassthroughFilter : public Filter {
public:
    PassthroughFilter(Shader &shader, const glm::vec2 &resolution);
    ~PassthroughFilter();

    //virtual void bind();
    //virtual void unbind();
    virtual void renderContent();
private:
    Shader *mShader;
};


#endif /* PASSTHROUGHFILTER_H_ */
