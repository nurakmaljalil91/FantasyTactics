/**
 * @file    Ellipsoid.h
 * @brief   Header file for the Ellipsoid class.
 * @details This file defines the Ellipsoid class which represents a 3D ellipsoid mesh.
 *          The ellipsoid is defined by its semi-axis lengths along the x, y, and z axes.
 *          It can be used for rendering ellipsoidal shapes in a 3D graphics context.
 *          The class currently does not implement any specific functionality but serves as a placeholder
 *          for future development of ellipsoid rendering capabilities.
 *          The ellipsoid can be used in various applications such as physics simulations, 3D modeling,
 *          and visualizations where ellipsoidal shapes are required.
 *          The class is designed to be extended in the future to include methods for generating vertices,
 *          normals, and texture coordinates for rendering the ellipsoid mesh.
 *          It can also be integrated with OpenGL or other graphics libraries for rendering in a 3D context.
 *          The ellipsoid can be defined by its semi-axis lengths, which can be set through a constructor or setter methods.
 *          The class is intended to be used in a graphics rendering framework where ellipsoidal shapes are needed.
 *          The ellipsoid can be rendered using various techniques such as tessellation or subdivision to create
 *          a smooth surface for the ellipsoid mesh.
 *          The class can be used in conjunction with other graphics components such as shaders, textures,
 *          and lighting to create realistic ellipsoidal shapes in a 3D scene.
 *          The ellipsoid can also be animated or transformed in a 3D space, allowing
 *          for dynamic rendering of ellipsoidal shapes in a graphics application.
 *          The class is designed to be flexible and extensible, allowing developers to add custom functionality
 *          for rendering and manipulating ellipsoidal shapes as needed in their applications.
 *          The ellipsoid can be used in various domains such as scientific visualization, computer graphics,
 *          and game development where ellipsoidal shapes are required for modeling objects or environments.
 *          The class can be further developed to include methods for collision detection, physics simulations,
 *          and other advanced features related to ellipsoidal shapes in a 3D graphics context
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-27
 */

#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "Mesh.h"

/**
 * @class Ellipsoid
 * @brief Class representing a 3D ellipsoid mesh.
 * @details This class currently serves as a placeholder for future development of ellipsoid rendering capabilities.
 *          It can be extended to include methods for generating vertices, normals, and texture coordinates for
 *          rendering the ellipsoid mesh in a 3D graphics context.
 */
class Ellipsoid final : public Mesh {
public:
    /**
     * Default constructor.
     * Initializes an empty ellipsoid mesh.
     * @param radiusX     Semi-axis length along the X axis.
     * @param radiusY     Semi-axis length along the Y axis.
     * @param radiusZ     Semi-axis length along the Z axis.
     * @param sectorCount Number of sectors (longitude divisions). Default is 36.
     * @param stackCount  Number of stacks (latitude divisions). Default is 18.
     */
    Ellipsoid(float radiusX, float radiusY, float radiusZ, int sectorCount = 36, int stackCount = 18);
};


#endif //ELLIPSOID_H
