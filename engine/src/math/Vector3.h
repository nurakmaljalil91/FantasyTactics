/**
 * @file    Vector3.h
 * @brief   Header file for the Vector3 struct.
 * @details This file contains the definition of the Vector struct which is responsible
 *          for storing 3D vector data using glm::vec3.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */

#ifndef FANTASYTACTICS_VECTOR3_H
#define FANTASYTACTICS_VECTOR3_H

#include "glm/glm.hpp"

namespace cbit {
    /**
     * @struct Vector3
     * @brief  Struct for representing a 3D vector.
     * @details The Vector struct encapsulates a glm::vec3 to represent a 3D vector.
     *          It provides constructors for initializing the vector and methods to access the underlying glm::vec3.
     */
    struct Vector3 {
        float x = 0.0f, y = 0.0f, z = 0.0f;

        Vector3() = default;

        Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {
        }

        glm::vec3 toGLM() const {
            return glm::vec3(x, y, z);
        }
    };
}
#endif //FANTASYTACTICS_VECTOR3_H
