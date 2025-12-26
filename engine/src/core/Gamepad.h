/**
 * @file    Gamepad.h
 * @brief   Header file for the Gamepad enumerations.
 * @details This file defines gamepad buttons and axes using GLFW codes.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-26
 */

#ifndef FANTASYTACTICS_GAMEPAD_H
#define FANTASYTACTICS_GAMEPAD_H

#include "OpenGLInclude.h"

namespace cbit {
    /**
     * @enum GamepadButton
     * @brief Enumeration for gamepad buttons.
     */
    enum class GamepadButton : int {
        A = GLFW_GAMEPAD_BUTTON_A,
        B = GLFW_GAMEPAD_BUTTON_B,
        X = GLFW_GAMEPAD_BUTTON_X,
        Y = GLFW_GAMEPAD_BUTTON_Y,
        LeftBumper = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
        RightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
        Back = GLFW_GAMEPAD_BUTTON_BACK,
        Start = GLFW_GAMEPAD_BUTTON_START,
        Guide = GLFW_GAMEPAD_BUTTON_GUIDE,
        LeftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
        RightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
        DpadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
        DpadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
        DpadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
        DpadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT
    };

    /**
     * @enum GamepadAxis
     * @brief Enumeration for gamepad axes.
     */
    enum class GamepadAxis : int {
        LeftX = GLFW_GAMEPAD_AXIS_LEFT_X,
        LeftY = GLFW_GAMEPAD_AXIS_LEFT_Y,
        RightX = GLFW_GAMEPAD_AXIS_RIGHT_X,
        RightY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
        LeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
        RightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
    };
}

#endif //FANTASYTACTICS_GAMEPAD_H
