/*
 * PolylineRenderer.h
 *
 *  Created on: Nov 7, 2016
 *      Author: Osama Arafa
 */

#ifndef POLYLINERENDERER_H_
#define POLYLINERENDERER_H_

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

struct Polyline;
class Shader;

class PolylineRenderer {
public:

	PolylineRenderer(Shader &shader, const glm::vec2 &resolution);
	virtual ~PolylineRenderer();

	void render(Polyline &pl, const glm::mat4 &view=glm::mat4(1.0f));
    void begin();
    void end();
private:
	enum Uniforms {
		UniformMVP,
		UniformPLColor,
	};

	GLuint mVAO, mVBO;	//, mEBO;
	glm::mat4 mProj;
	Shader *mShader;

	PolylineRenderer();
	PolylineRenderer(const PolylineRenderer &other);
	PolylineRenderer &operator=(const PolylineRenderer &other);

	void renderNonTransformed(Polyline &pl, const glm::mat4 &view=glm::mat4(1.0f));
	void renderTransformed(Polyline &pl, const glm::mat4 &view=glm::mat4(1.0f));
};

#endif /* POLYLINERENDERER_H_ */
