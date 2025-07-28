/**
 * @file    Sphere.h
 * @brief   Header file for the Sphere class.
 * @details Sphere class represents a simple sphere mesh with specified radius, stacks, and slices.
 *          The sphere is centered at the origin and uses spherical coordinates for vertex generation.
 *          It is used for rendering spherical objects in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-03-26
 */

#ifndef FANTASYTACTICS_SPHERE_H
#define FANTASYTACTICS_SPHERE_H

#include "Mesh.h"

/**
 * @class Sphere
 * @brief Class representing a sphere mesh.
 */
class Sphere final : public Mesh {
public:
    // Create a sphere of given radius, subdivided into
    // "stacks" rows (latitude) and "slices" columns (longitude).
    explicit Sphere(float radius = 1.0f, int stacks = 16, int slices = 32);

    ~Sphere() override;
};


#endif //FANTASYTACTICS_SPHERE_H
