/**
 * @file    Color.h
 * @brief   Header file for the Color struct.
 * @details This file contains the definition of the Color struct, which represents RGBA colors using glm::vec4.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */
#ifndef CBIT_COLOR_H
#define CBIT_COLOR_H

#include "glm/vec4.hpp"

namespace cbit {
    /**
     * @struct Color
     * @brief  Represents an RGBA color using glm::vec4.
     * @details The Color struct encapsulates a color defined by red, green, blue, and alpha components.
     *          It provides constructors for easy initialization and predefined static colors for common use cases.
     */
    struct Color {
        glm::vec4 glmVec4{0.0f, 0.0f, 0.0f, 1.0f};

        constexpr Color() = default;

        constexpr Color(const float r, const float g, const float b, const float a = 1.0f)
            : glmVec4(r, g, b, a) {
        }

        constexpr Color(const glm::vec4 &vec) : glmVec4(vec) {
        }

        operator glm::vec4 &() { return glmVec4; }
        operator const glm::vec4 &() const { return glmVec4; }

        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color LightYellow;
        static const Color LightBlue;
        static const Color LightPink;
        static const Color LightGreen;
        static const Color LightGray;
    };

    inline const Color Color::Black{0.0f, 0.0f, 0.0f, 1.0f};
    inline const Color Color::White{1.0f, 1.0f, 1.0f, 1.0f};
    inline const Color Color::Red{1.0f, 0.0f, 0.0f, 1.0f};
    inline const Color Color::Green{0.0f, 1.0f, 0.0f, 1.0f};
    inline const Color Color::Blue{0.0f, 0.0f, 1.0f, 1.0f};
    inline const Color Color::Yellow{1.0f, 1.0f, 0.0f};
    inline const Color Color::LightYellow{1.0f, 1.0f, 0.88f};
    inline const Color Color::LightBlue{0.68f, 0.85f, 0.9f};
    inline const Color Color::LightPink{1.0f, 0.71f, 0.76f};
    inline const Color Color::LightGreen{0.56f, 0.93f, 0.56f};
    inline const Color Color::LightGray{0.83f, 0.83f, 0.83f};
}

#endif //CBIT_COLOR_H
