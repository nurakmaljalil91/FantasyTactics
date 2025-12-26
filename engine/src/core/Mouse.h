/**
 * @file    Mouse.h
 * @brief   Header file for the Mouse enumeration.
 * @details This file defines the Mouse enumeration which represents various mouse buttons using GLFW codes.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-26
 */

#ifndef FANTASYTACTICS_MOUSE_H
#define FANTASYTACTICS_MOUSE_H

#include "OpenGLInclude.h"

namespace cbit {
    /**
     * @enum MouseButton
     * @brief Enumeration for mouse buttons.
     */
    enum class MouseButton : int {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
        Button4 = GLFW_MOUSE_BUTTON_4,
        Button5 = GLFW_MOUSE_BUTTON_5
    };
}

#endif //FANTASYTACTICS_MOUSE_H
