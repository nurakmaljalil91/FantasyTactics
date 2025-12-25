/**
 * @file    Math.h
 * @brief   Header file for the Vector struct.
 * @details This file contains the definition of the Vector struct which is responsible
 *          for storing 3D vector data using glm::vec3.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */

#ifndef FANTASYTACTICS_VECTOR_H
#define FANTASYTACTICS_VECTOR_H

#include "glm/glm.hpp"

struct Vector {
    glm::vec3 glmVector3{0.0f, 0.0f, 0.0f};

    Vector() = default;

    explicit Vector(const glm::vec3 &vector) : glmVector3(vector) {
    }

    Vector(const float x, const float y, const float z) : glmVector3(x, y, z) {
    }

    glm::vec3 &getGlmVector() { return glmVector3; }
    const glm::vec3 &getGlmVector() const { return glmVector3; }
};

#endif //FANTASYTACTICS_VECTOR_H
