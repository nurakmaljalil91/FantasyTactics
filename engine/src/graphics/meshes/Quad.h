/**
 * @file    Quad.h
 * @brief   Header file for the Quad class.
 * @details Quad class represents a simple 2D quad mesh in OpenGL.
 *          It inherits from the Mesh class and initializes a quad mesh with predefined vertices and texture coordinates.
 *          The quad is centered at the origin and has a size of 1 unit.
 *          The class provides methods for rendering the quad mesh using OpenGL.
 *          It is designed to be used in a graphics rendering context where OpenGL is used for rendering 2D and 3D graphics.
 *          The Quad class can be used for various purposes such as rendering backgrounds, UI elements, or simple 2D shapes in a 3D scene.
 *          The class is part of a larger graphics rendering framework and is intended to be used
 *          in conjunction with other components such as ShaderProgram, Texture2D, and Mesh classes for rendering 2D graphics in a scene.
 *          The quad mesh is defined by its vertices, which are initialized in the constructor,
 *          and it can be rendered using OpenGL commands.
 *          The class is designed to be straightforward and efficient, providing a basic implementation for rendering
 *          a 2D quad mesh in OpenGL applications.
 *          The Quad class can be extended in the future to include additional features such as texture mapping
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-28
 */

#ifndef QUAD_H
#define QUAD_H

#include "Mesh.h"

/**
 * @class Quad
 * @brief Quad class for representing a 2D quad mesh.
 * @details The Quad class is used to represent a 2D quad mesh in OpenGL
 */
class Quad final : public Mesh {
public:
    /**
     * Default constructor.
     * Initializes an empty quad mesh with no vertices and no OpenGL buffers.
     */
    Quad();

    /**
     * Destructor.
     * Cleans up OpenGL resources associated with the quad mesh.
     */
    ~Quad() override;
};


#endif //QUAD_H
