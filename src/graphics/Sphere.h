//
// Created by User on 26/3/2025.
//

#ifndef FANTASYTACTICS_SPHERE_H
#define FANTASYTACTICS_SPHERE_H


#include "Mesh.h"

class Sphere : public Mesh
{
public:
    // Create a sphere of given radius, subdivided into
    // "stacks" rows (latitude) and "slices" columns (longitude).
    Sphere(float radius = 1.0f, int stacks = 16, int slices = 32);
    ~Sphere() = default;
};


#endif //FANTASYTACTICS_SPHERE_H
