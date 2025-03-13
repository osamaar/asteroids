/*
 * helpers.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: Osama Arafa
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <string>

using namespace std;

void checkGLErrors(const char *file, int line) {
    GLenum err (glGetError());

    while(err!=GL_NO_ERROR) {
            string error;

            switch(err) {
                    case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                    case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                    case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                    case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
            }

            cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<endl;
            err=glGetError();
    }
}
