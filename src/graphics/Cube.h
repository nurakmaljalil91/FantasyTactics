//
// Created by User on 20/3/2025.
//

#ifndef FANTASYTACTICS_CUBE_H
#define FANTASYTACTICS_CUBE_H


#include "Mesh.h"

/**
 * A simple cube mesh, 1×1×1, centered at the origin.
 */
class Cube : public Mesh {
public:
    Cube();
    ~Cube() = default;
};


#endif //FANTASYTACTICS_CUBE_H
