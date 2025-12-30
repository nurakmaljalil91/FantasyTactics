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
     * @details Maps directly to GLFW key codes.
     */
    enum Keyboard : int {

        /* ===============================
         * Letters
         * =============================== */
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

        /* ===============================
         * Numbers (Top row)
         * =============================== */
        Num0 = GLFW_KEY_0,
        Num1 = GLFW_KEY_1,
        Num2 = GLFW_KEY_2,
        Num3 = GLFW_KEY_3,
        Num4 = GLFW_KEY_4,
        Num5 = GLFW_KEY_5,
        Num6 = GLFW_KEY_6,
        Num7 = GLFW_KEY_7,
        Num8 = GLFW_KEY_8,
        Num9 = GLFW_KEY_9,

        /* ===============================
         * Function keys
         * =============================== */
        F1  = GLFW_KEY_F1,
        F2  = GLFW_KEY_F2,
        F3  = GLFW_KEY_F3,
        F4  = GLFW_KEY_F4,
        F5  = GLFW_KEY_F5,
        F6  = GLFW_KEY_F6,
        F7  = GLFW_KEY_F7,
        F8  = GLFW_KEY_F8,
        F9  = GLFW_KEY_F9,
        F10 = GLFW_KEY_F10,
        F11 = GLFW_KEY_F11,
        F12 = GLFW_KEY_F12,

        /* ===============================
         * Control keys
         * =============================== */
        Escape      = GLFW_KEY_ESCAPE,
        Enter       = GLFW_KEY_ENTER,
        Tab         = GLFW_KEY_TAB,
        Backspace   = GLFW_KEY_BACKSPACE,
        Space       = GLFW_KEY_SPACE,

        /* ===============================
         * Modifier keys
         * =============================== */
        LeftShift   = GLFW_KEY_LEFT_SHIFT,
        RightShift  = GLFW_KEY_RIGHT_SHIFT,
        LeftControl = GLFW_KEY_LEFT_CONTROL,
        RightControl= GLFW_KEY_RIGHT_CONTROL,
        LeftAlt     = GLFW_KEY_LEFT_ALT,
        RightAlt    = GLFW_KEY_RIGHT_ALT,
        LeftSuper   = GLFW_KEY_LEFT_SUPER,
        RightSuper  = GLFW_KEY_RIGHT_SUPER,

        /* ===============================
         * Arrow keys
         * =============================== */
        Up    = GLFW_KEY_UP,
        Down  = GLFW_KEY_DOWN,
        Left  = GLFW_KEY_LEFT,
        Right = GLFW_KEY_RIGHT,

        /* ===============================
         * Navigation keys
         * =============================== */
        Insert   = GLFW_KEY_INSERT,
        Delete   = GLFW_KEY_DELETE,
        Home     = GLFW_KEY_HOME,
        End      = GLFW_KEY_END,
        PageUp   = GLFW_KEY_PAGE_UP,
        PageDown = GLFW_KEY_PAGE_DOWN,

        /* ===============================
         * Numpad
         * =============================== */
        Numpad0 = GLFW_KEY_KP_0,
        Numpad1 = GLFW_KEY_KP_1,
        Numpad2 = GLFW_KEY_KP_2,
        Numpad3 = GLFW_KEY_KP_3,
        Numpad4 = GLFW_KEY_KP_4,
        Numpad5 = GLFW_KEY_KP_5,
        Numpad6 = GLFW_KEY_KP_6,
        Numpad7 = GLFW_KEY_KP_7,
        Numpad8 = GLFW_KEY_KP_8,
        Numpad9 = GLFW_KEY_KP_9,

        NumpadAdd      = GLFW_KEY_KP_ADD,
        NumpadSubtract = GLFW_KEY_KP_SUBTRACT,
        NumpadMultiply = GLFW_KEY_KP_MULTIPLY,
        NumpadDivide   = GLFW_KEY_KP_DIVIDE,
        NumpadEnter    = GLFW_KEY_KP_ENTER,
        NumpadDecimal  = GLFW_KEY_KP_DECIMAL,

        /* ===============================
         * Punctuation
         * =============================== */
        Comma        = GLFW_KEY_COMMA,
        Period       = GLFW_KEY_PERIOD,
        Slash        = GLFW_KEY_SLASH,
        Semicolon    = GLFW_KEY_SEMICOLON,
        Apostrophe   = GLFW_KEY_APOSTROPHE,
        LeftBracket  = GLFW_KEY_LEFT_BRACKET,
        RightBracket = GLFW_KEY_RIGHT_BRACKET,
        Backslash    = GLFW_KEY_BACKSLASH,
        Minus        = GLFW_KEY_MINUS,
        Equal        = GLFW_KEY_EQUAL,
        GraveAccent  = GLFW_KEY_GRAVE_ACCENT,

        /* ===============================
         * Lock keys
         * =============================== */
        CapsLock   = GLFW_KEY_CAPS_LOCK,
        ScrollLock = GLFW_KEY_SCROLL_LOCK,
        NumLock    = GLFW_KEY_NUM_LOCK,

        /* ===============================
         * Menu
         * =============================== */
        Menu = GLFW_KEY_MENU
    };

} // namespace cbit

#endif // CBIT_KEYBOARD_H
