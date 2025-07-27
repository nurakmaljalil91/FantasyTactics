/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-27
 */

#include "Ellipsoid.h"

#include <glm/glm.hpp>
#include <cmath>
#include "glm/ext/scalar_constants.hpp"

Ellipsoid::Ellipsoid(const float radiusX, const float radiusY, const float radiusZ, const int sectorCount,
                     const int stackCount) {
    vertices.clear();

    for (int i = 0; i <= stackCount; ++i) {
        const float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
        const float xy = std::cos(stackAngle);
        const float z = std::sin(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            const float sectorAngle = j * 2 * glm::pi<float>() / sectorCount;

            const float x = xy * std::cos(sectorAngle);
            const float y = xy * std::sin(sectorAngle);

            Vertex v{};
            v.position = glm::vec3(x * radiusX, y * radiusY, z * radiusZ);
            v.normal = glm::normalize(glm::vec3(x / radiusX, y / radiusY, z / radiusZ)); // approximate
            v.textureCoordinates = glm::vec2(static_cast<float>(j) / sectorCount, static_cast<float>(i) / stackCount);

            vertices.push_back(v);
        }
    }

    // Create triangle indices
    std::vector<unsigned int> indices;

    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1); // beginning of current stack
        int k2 = k1 + sectorCount + 1; // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    this->indices = indices; // you'll need to add this to your Mesh class
    initializeBuffers();
    loaded = true;
}
