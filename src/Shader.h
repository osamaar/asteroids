/*
 * Shader.h
 *
 *  Created on: Oct 29, 2016
 *      Author: Osama Arafa
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <map>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>


class Shader {
public:
    Shader(const char *vertShaderFileName, const char *fragShaderFileName);
    ~Shader();

    void bind();
    void unbind();
    bool isValid();
    bool validate();
    GLuint GetUniformId(const GLchar *name);
    GLuint GetUniformId(const unsigned int key);
    GLuint getId();

    bool registerUniform(const GLchar *name, const unsigned int key);
    template <typename T>
//    bool setUniform(const char *name, const T &value, const bool transpose=false);
    bool setUniform(const unsigned int key, const T &value, const bool transpose=false);
    template <typename T>
    // For array types.
    // TODO: Implement all types (only GLfloat is implemented atm.)
    bool setArrayUniform(const unsigned int key, const T *ptr, const unsigned int size, bool transpose=false);
    template <typename T>
    bool setUniformAtLoc(const unsigned int location, const T &value, const bool transpose=false);
private:
    enum OpenGLErrorTarget { ErrorTargetProgram, ErrorTargetShader };

    GLuint mShaderProg;
//    std::map<std::string, GLuint> mUniforms;
//    typedef std::map<std::string, GLuint>::iterator UniformMapIter;
    std::map<unsigned int, GLuint> mUniforms;
    typedef std::map<unsigned int, GLuint>::iterator UniformMapIter;

    static void reportOpenGLError(GLuint target,
            OpenGLErrorTarget targetType,
            const char *msg);

    Shader();
    Shader(const Shader &other);
    Shader &operator =(const Shader &other);

    void sendUniformData(const GLuint id, const GLint &value, const bool transpose=false);
    void sendUniformData(const GLuint id, const GLfloat &value, const bool transpose=false);
    void sendUniformData(const GLuint id, const glm::vec2 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::vec3 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::vec4 &value, const bool transpose);

    void sendUniformData(const GLuint id, const GLuint &value, const bool transpose=false);
    void sendUniformData(const GLuint id, const glm::ivec2 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::ivec3 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::ivec4 &value, const bool transpose);

    void sendUniformData(const GLuint id, const glm::mat2 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::mat3 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::mat4 &value, const bool transpose);

    void sendUniformData(const GLuint id, const glm::mat2x3 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::mat3x2 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::mat2x4 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::mat4x2 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::mat3x4 &value, const bool transpose);
    void sendUniformData(const GLuint id, const glm::mat4x3 &value, const bool transpose);
    // Array types.
    void sendArrayUniformData(const GLuint id, const GLfloat *ptr, const int size, const bool transpose);
};

template<typename T>
inline bool Shader::setUniform(const unsigned int key, const T &value, const bool transpose) {
	UniformMapIter it = mUniforms.find(key);
	if (it == mUniforms.end()) return false;
	sendUniformData(it->second, value, transpose);
    return true;
}

template<typename T>
inline bool Shader::setArrayUniform(const unsigned int key, const T *ptr, const unsigned int size, bool transpose) {
	UniformMapIter it = mUniforms.find(key);
	if (it == mUniforms.end()) return false;
	sendArrayUniformData(it->second, ptr, size, transpose);
    return true;
}

template<typename T>
inline bool Shader::setUniformAtLoc(const unsigned int location, const T &value, const bool transpose) {
	sendUniformData(location, value, transpose);
    return true;
}


#endif /* SHADER_H_ */
