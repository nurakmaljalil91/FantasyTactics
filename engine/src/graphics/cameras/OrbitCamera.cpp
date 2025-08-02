/**
 * @file    OrbitCamera.h
 * @brief   Header file for the OrbitCamera class.
 * @details This file contains the definition of the OrbitCamera class which is responsible for managing
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-01
 */

#include "OrbitCamera.h"
#include <cmath>

bool OrbitCamera::_firstMouse = true;
float OrbitCamera::_lastX = 0.0f;
float OrbitCamera::_lastY = 0.0f;

OrbitCamera::OrbitCamera(const glm::vec3 &target, float distance, float yawDegrees, float pitchDegrees,
                         float mouseSensitivity, float scrollSensitivity)

    : Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), yawDegrees, pitchDegrees, /*speed*/0.0f, mouseSensitivity,
             /*zoom*/45.0f)
      , _target(target)
      , _distance(distance)
      , _scrollSensitivity(scrollSensitivity) {
    updateCameraPosition();
}

void OrbitCamera::updateCameraPosition() {
    // spherical → Cartesian
    const float yawRad = glm::radians(_yaw);
    const float pitchRad = glm::radians(_pitch);
    _position.x = _target.x + _distance * std::cos(pitchRad) * std::cos(yawRad);
    _position.y = _target.y + _distance * std::sin(pitchRad);
    _position.z = _target.z + _distance * std::cos(pitchRad) * std::sin(yawRad);

    // rebuild front/right/up to look at target
    _front = glm::normalize(_target - _position);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void OrbitCamera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= _mouseSensitivity;
    yOffset *= _mouseSensitivity;

    _yaw += xOffset;
    _pitch += yOffset;

    // clamp pitch so you don’t flip
    _pitch = glm::clamp(_pitch, -89.0f, 89.0f);
}

void OrbitCamera::processMouseScroll(float yOffset) {
    _distance -= yOffset * _scrollSensitivity;
    _distance = glm::clamp(_distance, 1.0f, 100.0f);
}

void OrbitCamera::mouseCallback(GLFWwindow *window, double xPosition, double yPosition) {
    if (_firstMouse) {
        _lastX = static_cast<float>(xPosition);
        _lastY = static_cast<float>(yPosition);
        _firstMouse = false;
    }

    const float xOffset = static_cast<float>(xPosition) - _lastX;
    const float yOffset = _lastY - static_cast<float>(yPosition); // reversed Y

    _lastX = static_cast<float>(xPosition);
    _lastY = static_cast<float>(yPosition);

    if (const auto camera = static_cast<OrbitCamera *>(glfwGetWindowUserPointer(window))) {
        camera->processMouseMovement(xOffset, yOffset);
    }
}

void OrbitCamera::scrollCallback(GLFWwindow *window, double xOffset, const double yOffset) {
    if (const auto camera = static_cast<OrbitCamera *>(glfwGetWindowUserPointer(window))) {
        camera->processMouseScroll(static_cast<float>(yOffset));
    }
}
