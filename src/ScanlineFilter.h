#ifndef SCANLINEFILTER_H_
#define SCANLINEFILTER_H_


#include "Filter.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

class ScanlineFilter : public Filter {
public:
    float mChromAbb;
    float mVignette;

    ScanlineFilter(Shader &shader, const glm::vec2 &resolution);
    ~ScanlineFilter();

    //virtual void bind();
    //virtual void unbind();

    virtual void renderContent();
private:
    Shader *mShader;
};


#endif /* SCANLINEFILTER_H_ */
