//
// Created by User on 1/1/2024.
//

#ifndef FANTASYTACTICS_SHADER_H
#define FANTASYTACTICS_SHADER_H

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include "glm/glm.hpp"
#include <map>

#include "../utilities/logger.h"

std::string get_file_contents(const char *filename);

class Shader {
public:
    Shader();

    Shader(const char *vsFilename, const char *fsFilename);

    ~Shader();

    enum ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    // Only supports vertex and fragment (this series will only have those two)
    bool loadShaders(const char *vsFilename, const char *fsFilename);

    void use();

    GLuint getProgram() const;

    void setUniform(const GLchar *name, const glm::vec2 &v);

    void setUniform(const GLchar *name, const glm::vec3 &v);

    void setUniform(const GLchar *name, const glm::vec4 &v);

    void setUniform(const GLchar *name, const glm::mat4 &m);

    void setUniform(const GLchar *name, const GLfloat f);

    void setUniform(const GLchar *name, const GLint v);

    void setUniformSampler(const GLchar *name, const GLint &slot);

private:
    std::string _fileToString(const std::string &filename);

    void _checkCompileErrors(GLuint shader, ShaderType type);

    // We are going to speed up looking for uniforms by keeping their locations in a map
    GLint _getUniformLocation(const GLchar *name);


    GLuint mHandle;
    std::map<std::string, GLint> mUniformLocations;
};

#endif //FANTASYTACTICS_SHADER_H
