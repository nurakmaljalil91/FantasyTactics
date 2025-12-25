/**
 * @file    Circle.h
 * @brief   Circle class for rendering a filled circle mesh.
 * @details Circle class that initializes a circle mesh with a specified radius and number of segments.
 *          The circle is centered at the origin and lies on the XY plane.
 * @author  Nur Akmal bin Jalil
 * @date    2025-03-25
 */
#ifndef FANTASYTACTICS_CIRCLE_H
#define FANTASYTACTICS_CIRCLE_H

#include "Mesh.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace cbit {
    /**
     * @class   Circle
     * @brief   Class representing a circle mesh.
     * @details This class initializes a circle mesh with a specified radius and number of segments.
     *          The circle is centered at the origin and lies on the XY plane.
     *          It uses triangle fan rendering to create a filled circle.
     */
    class Circle final : public Mesh {
    public:
        explicit Circle(float radius = 1.0f, int segments = 32);

        ~Circle() override;

        // Override the draw function
        void draw() override;
    };
}

#endif //FANTASYTACTICS_CIRCLE_H
