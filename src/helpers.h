/*
 * helpers.h
 *
 *  Created on: Nov 1, 2016
 *      Author: Osama Arafa
 */

#ifndef HELPERS_H_
#define HELPERS_H_


#include <glm/gtc/constants.hpp>

const double twoPi = glm::pi<double>() * 2;

void checkGLErrors(const char *file, int line);
double randRangeNaive(double start, double end);


#endif /* HELPERS_H_ */
