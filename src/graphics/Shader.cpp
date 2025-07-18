//
// Created by User on 1/1/2024.
//
#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader()
    : mHandle(0) {
}

Shader::Shader(const char *vsFilename, const char *fsFilename): mHandle(0) {
    loadShaders(vsFilename, fsFilename);
}


Shader::~Shader() {
    glDeleteProgram(mHandle);
}

// Loads vertex and fragment shaders
bool Shader::loadShaders(const char *vsFilename, const char *fsFilename) {
    std::string vsString = _fileToString(vsFilename);
    std::string fsString = _fileToString(fsFilename);

    const GLchar *vsSourcePtr = vsString.c_str();
    const GLchar *fsSourcePtr = fsString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);

    glCompileShader(vs);
    _checkCompileErrors(vs, VERTEX);

    glCompileShader(fs);
    _checkCompileErrors(fs, FRAGMENT);

    mHandle = glCreateProgram();
    if (mHandle == 0) {
        Logger::log()->error("Unable to create shader program!");
        return false;
    }

    glAttachShader(mHandle, vs);
    glAttachShader(mHandle, fs);

    glLinkProgram(mHandle);

    glDeleteShader(vs);
    glDeleteShader(fs);

    mUniformLocations.clear();

    _checkCompileErrors(mHandle, PROGRAM);

    return true;
}

// Opens and reads contents of ASCII file to a string.  Returns the string.
// Not good for very large files.
std::string Shader::_fileToString(const std::string &filename) {
    std::stringstream ss;
    std::ifstream file;

    try {
        file.open(filename, std::ios::in);

        if (!file.fail()) {
            ss << file.rdbuf();
        } else {
            Logger::log()->error("Error reading shader filename: {}", filename);
        }

        file.close();
    } catch (std::exception ex) {
        Logger::log()->error("Error reading shader filename: {}", filename);
    }

    return ss.str();
}

// Activate the shader program
void Shader::use() {
    if (mHandle > 0) {
        glUseProgram(mHandle);
    }
}

// Checks for shader compiler errors
void Shader::_checkCompileErrors(GLuint shader, ShaderType type) {
    GLint status = 0;
    if (type == PROGRAM) {
        glGetProgramiv(shader, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetProgramInfoLog(shader, length, &length, &errorLog[0]);
            Logger::log()->error("Error! Program failed to link. {}", errorLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            Logger::log()->error("Error! shader failed to compile. {}", errorLog);
        }
    }
}

// Returns the active shader program
GLuint Shader::getProgram() const {
    return mHandle;
}

// Sets a glm::vec2 shader uniform
void Shader::setUniform(const GLchar *name, const glm::vec2 &v) {
    GLint loc = _getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

// Sets a glm::vec3 shader uniform
void Shader::setUniform(const GLchar *name, const glm::vec3 &v) {
    GLint loc = _getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}

// Sets a glm::vec4 shader uniform
void Shader::setUniform(const GLchar *name, const glm::vec4 &v) {
    GLint loc = _getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

// Sets a glm::mat4 shader uniform
//-----------------------------------------------------------------------------
void Shader::setUniform(const GLchar *name, const glm::mat4 &m) {
    GLint loc = _getUniformLocation(name);

    // loc = location of uniform in shader
    // count = how many matrices (1 if not an array of mats)
    // transpose = False for opengl because column major
    // value = the matrix to set for the uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

// Sets a GLfloat shader uniform
//-----------------------------------------------------------------------------
void Shader::setUniform(const GLchar *name, const GLfloat f) {
    GLint loc = _getUniformLocation(name);
    glUniform1f(loc, f);
}

//-----------------------------------------------------------------------------
// Sets a GLint shader uniform
//-----------------------------------------------------------------------------
void Shader::setUniform(const GLchar *name, const GLint v) {
    GLint loc = _getUniformLocation(name);
    glUniform1i(loc, v);
}

//-----------------------------------------------------------------------------
// Sets a GLint shader uniform that is specific to a texture unit
//-----------------------------------------------------------------------------
void Shader::setUniformSampler(const GLchar *name, const GLint &slot) {
    glActiveTexture(GL_TEXTURE0 + slot);

    GLint loc = _getUniformLocation(name);
    glUniform1i(loc, slot);
}

// Returns the uniform identifier given it's string name.
// NOTE: shader must be currently active first.
GLint Shader::_getUniformLocation(const GLchar *name) {
    std::map<std::string, GLint>::iterator it = mUniformLocations.find(name);

    // Only need to query the shader program IF it doesn't already exist.
    if (it == mUniformLocations.end()) {
        // Find it and add it to the map
        mUniformLocations[name] = glGetUniformLocation(mHandle, name);
    }

    // Return it
    return mUniformLocations[name];
}

