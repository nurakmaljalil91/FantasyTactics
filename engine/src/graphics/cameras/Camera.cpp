/**
 * @file    Camera.cpp
 * @brief   Implementation file for the Camera class.
 * @details This file contains the implementation of the Camera class which is responsible for managing
 *          the camera in the game. The Camera class is responsible for setting up the camera
 *          and updating the view matrix.
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-07
 */

#include "Camera.h"

void cbit::Camera::setup(const glm::vec3 position,
                         const glm::vec3 worldUp,
                         const float yawDegrees,
                         const float pitchDegrees,
                         const float movementSpeed,
                         const float mouseSensitivity,
                         const float zoomDegrees
) {
    _position = position;
    _worldUp = worldUp;
    _yaw = yawDegrees;
    _pitch = pitchDegrees;
    _movementSpeed = movementSpeed;
    _mouseSensitivity = mouseSensitivity;
    _zoom = zoomDegrees;

    // ensure front/right/up are all consistent
    updateCameraVectors();
}

cbit::Camera::Camera(const glm::vec3 &position,
                     const glm::vec3 &worldUp,
                     const float yawDegrees,
                     const float pitchDegrees,
                     const float movementSpeed,
                     const float mouseSensitivity,
                     const float zoomDegrees
) {
    setup(position, worldUp, yawDegrees, pitchDegrees, movementSpeed, mouseSensitivity, zoomDegrees);
}

glm::mat4 cbit::Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 cbit::Camera::getProjectionMatrix(const float aspectRatio) const {
    return glm::perspective(glm::radians(_zoom), aspectRatio, 0.1f, 100.0f);
}

void cbit::Camera::processKeyboard(const float deltaTime, const bool forward, const bool backward, const bool left,
                                   const bool right) {
    const float velocity = _movementSpeed * deltaTime;
    if (forward) _position += _front * velocity;
    if (backward) _position -= _front * velocity;
    if (left) _position -= _right * velocity;
    if (right) _position += _right * velocity;
}

void cbit::Camera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= _mouseSensitivity;
    yOffset *= _mouseSensitivity;

    _yaw += xOffset;
    _pitch += yOffset;

    // constrain vertical look
    _pitch = glm::clamp(_pitch, -89.0f, 89.0f);

    updateCameraVectors();
}

void cbit::Camera::processMouseScroll(const float yOffset) {
    _zoom -= yOffset;
    _zoom = glm::clamp(_zoom, 1.0f, 45.0f);
}

void cbit::Camera::updateCameraVectors() {
    // compute new front vector
    glm::vec3 f;
    f.x = static_cast<float>(cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)));
    f.y = static_cast<float>(sin(glm::radians(_pitch)));
    f.z = static_cast<float>(sin(glm::radians(_yaw)) * cos(glm::radians(_pitch)));
    _front = glm::normalize(f);

    // recalc right and up
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}
