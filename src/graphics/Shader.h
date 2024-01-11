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

    ~Shader();

    enum ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    // Only supports vertex and fragment (this series will only have those two)
    bool LoadShaders(const char *vsFilename, const char *fsFilename);

    void Use();

    GLuint GetProgram() const;

    void SetUniform(const GLchar *name, const glm::vec2 &v);

    void SetUniform(const GLchar *name, const glm::vec3 &v);

    void SetUniform(const GLchar *name, const glm::vec4 &v);

    void SetUniform(const GLchar *name, const glm::mat4 &m);

    void SetUniform(const GLchar *name, const GLfloat f);

    void SetUniform(const GLchar *name, const GLint v);

    void SetUniformSampler(const GLchar *name, const GLint &slot);

private:
    std::string FileToString(const std::string &filename);

    void CheckCompileErrors(GLuint shader, ShaderType type);

    // We are going to speed up looking for uniforms by keeping their locations in a map
    GLint GetUniformLocation(const GLchar *name);


    GLuint mHandle;
    std::map<std::string, GLint> mUniformLocations;
};

#endif //FANTASYTACTICS_SHADER_H
