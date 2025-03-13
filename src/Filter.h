#ifndef FILTER_H_
#define FILTER_H_


#include "Framebuffer.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

class Shader;

// Usage:
// pp0.bind()
// renderStuff()
// pp0.process()
// pp0.unbind()
//
// pp1.bind()       // derived
// pp1.setSomeParameters(...) <-- And override updateParameters
// pp0.renderContent()
// pp1.process()
// pp1.unbind()
//
// pp2.bind()
// pp1.renderContent()
// pp1.process()
// pp2.unbind()
//
// prepare mainloop rendering viewport.
// pp2.renderContent() ---> To default framebuffer

// Derive this class to create a concrete filter. Use mInputFramebuffer and add
// more fremebuffers for different filtering stages in derived class as needed.
// Usable as-is as a one-stage shader. But it's better to derive it for the
// specific use.
// Override unbind(), Process(), and renderContent().
// All derived classes must call registerCommonShaderUniforms() on every new
// shader and setCommonShaderUniforms() before every draw call.
class Filter {
public:
    Filter(const glm::vec2 &resolution);
    virtual ~Filter();

    void bind();

    // Should override to unbind output framebuffer (set from process()). In
    // the implest case provided here, it's the same as input buffer, and no
    // override is needed. Use unbindFramebuffer() here.
    virtual void unbind();

    // All filter logic happens here. Bind/unbind framebuffers/shaders as
    // needed and render between them.
    virtual void process() { }

    // Render screen quad texture from our processed FB to whatever FB is
    // active now. Use renderFramebuffer() here. Simplest case is provided.
    virtual void renderContent() = 0;
    
    //void process();
protected:
    glm::vec2 mResolution;
    Framebuffer mInputFramebuffer;

    // Registers framebuffer color texture sampler uniform with key=0.
    // Can include more uniforms but would require revising all derived classes
    // to make sure they don't use the new uniform keys.
    void registerCommonShaderUniforms(Shader &shader);
    void setCommonShaderUniforms(Shader &shader);

    void bindFramebuffer(Framebuffer &fb);
    void unbindFramebuffer(Framebuffer &fb);

    // This is a noop: base class has no param to update
    // Called from renderContents(). Override for own logic.
    virtual void updateParameters() { }

    // Render screen quad texture associated with a given framebuffer to
    // whatever FB is active now. Uses explicit uniform locations. Loc 0 is
    // screen quad texture.
    void renderFramebuffer(Framebuffer &fb, Shader &shader);
private:
    GLuint mVAO, mVBO, mEBO;
};


#endif /* FILTER_H_ */
