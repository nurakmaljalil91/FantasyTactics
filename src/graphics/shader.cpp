//
// Created by User on 1/1/2024.
//
#include "shader.h"

shader::shader()
    : handle(0) {
}

shader::~shader() {
    glDeleteProgram(handle);
}

// Loads vertex and fragment shaders
bool shader::loadShaders(const char *vsFilename, const char *fsFilename) {
    std::string vsString = fileToString(vsFilename);
    std::string fsString = fileToString(fsFilename);

    const GLchar *vsSourcePtr = vsString.c_str();
    const GLchar *fsSourcePtr = fsString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);

    glCompileShader(vs);
    checkCompileErrors(vs, VERTEX);

    glCompileShader(fs);
    checkCompileErrors(fs, FRAGMENT);

    handle = glCreateProgram();
    if (handle == 0) {
        logger::log()->error("Unable to create shader program!");
        return false;
    }

    glAttachShader(handle, vs);
    glAttachShader(handle, fs);

    glLinkProgram(handle);

    glDeleteShader(vs);
    glDeleteShader(fs);

    uniformLocations.clear();

    checkCompileErrors(handle, PROGRAM);

    return true;
}

// Opens and reads contents of ASCII file to a string.  Returns the string.
// Not good for very large files.
std::string shader::fileToString(const std::string &filename) {
    std::stringstream ss;
    std::ifstream file;

    try {
        file.open(filename, std::ios::in);

        if (!file.fail()) {
            ss << file.rdbuf();
        }

        file.close();
    } catch (std::exception ex) {
        logger::log()->error("Error reading shader filename: {}", filename);
    }

    return ss.str();
}

// Activate the shader program
void shader::use() {
    if (handle > 0) {
        glUseProgram(handle);
    }
}

// Checks for shader compiler errors
void shader::checkCompileErrors(GLuint shader, shader::ShaderType type) {
    GLint status = 0;
    if (type == PROGRAM) {
        glGetProgramiv(shader, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetProgramInfoLog(shader, length, &length, &errorLog[0]);
            logger::log()->error("Error! Program failed to link. {}", errorLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            logger::log()->error("Error! shader failed to compile. {}", errorLog);
        }
    }
}

// Returns the active shader program
GLuint shader::getProgram() const {
    return handle;
}

// Sets a glm::vec2 shader uniform
void shader::setUniform(const GLchar *name, const glm::vec2 &v) {
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

// Sets a glm::vec3 shader uniform
void shader::setUniform(const GLchar *name, const glm::vec3 &v) {
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}

// Sets a glm::vec4 shader uniform
void shader::setUniform(const GLchar *name, const glm::vec4 &v) {
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

// Returns the uniform identifier given it's string name.
// NOTE: shader must be currently active first.
GLint shader::getUniformLocation(const GLchar *name) {
    std::map<std::string, GLint>::iterator it = uniformLocations.find(name);

    // Only need to query the shader program IF it doesn't already exist.
    if (it == uniformLocations.end()) {
        // Find it and add it to the map
        uniformLocations[name] = glGetUniformLocation(handle, name);
    }

    // Return it
    return uniformLocations[name];
}
