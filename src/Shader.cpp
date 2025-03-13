/*
 * Shader.cpp
 *
 *  Created on: Oct 29, 2016
 *      Author: Osama Arafa
 */

#include "helpers.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <exception>

using namespace std;

template <typename T>
class AutoDelete {
public:
    AutoDelete(T *ptr) : m_ptr(ptr) { }
    ~AutoDelete() { delete m_ptr; }
private:
    T *m_ptr;
};

std::string readTextFile(const char *fileName) {
    ifstream file;
    stringstream ss;
    file.open(fileName, ios::in);
    if (!file.is_open())
    	throw("failed to open file");
    ss << file.rdbuf();
    string retval = ss.str();
    return retval;
}

Shader::Shader(const char *vsFileName, const char *fsFileName) : mShaderProg(0) {
    int success;

    string vsStr = readTextFile(vsFileName);
    string fsStr = readTextFile(fsFileName);

    GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar *vsCStr = vsStr.c_str();
    const GLchar *fsCStr = fsStr.c_str();
    glShaderSource(vertShaderId, 1, &vsCStr, 0);
    glShaderSource(fragShaderId, 1, &fsCStr, 0);

    glCompileShader(vertShaderId);
    checkGLErrors(__FILE__, __LINE__);
    glCompileShader(fragShaderId);
    checkGLErrors(__FILE__, __LINE__);

    glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &success);
    checkGLErrors(__FILE__, __LINE__);
    if (success != GL_TRUE) {
        reportOpenGLError(vertShaderId,
                ErrorTargetShader,
                "ERROR::SHADER::VERTEX::COMPILATION_FAILED:");
    }

    glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &success);
    checkGLErrors(__FILE__, __LINE__);
    if (success != GL_TRUE) {
        reportOpenGLError(fragShaderId,
                ErrorTargetShader,
                "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:");
    }

    mShaderProg = glCreateProgram();
    glAttachShader(mShaderProg, vertShaderId);
    checkGLErrors(__FILE__, __LINE__);
    glAttachShader(mShaderProg, fragShaderId);
    checkGLErrors(__FILE__, __LINE__);

    glLinkProgram(mShaderProg);
    checkGLErrors(__FILE__, __LINE__);

    glGetProgramiv(mShaderProg, GL_LINK_STATUS, &success);
    checkGLErrors(__FILE__, __LINE__);
    if (success != GL_TRUE) {
        reportOpenGLError(mShaderProg,
                ErrorTargetProgram,
                "ERROR::SHADER::PROGRAM::LINKING_FAILED:");
    }

//    if (!validate()) {
//        reportOpenGLError(mShaderProg,
//                ErrorTargetProgram,
//                "ERROR::SHADER::PROGRAM::VALIDATION_FAILED:");
//    }

    // Mark shaders for deletion as soon as program is deleted
    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);
}

Shader::~Shader() {
    glDeleteProgram(mShaderProg);
}

void Shader::bind() {
    glUseProgram(mShaderProg);
}

void Shader::unbind() {
    glUseProgram(0);
}

bool Shader::isValid() {
    return mShaderProg > 0;
}

bool Shader::validate() {
    GLint success;

    glValidateProgram(mShaderProg);
    glGetShaderiv(mShaderProg, GL_VALIDATE_STATUS, &success);
    checkGLErrors(__FILE__, __LINE__);

//    if (success != GL_TRUE) {
//        reportOpenGLError(mShaderProg,
//                ErrorTargetProgram,
//                "ERROR::SHADER::PROGRAM::PROGRAM_VALIDATION_FAILED:");
//        return false;
//    }
    return true;
}

GLuint Shader::GetUniformId(const GLchar* name) {
	return glGetUniformLocation(mShaderProg, name);
}

GLuint Shader::GetUniformId(const unsigned int key) {
	UniformMapIter it = mUniforms.find(key);
	if (it == mUniforms.end()) return 0;
	return it->second;
}

