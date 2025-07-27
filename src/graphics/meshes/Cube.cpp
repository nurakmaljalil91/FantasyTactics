/**
 * @file    Cube.cpp
 * @brief   Implementation of the Cube class.
 * @details This file contains the implementation of the Cube class which represents a simple cube mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-03-20
 */

#include "Cube.h"
#include <glm/glm.hpp>

Cube::Cube() {
    // 6 faces, 2 triangles per face, 3 vertices per triangle => 36 vertices total.
    // Each face is a square in the xy, yz or zx plane, with normal pointing outward.
    // For clarity, each face covers the full [0..1]×[0..1] range in texture UV.
    vertices = {
        //--------------------- Positions ------------- Normals ------------- UVs ------------
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

    // Fix potential typo if you see:
    // {+0.5f, +0.f,  -0.5f} --> Should be {+0.5f, +0.5f, -0.5f}

    // Upload to GPU
    initializeBuffers();
    loaded = true;
}

Cube::~Cube() = default;
