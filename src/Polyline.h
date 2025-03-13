/*
 * Polyline.h
 *
 *  Created on: Nov 7, 2016
 *      Author: Osama Arafa
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "glm/glm.hpp"
#include <vector>

class PolylineRenderer;

struct Polyline {
    enum PivotPoint { PivotOrigin, PivotPoint0, PivotCG };
//    struct Point { double x, y; };

//    std::vector<Point> points;
    std::vector<glm::vec2> points;
    glm::vec4 color;
    double width;

    glm::vec2 pos;
    double scale;
    double rotation;

    bool closed;

    Polyline(bool closed=false);
    virtual ~Polyline();

    void addPoint(double x, double y);
    void render(PolylineRenderer &renderer, const glm::mat4 &view=glm::mat4(1.0));
    void PivotOn(enum PivotPoint pp);
    glm::vec2 pivot();
private:
    enum PivotPoint mPivot;
};

#endif /* POLYLINE_H_ */
