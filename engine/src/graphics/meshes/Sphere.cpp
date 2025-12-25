/**
 * @file    Sphere.cpp
 * @brief   Sphere class for rendering a sphere mesh.
 * @details Sphere class that initializes a sphere mesh with a specified radius, stacks, and slices.
 * @author  Nur Akmal bin Jalil
 * @date    2025-03-26
 */
#include "Sphere.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for pi constants

cbit::Sphere::Sphere(const float radius, const int stacks, const int slices) {
    // Helper lambda for generating one vertex (position, normal, texcoords)
    auto makeVertex = [&](const int stackIdx, const int sliceIdx) {
        const float theta = glm::pi<float>() * static_cast<float>(stackIdx) / static_cast<float>(stacks); // 0..pi
        const float phi = glm::two_pi<float>() * static_cast<float>(sliceIdx) / static_cast<float>(slices); // 0..2pi

        const float x = radius * std::sin(theta) * std::cos(phi);
        const float y = radius * std::cos(theta);
        const float z = radius * std::sin(theta) * std::sin(phi);

        Vertex v{};
        v.position = glm::vec3(x, y, z);
        v.normal = glm::normalize(v.position);
        v.textureCoordinates = glm::vec2(phi / glm::two_pi<float>(), 1.0f - theta / glm::pi<float>());
        return v;
    };

    // Vertex generation: (stacks + 1) * (slices + 1)
    for (int i = 0; i <= stacks; ++i) {
        for (int j = 0; j <= slices; ++j) {
            vertices.push_back(makeVertex(i, j));
        }
    }

    // Index generation: 2 triangles per quad (i,j) to (i+1,j+1)
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            const int current = i * (slices + 1) + j;
            const int next = (i + 1) * (slices + 1) + j;

            // Triangle 1
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            // Triangle 2
            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    initializeBuffers();
    loaded = true;
}

cbit::Sphere::~Sphere() = default;
