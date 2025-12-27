/**
 * @file    Vector2.h
 * @brief   Thin wrapper around glm::vec2
 * @details This file defines the Vector2 struct which is a thin wrapper around glm::vec2.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */

#ifndef CBIT_VECTOR2_H
#define CBIT_VECTOR2_H

#include "glm/vec2.hpp"

namespace cbit {
    struct Vector2 {
        float x = 0.0f, y = 0.0f;

        Vector2() = default;

        Vector2(const float x, const float y) : x(x), y(y) {
        }

        [[nodiscard]] glm::vec2 toGLM() const { return {x, y}; }
    };
}

#endif //CBIT_VECTOR2_H
