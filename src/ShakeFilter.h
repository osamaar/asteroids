#ifndef SHAKEFILTER_H_
#define SHAKEFILTER_H_


#include "Filter.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

// Copies input to output.
class ShakeFilter : public Filter {
public:
    ShakeFilter(Shader &shader, const glm::vec2 &resolution);
    ~ShakeFilter();

    //virtual void bind();
    //virtual void unbind();
    virtual void renderContent();
private:
    Shader *mShader;
};


#endif /* SHAKEFILTER_H_ */
