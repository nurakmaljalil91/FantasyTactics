/**
 * @file    IsometricCamera.cpp
 * @brief   Implementation of an isometric (orthographic) camera.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-21
 */

#include "IsometricCamera.h"

#include <iostream>
#include <ostream>

IsometricCamera::IsometricCamera(const glm::vec3 &center, float size, float distance): Camera(), _center(center),
                                                                                       _size(size), _distance(distance) {
    // classic isometric angles: yaw=225°, pitch=–35.264°
    _yaw = 225.0f;
    _pitch = -35.264f;
    // compute initial front/right/up
    updateCameraVectors();
    // position ourselves
    updateCamera();
}

void IsometricCamera::updateCamera() {
    // place camera at center minus front*distance
    // (front comes from base updateCameraVectors())
    _position = _center - _front * _distance;
    // recompute right & up so lookAt() uses them correctly
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void IsometricCamera::processMouseScroll(const float yOffset) {
    _size -= yOffset;
    if (_size < 1.0f) _size = 1.0f;
}

void IsometricCamera::scrollCallback(GLFWwindow *window, double, const double dy) {
    if (const auto camera = static_cast<IsometricCamera *>(glfwGetWindowUserPointer(window))) camera->
            processMouseScroll(static_cast<float>(dy));
}

glm::mat4 IsometricCamera::getProjectionMatrix(const float aspectRatio) const {
    const float right = _size * aspectRatio;
    const float left = -right;
    const float top = _size;
    const float bottom = -_size;
    // use a large near/far to encompass your scene
    return glm::ortho(left, right, bottom, top, -1000.0f, 1000.0f);
}
