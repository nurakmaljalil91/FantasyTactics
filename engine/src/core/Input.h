/**
 * @file    Input.h
 * @brief   Input a class header file
 * @details This file contains the definition of the Input class which is responsible
 *          for handling input in the game. The Input class is responsible for handling keyboard
 *          and mouse input in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-26
 */

#ifndef CBIT_INPUT_H
#define CBIT_INPUT_H

#include <array>

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"

namespace cbit {
    /**
     * @class Input
     * @brief  Input class for handling input in the game.
     */
    class Input {
    public:
        static void setWindow(GLFWwindow *window);
        static void setGamepad(int gamepadId);

        static void update();

        static bool isKeyDown(Keyboard key);
        static bool isKeyPressed(Keyboard key);
        static bool isKeyReleased(Keyboard key);

        static bool isMouseDown(MouseButton button);
        static bool isMousePressed(MouseButton button);
        static bool isMouseReleased(MouseButton button);
        static void getMousePosition(double &x, double &y);
        static void getMouseDelta(double &dx, double &dy);

        static bool isGamepadPresent();
        static bool isGamepadButtonDown(GamepadButton button);
        static bool isGamepadButtonPressed(GamepadButton button);
        static bool isGamepadButtonReleased(GamepadButton button);
        static float getGamepadAxis(GamepadAxis axis);

    private:
        static GLFWwindow *_window;
        static std::array<unsigned char, GLFW_KEY_LAST + 1> _current;
        static std::array<unsigned char, GLFW_KEY_LAST + 1> _previous;
        static std::array<unsigned char, GLFW_MOUSE_BUTTON_LAST + 1> _mouseCurrent;
        static std::array<unsigned char, GLFW_MOUSE_BUTTON_LAST + 1> _mousePrevious;
        static double _mouseX;
        static double _mouseY;
        static double _mouseDeltaX;
        static double _mouseDeltaY;
        static bool _mouseInitialized;

        static int _gamepadId;
        static bool _gamepadPresent;
        static GLFWgamepadstate _gamepadCurrent;
        static GLFWgamepadstate _gamepadPrevious;
    };
}

#endif //CBIT_INPUT_H
