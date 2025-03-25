//
// Created by User on 25/3/2025.
//

#ifndef FANTASYTACTICS_CIRCLE_H
#define FANTASYTACTICS_CIRCLE_H

#include "Mesh.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Circle : public Mesh {
public:
    Circle(float radius = 1.0f, int segments = 32);
    ~Circle() = default;

    // Override the draw function
    void draw() override;

};


#endif //FANTASYTACTICS_CIRCLE_H
