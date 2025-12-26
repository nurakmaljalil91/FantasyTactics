/**
 * @file    Input.h
 * @brief   Input a class header file
 * @details This file contains the definition of the Input class which is responsible
 *          for handling input in the game. The Input class is responsible for handling keyboard
 *          and mouse input in the game.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-26
 */

#include "Input.h"

GLFWwindow *cbit::Input::_window = nullptr;
std::array<unsigned char, GLFW_KEY_LAST + 1> cbit::Input::_current = {};
std::array<unsigned char, GLFW_KEY_LAST + 1> cbit::Input::_previous = {};

void cbit::Input::setWindow(GLFWwindow *window) {
    _window = window;
}

void cbit::Input::update() {
    if (!_window) {
        return;
    }

    _previous = _current;
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        _current[i] = glfwGetKey(_window, i) == GLFW_PRESS;
    }
}

bool cbit::Input::isKeyDown(const Keyboard key) {
    return _current[static_cast<int>(key)];
}

bool cbit::Input::isKeyPressed(const Keyboard key) {
    return _current[static_cast<int>(key)] && !_previous[static_cast<int>(key)];
}

bool cbit::Input::isKeyReleased(const Keyboard key) {
    return !_current[static_cast<int>(key)] && _previous[static_cast<int>(key)];
}
