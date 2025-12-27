/**
 * @file    Mesh.h
 * @brief   Mesh class for loading and rendering 3D models.
 * @details Basic mesh class that can load Wavefront OBJ files and render them using OpenGL.
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-11
 */

#ifndef CBIT_MESH_H
#define CBIT_MESH_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "OpenGLInclude.h"

namespace cbit {
    /**
     * @struct  Vertex
     * @brief   Represents a single vertex in a mesh.
     * @details Contains position, normal, and texture coordinate data.
     * @author  Nur Akmal bin Jalil
     * @date    2024-01-11
     */
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
    };

    /**
     * @class   Mesh
     * @brief   Represents a 3D mesh loaded from an OBJ file.
     * @details This class handles loading a mesh from a Wavefront OBJ file and rendering it using OpenGL.
     *          It supports basic vertex attributes like position, normal, and texture coordinates.
     * @author  Nur Akmal bin Jalil
     * @date    2024-01-11
     */
    class Mesh {
    public:
        /**
         * Default constructor.
         * Initializes an empty mesh with no vertices and no OpenGL buffers.
         */
        Mesh();

        /**
         * Destructor.
         * Cleans up OpenGL resources associated with the mesh.
         */
        virtual ~Mesh();

        /**
         * @brief   Loads a Wavefront OBJ model from a file.
         * @param   filename
         * @return  if the model was loaded successfully.
         * @details This function reads the OBJ file, extracts vertex positions, normals, and texture
         *          coordinates, and initializes OpenGL buffers for rendering.
         *          NOTE! This is not a complete, full-featured OBJ loader.  It is greatly simplified.
         *          Assumptions:
         *          - an OBJ file must contain only triangles.
         *          - We ignore materials.
         *          - We ignore normals.
         *          - Only commands "v", "vt", and "f" are supported.
         */
        bool loadObj(const std::string &filename);

        /**
         * @brief   Renders the mesh using OpenGL.
         * @details Binds the vertex array object and draws the mesh using the currently active shader
         *          program. Assumes the mesh has been loaded and buffers initialized.
         */
        virtual void draw();

    protected:
        /**
         * @brief   Initializes OpenGL buffers for the mesh.
         * @details Creates and binds a vertex array object (VAO), vertex buffer object (VBO),
         *          and element buffer object (EBO) to store vertex data and indices.
         */
        void initializeBuffers();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        bool loaded;
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };
}
#endif //CBIT_MESH_H
