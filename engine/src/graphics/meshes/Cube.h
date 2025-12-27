/**
 * @file    Cube.h
 * @brief   Header file for the Cube class.
 * @details Cube class represents a simple cube mesh with predefined vertices, normals, and texture coordinates.
 * @author  Nur Akmal bin Jalil
 * @date    2025-03-20
 */

#ifndef CBIT_CUBE_H
#define CBIT_CUBE_H

#include "Mesh.h"

namespace cbit {
    /**
     * @class   Cube
     * @brief   Class representing a simple cube mesh.
     * @details This class initializes a cube mesh with predefined vertices, normals, and texture coordinates.
     *          The cube is centered at the origin and has a size of 1 unit.
     */
    class Cube final : public Mesh {
    public:
        Cube();

        ~Cube() override;
    };
}
#endif //CBIT_CUBE_H
