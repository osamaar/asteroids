/*
 * Polyline.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: Osama Arafa
 */

#include "Polyline.h"
#include "PolylineRenderer.h"
#include <glm/glm.hpp>
#include <vector>

Polyline::Polyline(bool closed)
		: points()
		, color()
		, width(1.0)
		, closed(closed)
		, pos(0.0f)
		, scale(1.0)
		, rotation(0.0)
		, mPivot(PivotOrigin) {
	// TODO Auto-generated constructor stub
}

Polyline::~Polyline() {
	// TODO Auto-generated destructor stub
}

void Polyline::addPoint(double x, double y) {
	points.push_back(glm::vec2(x, y));
}

void Polyline::render(PolylineRenderer& renderer, const glm::mat4 &view) {
    renderer.render(*this, view);
}

void Polyline::PivotOn(enum PivotPoint pp) {
    mPivot = pp;
}

glm::vec2 Polyline::pivot() {
    switch (mPivot) {
    case PivotOrigin:
        return glm::vec2(0.0f);
    case PivotPoint0:
        return points[0];
    case PivotCG:
    {
        size_t n = points.size();
        glm::vec2 total(0.0f);
        for (size_t i = 0; i < n; i++) {
            total += points[i];
        }
        return total/(float)n;
    }
    default:
        return glm::vec2(0.0f);
    }
}
