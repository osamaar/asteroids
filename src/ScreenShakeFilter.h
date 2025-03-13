#ifndef SCREENSHAKEFILTER_H_
#define SCREENSHAKEFILTER_H_


#include "Filter.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>

class ScreenShakeFilter : public Filter {
public:
    ScreenShakeFilter(Shader &shader, const glm::vec2 &resolution);
    ~ScreenShakeFilter();

    //virtual void bind();
    //virtual void unbind();

    virtual void renderContent();
    void init(double radius);
    void update(int dt);
private:
    Shader *mShader;
    glm::dvec2 mOffset;
    glm::dvec2 mTargetOffset;
    double mRadius;
    int mDuration;
    int mElapsed;
    double mFreq;
    double mSpeed;

    void pickTargetOffset(double radius);
};


#endif /* SCREENSHAKEFILTER_H_ */
