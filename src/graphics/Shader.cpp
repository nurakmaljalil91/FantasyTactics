//
// Created by User on 1/1/2024.
//
#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader()
    : mHandle(0) {
}

Shader::~Shader() {
    glDeleteProgram(mHandle);
}

// Loads vertex and fragment shaders
bool Shader::LoadShaders(const char *vsFilename, const char *fsFilename) {
    std::string vsString = FileToString(vsFilename);
    std::string fsString = FileToString(fsFilename);

    const GLchar *vsSourcePtr = vsString.c_str();
    const GLchar *fsSourcePtr = fsString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);

    glCompileShader(vs);
    CheckCompileErrors(vs, VERTEX);

    glCompileShader(fs);
    CheckCompileErrors(fs, FRAGMENT);

    mHandle = glCreateProgram();
    if (mHandle == 0) {
        Logger::Log()->error("Unable to create shader program!");
        return false;
    }

    glAttachShader(mHandle, vs);
    glAttachShader(mHandle, fs);

    glLinkProgram(mHandle);

    glDeleteShader(vs);
    glDeleteShader(fs);

    mUniformLocations.clear();

    CheckCompileErrors(mHandle, PROGRAM);

    return true;
}

// Opens and reads contents of ASCII file to a string.  Returns the string.
// Not good for very large files.
std::string Shader::FileToString(const std::string &filename) {
    std::stringstream ss;
    std::ifstream file;

    try {
        file.open(filename, std::ios::in);

        if (!file.fail()) {
            ss << file.rdbuf();
        }

        file.close();
    } catch (std::exception ex) {
        Logger::Log()->error("Error reading shader filename: {}", filename);
    }

    return ss.str();
}

// Activate the shader program
void Shader::Use() {
    if (mHandle > 0) {
        glUseProgram(mHandle);
    }
}

// Checks for shader compiler errors
void Shader::CheckCompileErrors(GLuint shader, Shader::ShaderType type) {
    GLint status = 0;
    if (type == PROGRAM) {
        glGetProgramiv(shader, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetProgramInfoLog(shader, length, &length, &errorLog[0]);
            Logger::Log()->error("Error! Program failed to link. {}", errorLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            Logger::Log()->error("Error! shader failed to compile. {}", errorLog);
        }
    }
}

// Returns the active shader program
GLuint Shader::GetProgram() const {
    return mHandle;
}

// Sets a glm::vec2 shader uniform
void Shader::SetUniform(const GLchar *name, const glm::vec2 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

// Sets a glm::vec3 shader uniform
void Shader::SetUniform(const GLchar *name, const glm::vec3 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}

// Sets a glm::vec4 shader uniform
void Shader::SetUniform(const GLchar *name, const glm::vec4 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

// Sets a glm::mat4 shader uniform
//-----------------------------------------------------------------------------
void Shader::SetUniform(const GLchar* name, const glm::mat4& m)
{
    GLint loc = GetUniformLocation(name);

    // loc = location of uniform in shader
    // count = how many matrices (1 if not an array of mats)
    // transpose = False for opengl because column major
    // value = the matrix to set for the uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

// Returns the uniform identifier given it's string name.
// NOTE: shader must be currently active first.
GLint Shader::GetUniformLocation(const GLchar *name) {
    std::map<std::string, GLint>::iterator it = mUniformLocations.find(name);

    // Only need to query the shader program IF it doesn't already exist.
    if (it == mUniformLocations.end()) {
        // Find it and add it to the map
        mUniformLocations[name] = glGetUniformLocation(mHandle, name);
    }

    // Return it
    return mUniformLocations[name];
}
