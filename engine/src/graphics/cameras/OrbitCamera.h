/**
 * @file    OrbitCamera.h
 * @brief   Header file for the OrbitCamera class.
 * @details This file contains the definition of the OrbitCamera class which is responsible for managing
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-01
 */

#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H

#include "Camera.h"
#include <GLFW/glfw3.h>

/**
 * @class OrbitCamera
 * @brief A camera that orbits around a fixed target point.
 *
 * Usage:
 *   OrbitCamera orbitCam({0.0f, 0.0f, 0.0f}, 10.0f);
 *   // after creating GLFW window:
 *   glfwSetWindowUserPointer(window, &orbitCam);
 *   glfwSetCursorPosCallback(window, OrbitCamera::mouseCallback);
 *   glfwSetScrollCallback(window,  OrbitCamera::scrollCallback);
 *   // (optionally) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
 *
 *   // In your render loop:
 *   orbitCam.updateCameraPosition();
 *   glm::mat4 view = orbitCam.getViewMatrix();
 *   glm::mat4 proj = orbitCam.getProjectionMatrix(float(width)/height);
 */

class OrbitCamera : public Camera {
public:
    explicit OrbitCamera(
        const glm::vec3 &target = glm::vec3(0.0f),
        float distance = 10.0f,
        float yawDegrees = -90.0f,
        float pitchDegrees = 0.0f,
        float mouseSensitivity = 0.1f,
        float scrollSensitivity = 1.0f
    );

    /// Call each frame before rendering to recompute `_position`/vectors.
    void updateCameraPosition();

    /// Override to adjust yaw/pitch based on mouse drag.
    void processMouseMovement(float xOffset, float yOffset);

    /// Override to adjust distance based on scroll.
    void processMouseScroll(float yOffset);

    /// GLFW cursor‐move callback
    static void mouseCallback(GLFWwindow *window, double xPosition, double yPosition);

    /// GLFW scroll‐wheel callback
    static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

private:
    glm::vec3 _target;
    float _distance;
    float _scrollSensitivity;

    // for first‐time mouse initialization
    static bool _firstMouse;
    static float _lastX, _lastY;
};


#endif //ORBITCAMERA_H
