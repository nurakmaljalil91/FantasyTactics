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
std::array<unsigned char, GLFW_MOUSE_BUTTON_LAST + 1> cbit::Input::_mouseCurrent = {};
std::array<unsigned char, GLFW_MOUSE_BUTTON_LAST + 1> cbit::Input::_mousePrevious = {};
double cbit::Input::_mouseX = 0.0;
double cbit::Input::_mouseY = 0.0;
double cbit::Input::_mouseDeltaX = 0.0;
double cbit::Input::_mouseDeltaY = 0.0;
bool cbit::Input::_mouseInitialized = false;
int cbit::Input::_gamepadId = GLFW_JOYSTICK_1;
bool cbit::Input::_gamepadPresent = false;
GLFWgamepadstate cbit::Input::_gamepadCurrent = {};
GLFWgamepadstate cbit::Input::_gamepadPrevious = {};

void cbit::Input::setWindow(GLFWwindow *window) {
    _window = window;
}

void cbit::Input::setGamepad(int gamepadId) {
    _gamepadId = gamepadId;
}

void cbit::Input::update() {
    if (!_window) {
        return;
    }

    _previous = _current;
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        _current[i] = glfwGetKey(_window, i) == GLFW_PRESS;
    }

    _mousePrevious = _mouseCurrent;
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
        _mouseCurrent[i] = glfwGetMouseButton(_window, i) == GLFW_PRESS;
    }

    double x = 0.0;
    double y = 0.0;
    glfwGetCursorPos(_window, &x, &y);
    if (!_mouseInitialized) {
        _mouseX = x;
        _mouseY = y;
        _mouseDeltaX = 0.0;
        _mouseDeltaY = 0.0;
        _mouseInitialized = true;
    } else {
        _mouseDeltaX = x - _mouseX;
        _mouseDeltaY = y - _mouseY;
        _mouseX = x;
        _mouseY = y;
    }

    _gamepadPresent = glfwJoystickIsGamepad(_gamepadId) == GLFW_TRUE;
    if (_gamepadPresent) {
        _gamepadPrevious = _gamepadCurrent;
        glfwGetGamepadState(_gamepadId, &_gamepadCurrent);
    } else {
        _gamepadPrevious = {};
        _gamepadCurrent = {};
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

bool cbit::Input::isMouseDown(const MouseButton button) {
    return _mouseCurrent[static_cast<int>(button)];
}

bool cbit::Input::isMousePressed(const MouseButton button) {
    const int index = static_cast<int>(button);
    return _mouseCurrent[index] && !_mousePrevious[index];
}

bool cbit::Input::isMouseReleased(const MouseButton button) {
    const int index = static_cast<int>(button);
    return !_mouseCurrent[index] && _mousePrevious[index];
}

void cbit::Input::getMousePosition(double &x, double &y) {
    x = _mouseX;
    y = _mouseY;
}

void cbit::Input::getMouseDelta(double &dx, double &dy) {
    dx = _mouseDeltaX;
    dy = _mouseDeltaY;
}

bool cbit::Input::isGamepadPresent() {
    return _gamepadPresent;
}

bool cbit::Input::isGamepadButtonDown(const GamepadButton button) {
    if (!_gamepadPresent) {
        return false;
    }
    return _gamepadCurrent.buttons[static_cast<int>(button)] == GLFW_PRESS;
}

bool cbit::Input::isGamepadButtonPressed(const GamepadButton button) {
    if (!_gamepadPresent) {
        return false;
    }
    const int index = static_cast<int>(button);
    return _gamepadCurrent.buttons[index] == GLFW_PRESS && _gamepadPrevious.buttons[index] == GLFW_RELEASE;
}

bool cbit::Input::isGamepadButtonReleased(const GamepadButton button) {
    if (!_gamepadPresent) {
        return false;
    }
    const int index = static_cast<int>(button);
    return _gamepadCurrent.buttons[index] == GLFW_RELEASE && _gamepadPrevious.buttons[index] == GLFW_PRESS;
}

float cbit::Input::getGamepadAxis(const GamepadAxis axis) {
    if (!_gamepadPresent) {
        return 0.0f;
    }
    return _gamepadCurrent.axes[static_cast<int>(axis)];
}
