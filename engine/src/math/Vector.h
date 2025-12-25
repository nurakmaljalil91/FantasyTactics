/**
 * @file    Vector.h
 * @brief   Header file for the Vector struct.
 * @details This file contains the definition of the Vector struct which is responsible
 *          for storing 3D vector data using glm::vec3.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */

#ifndef FANTASYTACTICS_VECTOR_H
#define FANTASYTACTICS_VECTOR_H

#include "glm/glm.hpp"

namespace cbit {
    /**
     * @struct Vector
     * @brief  Struct for representing a 3D vector.
     * @details The Vector struct encapsulates a glm::vec3 to represent a 3D vector.
     *          It provides constructors for initializing the vector and methods to access the underlying glm::vec3.
     */
    struct Vector {
        glm::vec3 glmVec3{0.0f, 0.0f, 0.0f};

        Vector() = default;

        Vector(const glm::vec3 &vector) : glmVec3(vector) {
        }

        Vector(const float x, const float y, const float z) : glmVec3(x, y, z) {
        }

        operator glm::vec3 &() { return glmVec3; }
        operator const glm::vec3 &() const { return glmVec3; }

        glm::vec3 &data() { return glmVec3; }
        const glm::vec3 &data() const { return glmVec3; }
    };
}
#endif //FANTASYTACTICS_VECTOR_H
