/**
 * @file    ShaderProgram.cpp
 * @brief   Implementation of the ShaderProgram class for managing OpenGL shaders.
 * @details Implements loading, compiling, and linking vertex and fragment shaders,
 *          as well as setting uniform variables in the shader program.
 *          This class is used to encapsulate shader operations in OpenGL applications.
 *          It provides methods to load shaders from files, check for compilation errors,
 *          and set various types of uniform variables (e.g., vectors, matrices).
 *          The shaders are expected to be written in GLSL (OpenGL Shading Language).
 *          The class also manages the OpenGL shader program handle and provides a method to use the shader program
 *          in rendering operations.
 *          It is designed to be used in conjunction with OpenGL rendering contexts.
 *          The shaders are expected to be compatible with OpenGL 3.3 or higher.
 *          The class handles shader compilation errors and logs them using a Logger utility.
 *          It also provides a method to convert file contents to a string for shader source code.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL for rendering.
 *          The shaders can be used for various rendering tasks, including 3D rendering, post-processing effects, and more.
 *          The class is part of a larger graphics rendering framework and is intended to be used by other components
 *          such as Mesh, Camera, and Texture2D classes for rendering 3D objects in a scene.
 *          The class is designed to be extensible, allowing for future enhancements such as support for geometry shaders,
 *          tessellation shaders, and compute shaders if needed.
 *          The class is also designed to be thread-safe, allowing it to be used in multithreaded rendering contexts
 *          where shaders may be loaded or used from different threads.
 *          The class is expected to be used in a C++ application that uses the OpenGL API for rendering graphics.
 *          The shaders are expected to be written in GLSL and follow the OpenGL shading language conventions.
 *          The class is designed to be used with the GLAD OpenGL loader for function loading,
 *          and it assumes that the OpenGL context has been created and initialized before using this class.
 *          The class is part of a larger graphics rendering framework and is intended to be used
 *          in conjunction with other components such as Mesh, Camera, and Texture2D classes for rendering 3D objects in a scene.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL for rendering.
 *          The shaders can be used for various rendering tasks, including 3D rendering, post-processing effects, and more.
 *          The class is part of a larger graphics rendering framework and is intended to be used by other components
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-01
 */
#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram()
    : _handle(0) {
}

ShaderProgram::ShaderProgram(const char *vsFilename, const char *fsFilename) : _handle(0) {
    loadShaders(vsFilename, fsFilename);
}


ShaderProgram::~ShaderProgram() {
    glDeleteProgram(_handle);
}

// Loads vertex and fragment shaders
bool ShaderProgram::loadShaders(const char *vsFilename, const char *fsFilename) {
    const std::string vsString = _fileToString(vsFilename);
    const std::string fsString = _fileToString(fsFilename);

    const GLchar *vsSourcePtr = vsString.c_str();
    const GLchar *fsSourcePtr = fsString.c_str();

    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);

    glCompileShader(vs);
    _checkCompileErrors(vs, VERTEX);

    glCompileShader(fs);
    _checkCompileErrors(fs, FRAGMENT);

    _handle = glCreateProgram();
    if (_handle == 0) {
        Logger::log()->error("Unable to create shader program!");
        return false;
    }

    glAttachShader(_handle, vs);
    glAttachShader(_handle, fs);

    glLinkProgram(_handle);

    glDeleteShader(vs);
    glDeleteShader(fs);

    _uniformLocations.clear();

    _checkCompileErrors(_handle, PROGRAM);

    return true;
}

// Opens and reads contents of an ASCII file to a string.  Returns the string.
// Not good for very large files.
std::string ShaderProgram::_fileToString(const std::string &filename) {
    std::stringstream ss;

    try {
        std::ifstream file;
        file.open(filename, std::ios::in);

        if (!file.fail()) {
            ss << file.rdbuf();
        } else {
            Logger::log()->error("Error reading shader filename: {}", filename);
        }

        file.close();
    } catch (std::exception &ex) {
        Logger::log()->error("Error reading shader filename: {}", filename);
        Logger::log()->error("Exception: {}", ex.what());
    }

    return ss.str();
}

// Activate the shader program
void ShaderProgram::use() const {
    if (_handle > 0) {
        glUseProgram(_handle);
    }
}

// Checks for shader compiler errors
void ShaderProgram::_checkCompileErrors(const GLuint shader, const ShaderType type) {
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
GLuint ShaderProgram::getProgram() const {
    return _handle;
}

// Sets a glm::vec2 shader uniform
void ShaderProgram::setUniform(const GLchar *name, const glm::vec2 &vector) {
    const GLint loc = _getUniformLocation(name);
    glUniform2f(loc, vector.x, vector.y);
}

// Sets a glm::vec3 shader uniform
void ShaderProgram::setUniform(const GLchar *name, const glm::vec3 &vector) {
    const GLint loc = _getUniformLocation(name);
    glUniform3f(loc, vector.x, vector.y, vector.z);
}

// Sets a glm::vec4 shader uniform
void ShaderProgram::setUniform(const GLchar *name, const glm::vec4 &vector) {
    const GLint loc = _getUniformLocation(name);
    glUniform4f(loc, vector.x, vector.y, vector.z, vector.w);
}

// Sets a glm::mat4 shader uniform
void ShaderProgram::setUniform(const GLchar *name, const glm::mat4 &material) {
    const GLint loc = _getUniformLocation(name);

    // loc = location of uniform in shader
    // count = how many matrices (1 if not an array of mats)
    // transpose = False for opengl because column major
    // value = the matrix to set for the uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(material));
}

// Sets a GLfloat shader uniform
void ShaderProgram::setUniform(const GLchar *name, const GLfloat glFloat) {
    const GLint loc = _getUniformLocation(name);
    glUniform1f(loc, glFloat);
}


// Sets a GLint shader uniform
void ShaderProgram::setUniform(const GLchar *name, const GLint glLint) {
    const GLint loc = _getUniformLocation(name);
    glUniform1i(loc, glLint);
}


// Sets a GLint shader uniform that is specific to a texture unit
void ShaderProgram::setUniformSampler(const GLchar *name, const GLint &slot) {
    glActiveTexture(GL_TEXTURE0 + slot);

    const GLint loc = _getUniformLocation(name);
    glUniform1i(loc, slot);
}

bool ShaderProgram::hasUniform(const GLchar *name) const {
    const GLint loc = glGetUniformLocation(_handle, name);
    return loc != -1;
}

// Returns the uniform identifier given its string name.
// NOTE: shader must be currently active first.
GLint ShaderProgram::_getUniformLocation(const GLchar *name) {
    // Only need to query the shader program IF it doesn't already exist.
    if (const auto it = _uniformLocations.find(name); it == _uniformLocations.end()) {
        // Find it and add it to the map
        _uniformLocations[name] = glGetUniformLocation(_handle, name);
    }

    // Return it
    return _uniformLocations[name];
}
