/**
 * @file    Quad.cpp
 * @brief   Implementation of the Quad class
 * @details This file contains the implementation of the Quad class which is used to represent a 2D quad mesh.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-28
 */

#include "Quad.h"

Quad::Quad() {
    // 4 corners: pos, normal, uv
    vertices = {
        // top-left
        {{-0.5f, 0.5f, 0.0f}, {0, 0, 1}, {0.0f, 1.0f}},
        // top-right
        {{0.5f, 0.5f, 0.0f}, {0, 0, 1}, {1.0f, 1.0f}},
        // bottom-right
        {{0.5f, -0.5f, 0.0f}, {0, 0, 1}, {1.0f, 0.0f}},
        // bottom-left
        {{-0.5f, -0.5f, 0.0f}, {0, 0, 1}, {0.0f, 0.0f}},
    };

    // two triangles
    indices = {
        0, 1, 2,
        2, 3, 0
    };

    // upload to GPU
    initializeBuffers();

    // mark ready
    loaded = true;
}

Quad::~Quad() = default;
