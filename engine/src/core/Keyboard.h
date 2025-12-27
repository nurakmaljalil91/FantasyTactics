/**
 * @file    Keyboard.h
 * @brief   Header file for the Keyboard enumeration.
 * @details This file defines the Keyboard enumeration which represents various keyboard keys using GLFW key codes.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-26
 */

#ifndef CBIT_KEYBOARD_H
#define CBIT_KEYBOARD_H

#include "OpenGLInclude.h"

namespace cbit {
    /**
     * @enum Keyboard
     * @brief Enumeration for keyboard keys.
     * @details This enumeration defines various keyboard keys using GLFW key codes.
     */
    enum Keyboard: int {
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
        Escape = GLFW_KEY_ESCAPE,
        Enter = GLFW_KEY_ENTER,
        Space = GLFW_KEY_SPACE,
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        RightShift = GLFW_KEY_RIGHT_SHIFT,
        LeftControl = GLFW_KEY_LEFT_CONTROL,
        RightControl = GLFW_KEY_RIGHT_CONTROL,
        Up = GLFW_KEY_UP,
        Down = GLFW_KEY_DOWN,
        Left = GLFW_KEY_LEFT,
        Right = GLFW_KEY_RIGHT
    };
}

#endif //CBIT_KEYBOARD_H
