/**
 * @file    Input.h
 * @brief   Input a class header file
 * @details This file contains the definition of the Input class which is responsible
 *          for handling input in the game. The Input class is responsible for handling keyboard
 *          and mouse input in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-26
 */

#ifndef FANTASYTACTICS_INPUT_H
#define FANTASYTACTICS_INPUT_H

#include <array>

#include "Keyboard.h"

namespace cbit {
    /**
     * @class Input
     * @brief  Input class for handling input in the game.
     */
    class Input {
    public:
        static void setWindow(GLFWwindow *window);

        static void update();

        static bool isKeyDown(Keyboard key);

        static bool isKeyPressed(Keyboard key);

        static bool isKeyReleased(Keyboard key);

    private:
        static GLFWwindow *_window;
        static std::array<unsigned char, GLFW_KEY_LAST + 1> _current;
        static std::array<unsigned char, GLFW_KEY_LAST + 1> _previous;
    };
}

#endif //FANTASYTACTICS_INPUT_H
