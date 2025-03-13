#ifndef COLORSHIFTFILTER_H_
#define COLORSHIFTFILTER_H_


#include "Filter.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

class ColorShiftFilter : public Filter {
public:
    ColorShiftFilter(Shader &shader, const glm::vec2 &resolution);
    ~ColorShiftFilter();

    //virtual void bind();
    //virtual void unbind();

    virtual void renderContent();
    void init(double radius, double rotation=0.0);
    void update(int dt);
private:
    Shader *mShader;
    glm::dvec2 mLastOffset;
    glm::dvec2 mNextOffset;
    glm::dvec2 mCurrOffset;
    glm::dvec2 mDirection;
    bool mSwingDir;
    double mRadius;
    int mElapsed;   // since last offset
    double mFreq;

    void pickNextOffset();
};


#endif /* COLORSHIFTFILTER_H_ */
