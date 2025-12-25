/**
 * @file    Cube.cpp
 * @brief   Implementation of the Cube class.
 * @details This file contains the implementation of the Cube class which represents a simple cube mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-03-20
 */

#include "Cube.h"
#include <glm/glm.hpp>

cbit::Cube::Cube() {
   // Define 36 unique vertices (6 faces × 2 triangles × 3 vertices)
    vertices = {
        // Front face (+Z)
        {{-0.5f, -0.5f, +0.5f}, {0.f, 0.f, +1.f}, {0.f, 0.f}},
        {{+0.5f, -0.5f, +0.5f}, {0.f, 0.f, +1.f}, {1.f, 0.f}},
        {{+0.5f, +0.5f, +0.5f}, {0.f, 0.f, +1.f}, {1.f, 1.f}},
        {{-0.5f, -0.5f, +0.5f}, {0.f, 0.f, +1.f}, {0.f, 0.f}},
        {{+0.5f, +0.5f, +0.5f}, {0.f, 0.f, +1.f}, {1.f, 1.f}},
        {{-0.5f, +0.5f, +0.5f}, {0.f, 0.f, +1.f}, {0.f, 1.f}},

        // Back face (-Z)
        {{+0.5f, -0.5f, -0.5f}, {0.f, 0.f, -1.f}, {0.f, 0.f}},
        {{-0.5f, -0.5f, -0.5f}, {0.f, 0.f, -1.f}, {1.f, 0.f}},
        {{-0.5f, +0.5f, -0.5f}, {0.f, 0.f, -1.f}, {1.f, 1.f}},
        {{+0.5f, -0.5f, -0.5f}, {0.f, 0.f, -1.f}, {0.f, 0.f}},
        {{-0.5f, +0.5f, -0.5f}, {0.f, 0.f, -1.f}, {1.f, 1.f}},
        {{+0.5f, +0.5f, -0.5f}, {0.f, 0.f, -1.f}, {0.f, 1.f}},

        // Left face (-X)
        {{-0.5f, -0.5f, -0.5f}, {-1.f, 0.f, 0.f}, {0.f, 0.f}},
        {{-0.5f, -0.5f, +0.5f}, {-1.f, 0.f, 0.f}, {1.f, 0.f}},
        {{-0.5f, +0.5f, +0.5f}, {-1.f, 0.f, 0.f}, {1.f, 1.f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.f, 0.f, 0.f}, {0.f, 0.f}},
        {{-0.5f, +0.5f, +0.5f}, {-1.f, 0.f, 0.f}, {1.f, 1.f}},
        {{-0.5f, +0.5f, -0.5f}, {-1.f, 0.f, 0.f}, {0.f, 1.f}},

        // Right face (+X)
        {{+0.5f, -0.5f, +0.5f}, {+1.f, 0.f, 0.f}, {0.f, 0.f}},
        {{+0.5f, -0.5f, -0.5f}, {+1.f, 0.f, 0.f}, {1.f, 0.f}},
        {{+0.5f, +0.5f, -0.5f}, {+1.f, 0.f, 0.f}, {1.f, 1.f}},
        {{+0.5f, -0.5f, +0.5f}, {+1.f, 0.f, 0.f}, {0.f, 0.f}},
        {{+0.5f, +0.5f, -0.5f}, {+1.f, 0.f, 0.f}, {1.f, 1.f}},
        {{+0.5f, +0.5f, +0.5f}, {+1.f, 0.f, 0.f}, {0.f, 1.f}},

        // Top face (+Y)
        {{-0.5f, +0.5f, +0.5f}, {0.f, +1.f, 0.f}, {0.f, 0.f}},
        {{+0.5f, +0.5f, +0.5f}, {0.f, +1.f, 0.f}, {1.f, 0.f}},
        {{+0.5f, +0.5f, -0.5f}, {0.f, +1.f, 0.f}, {1.f, 1.f}},
        {{-0.5f, +0.5f, +0.5f}, {0.f, +1.f, 0.f}, {0.f, 0.f}},
        {{+0.5f, +0.5f, -0.5f}, {0.f, +1.f, 0.f}, {1.f, 1.f}},
        {{-0.5f, +0.5f, -0.5f}, {0.f, +1.f, 0.f}, {0.f, 1.f}},

        // Bottom face (-Y)
        {{-0.5f, -0.5f, -0.5f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
        {{+0.5f, -0.5f, -0.5f}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
        {{+0.5f, -0.5f, +0.5f}, {0.f, -1.f, 0.f}, {1.f, 1.f}},
        {{-0.5f, -0.5f, -0.5f}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
        {{+0.5f, -0.5f, +0.5f}, {0.f, -1.f, 0.f}, {1.f, 1.f}},
        {{-0.5f, -0.5f, +0.5f}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
    };

    // Generate a simple index buffer (0, 1, 2, ..., 35)
    for (unsigned int i = 0; i < vertices.size(); ++i) {
        indices.push_back(i);
    }

    // Upload to GPU
    initializeBuffers();
    loaded = true;
}

cbit::Cube::~Cube() = default;
