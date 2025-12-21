/**
 * @file    ShaderProgram.h
 * @brief   Header file for the Shader class.
 * @details This file defines the Shader class which is responsible for loading, compiling, and managing OpenGL shaders.
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-01
 */

#ifndef FANTASYTACTICS_SHADER_H
#define FANTASYTACTICS_SHADER_H

#include "OpenGLInclude.h"
#include <string>
#include "glm/glm.hpp"
#include <map>
#include "../../utilities/Logger.h"

/**
 * @brief Reads the contents of a file into a string.
 * @param filename The name of the file to read.
 * @return A string containing the contents of the file.
 */
std::string getFileContents(const char *filename);


/**
 * @enum    ShaderType
 * @brief   Enum representing different types of shaders.
 * @details This enum is used to distinguish between vertex, fragment, and program shaders.
 */
enum ShaderType {
    VERTEX,
    FRAGMENT,
    PROGRAM
};

/**
 * @class   ShaderProgram
 * @brief   Class for managing OpenGL shaders.
 * @details This class handles loading, compiling, and linking vertex and fragment shaders.
 *          It also provides methods to set uniform variables in the shader program.
 */
class ShaderProgram {
public:
    /**
     * Default constructor.
     * Initializes the shader handle to 0.
     */
    ShaderProgram();

    /**
     * Constructor that loads shaders from specified files.
     * @param vsFilename filename for the vertex shader.
     * @param fsFilename filename for the fragment shader.
     * @details This constructor attempts to load and compile the vertex and fragment shaders from the given files.
     *          If successful, it links them into a shader program.
     *          If the shaders cannot be loaded, the shader handle will remain 0.
     */
    ShaderProgram(const char *vsFilename, const char *fsFilename);

    /**
     * Destructor.
     * Cleans up the shader program by deleting it from OpenGL.
     */
    ~ShaderProgram();

    /**
     * Loads vertex and fragment shaders from files and links them into a shader program.
     * @param vsFilename filename for the vertex shader.
     * @param fsFilename filename for the fragment shader.
     * @return true if shaders were loaded and linked successfully, false otherwise.
     */
    bool loadShaders(const char *vsFilename, const char *fsFilename);

    /**
     * Activates the shader program for use in rendering.
     * This must be called before setting uniforms or drawing objects that use this shader.
     */
    void use() const;

    /**
     * Returns the OpenGL program ID for this shader.
     * @return GLuint representing the shader program ID.
     */
    [[nodiscard]] GLuint getProgram() const;

    /**
     * Sets a uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @param vector The glm::vec2 value to set.
     */
    void setUniform(const GLchar *name, const glm::vec2 &vector);

    /**
     * Sets a uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @param vector The glm::vec3 value to set.
     */
    void setUniform(const GLchar *name, const glm::vec3 &vector);

    /**
     * Sets a uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @param vector The glm::vec4 value to set.
     */
    void setUniform(const GLchar *name, const glm::vec4 &vector);

    /**
     * Sets a uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @param material The glm::mat4 value to set.
     */
    void setUniform(const GLchar *name, const glm::mat4 &material);

    /**
     * Sets a uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @param glFloat The GLfloat value to set.
     */
    void setUniform(const GLchar *name, GLfloat glFloat);

    /**
     * Sets a uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @param glLint The GLint value to set.
     */
    void setUniform(const GLchar *name, GLint glLint);

    /**
     * Sets a sampler uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @param slot The texture unit slot to bind the sampler to.
     * @details This activates the texture unit and sets the sampler uniform to that unit.
     */
    void setUniformSampler(const GLchar *name, const GLint &slot);

    /**
     * Checks if a uniform variable exists in the shader program.
     * @param name The name of the uniform variable.
     * @return True if the uniform exists in the shader program, false otherwise.
     */
    bool hasUniform(const GLchar *name) const;

private:
    /**
     * Gets the contents of a file as a string.
     * @param filename filename of the shader file to read.
     * @return string containing the contents of the file.
     */
    static std::string _fileToString(const std::string &filename);

    /**
     * Compiles a shader from source code.
     * @param shader The OpenGL shader ID to compile.
     * @param type The type of shader (vertex, fragment, etc.).
     * @return GLuint representing the compiled shader ID, or 0 if compilation failed.
     */
    static void _checkCompileErrors(GLuint shader, ShaderType type);

    /**
     * Gets the location of a uniform variable in the shader program.
     * @param name The name of the uniform variable.
     * @return GLint representing the location of the uniform variable.
     * @details Caches the location to avoid repeated lookups.
     */
    GLint _getUniformLocation(const GLchar *name);

    GLuint _handle;
    std::map<std::string, GLint> _uniformLocations;
};

#endif //FANTASYTACTICS_SHADER_H