void Shader::reportOpenGLError(GLuint target,
        OpenGLErrorTarget targetType,
        const char *msg)
{
    int infoLogLen = 0;
    vector<char> strBuffer(256);

    switch (targetType) {
    case ErrorTargetShader:
        glGetShaderiv(target, GL_INFO_LOG_LENGTH, &infoLogLen);
        checkGLErrors(__FILE__, __LINE__);
        strBuffer.reserve(infoLogLen);
        glGetShaderInfoLog(target, infoLogLen, NULL, &strBuffer[0]);
        break;
    case ErrorTargetProgram:
        glGetProgramiv(target, GL_INFO_LOG_LENGTH, &infoLogLen);
        strBuffer.reserve(infoLogLen);
        glGetProgramInfoLog(target, infoLogLen, NULL, &strBuffer[0]);
        break;
    default:
    	break;
    }

    string infoLog(&strBuffer[0], infoLogLen);
    std::cout << msg
              << infoLog
              << std::endl;
}

bool Shader::registerUniform(const GLchar *name, const unsigned int key) {
	int id = glGetUniformLocation(mShaderProg, name);
	if (id == -1) return false;
    mUniforms[key] = id;
    return true;
}

void Shader::sendUniformData(const GLuint id, const GLint &value, const bool transpose) {
    glUniform1i(id, value);
}

void Shader::sendUniformData(const GLuint id, const GLfloat &value, const bool transpose) {
    glUniform1f(id, value);
}

void Shader::sendUniformData(const GLuint id, const glm::vec2 &value, const bool transpose) {
    glUniform2f(id, value[0], value[1]);
}

void Shader::sendUniformData(const GLuint id, const glm::vec3 &value, const bool transpose) {
    glUniform3f(id, value[0], value[1], value[2]);
}

void Shader::sendUniformData(const GLuint id, const glm::vec4 &value, const bool transpose) {
    glUniform4f(id, value[0], value[1], value[2], value[3]);
}

void Shader::sendUniformData(const GLuint id, const GLuint &value, const bool transpose) {
    glUniform1i(id, value);
}

void Shader::sendUniformData(const GLuint id, const glm::ivec2 &value, const bool transpose) {
    glUniform2i(id, value[0], value[1]);
}

void Shader::sendUniformData(const GLuint id, const glm::ivec3 &value, const bool transpose) {
    glUniform3i(id, value[0], value[1], value[2]);
}

void Shader::sendUniformData(const GLuint id, const glm::ivec4 &value, const bool transpose) {
    glUniform4i(id, value[0], value[1], value[2], value[3]);
}

void Shader::sendUniformData(const GLuint id, const glm::mat2 &value, const bool transpose) {
    glUniformMatrix2fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat3 &value, const bool transpose) {
    glUniformMatrix3fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat4 &value, const bool transpose) {
    glUniformMatrix4fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat2x3 &value, const bool transpose) {
    glUniformMatrix2x3fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat3x2 &value, const bool transpose) {
    glUniformMatrix3x2fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat2x4 &value, const bool transpose) {
    glUniformMatrix2x4fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat4x2 &value, const bool transpose) {
    glUniformMatrix4x2fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat3x4 &value, const bool transpose) {
    glUniformMatrix3x4fv(id, 1, transpose, glm::value_ptr(value));
}

void Shader::sendUniformData(const GLuint id, const glm::mat4x3 &value, const bool transpose) {
    glUniformMatrix4x3fv(id, 1, transpose, glm::value_ptr(value));
}

// Array types
void Shader::sendArrayUniformData(const GLuint id, const GLfloat *ptr, const int size, const bool transpose) {
    glUniform1fv(id, size, ptr);
}

GLuint Shader::getId() {
	return mShaderProg;
}
//void Shader::reportOpenGLError(GLuint target,
//        OpenGLErrorTarget targetType,
//        const char *msg)
//{
//    int infoLogLen;
//    vector<char> strBuffer;
//    GLboolean (*getParamFn)(GLuint, GLenum, GLint *);
//    GLboolean (*getInfoLogFn)(GLuint, GLenum, GLint *);
//
//    switch (targetType) {
//    case ErrorTargetShader:
//        getParamFn = &glGetShaderiv;
//        getInfoLogFn = &glGetShaderInfoLog;
//        break;
//    case ErrorTargetProgram:
//        getParamFn = &glGetProgramiv;
//        getInfoLogFn = &glGetProgramInfoLog;
//        break;
//    default:
//    }
//
//    getParamFn(target, GL_INFO_LOG_LENGTH, &infoLogLen);
//    getInfoLogFn(target, infoLogLen, NULL, strBuffer);
//
//    string infoLog(&strBuffer[0], infoLogLen);
//    std::cout << msg
//              << infoLog
//              << std::endl;
//}
