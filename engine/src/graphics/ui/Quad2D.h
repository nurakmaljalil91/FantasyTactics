/**
 * @file    Quad2D.h
 * @brief   Header file for the Quad2D class.
 * @details Quad2D class represents a simple 2D quad mesh used for rendering UI elements in OpenGL.
 *          It initializes a quad mesh with predefined vertices and texture coordinates.
 *          The quad is centered at the origin and has a size of 1 unit.
 *          The class provides methods for rendering the quad mesh using OpenGL.
 *          It is designed to be used in a graphics rendering context where OpenGL is used for rendering 2D and 3D graphics.
 *          The Quad2D class can be used for various purposes such as rendering UI backgrounds, buttons, or simple 2D shapes in a 3D scene.
 *          The class is part of a larger graphics rendering framework and is intended to be
 *          used in conjunction with other components such as ShaderProgram, Texture2D, and Mesh classes for rendering UI elements in a scene.
 *          The quad mesh is defined by its vertices, which are initialized in the constructor,
 *          and it can be rendered using OpenGL commands.
 *          The class is designed to be straightforward and efficient, providing a basic implementation for rendering
 *          a 2D quad mesh in OpenGL applications.
 *          The Quad2D class can be extended in the future to include additional features such as texture mapping,
 *          color blending, or other UI-specific rendering techniques.
 *          It is intended to be used in a game engine or graphics application that uses OpenGL for rendering UI elements.
 *          The class can be integrated with other UI components to create a complete
 *          user interface system for games or applications.
 *          The Quad2D class is designed to be flexible and extensible, allowing developers to add custom functionality
 *          for rendering and manipulating UI elements as needed in their applications.
 *          The class can be used in various domains such as game development, graphical user interfaces,
 *          and other applications where 2D UI elements are required for interaction and display.
 *          The Quad2D can be animated or transformed in a 3D space, allowing
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-28
 */

#ifndef Quad2D_H
#define Quad2D_H

#include "OpenGLInclude.h"

class Quad2D {
public:
    Quad2D();

    ~Quad2D();

    void draw() const;

private:
    GLuint VAO = 0, VBO = 0;
};


#endif //Quad2D_H
