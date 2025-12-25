/**
 * @file    FirstPersonCamera.cpp
 * @brief   Implementation file for the FirstPersonCamera class.
 * @details Implements a first-person camera with WASD movement, mouse look, and scroll zoom.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-21
 */

#include "FirstPersonCamera.h"

bool cbit::FirstPersonCamera::_firstMouse = true;
float cbit::FirstPersonCamera::_lastX = 0.0f;
float cbit::FirstPersonCamera::_lastY = 0.0f;

cbit::FirstPersonCamera::FirstPersonCamera(const glm::vec3 &position, const glm::vec3 &worldUp, float yawDegrees,
                                     float pitchDegrees, float movementSpeed, float mouseSensitivity,
                                     float zoomDegrees) {
    // initialize lastX/lastY to a center on the first callback
    // you might want to set these to your window’s width/2 and height/2 before showing the cursor
}

void cbit::FirstPersonCamera::processInput(GLFWwindow *window, float deltaTime) {
    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) processKeyboard(deltaTime, true, false, false, false);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) processKeyboard(deltaTime, false, true,  false, false);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) processKeyboard(deltaTime, false, false, true,  false);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) processKeyboard(deltaTime, false, false, false, true);
}

void cbit::FirstPersonCamera::mouseCallback(GLFWwindow *window, const double xPosition, const double yPosition) {
    if (_firstMouse) {
        _lastX = static_cast<float>(xPosition);
        _lastY = static_cast<float>(yPosition);
        _firstMouse = false;
    }

    const float xOffset = static_cast<float>(xPosition) - _lastX;
    const float yOffset = _lastY - static_cast<float>(xPosition); // reversed: y ranges bottom→top

    _lastX = static_cast<float>(xPosition);
    _lastY = static_cast<float>(xPosition);

    // Retrieve the camera instance from the GLFW user pointer
    // (make sure you call glfwSetWindowUserPointer(window, &fpsCam); after construction)
    if (const auto firstPersonCamera = static_cast<FirstPersonCamera*>(glfwGetWindowUserPointer(window))) {
        firstPersonCamera->processMouseMovement(xOffset, yOffset);
    }
}

void cbit::FirstPersonCamera::scrollCallback(GLFWwindow *window, double xOffset, const double yOffset) {
    if (const auto firstPersonCamera = static_cast<FirstPersonCamera*>(glfwGetWindowUserPointer(window))) {
        firstPersonCamera->processMouseScroll(static_cast<float>(yOffset));
    }
}
