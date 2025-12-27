/**
 * @file    FirstPersonCamera.h
 * @brief   Header file for the FirstPersonCamera class.
 * @details This file contains the definition of the FirstPersonCamera class which is responsible for managing
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-21
 */

#ifndef CBIT_FIRSTPERSONCAMERA_H
#define CBIT_FIRSTPERSONCAMERA_H

#include "Camera.h"
#include <GLFW/glfw3.h>

namespace cbit {
    /**
     * @class FirstPersonCamera
     * @brief Extends Camera to support first‑person controls (WASD, mouse look, scroll zoom).
     *
     * Usage:
     *   // After creating your GLFW window:
     *   FirstPersonCamera fpsCamera({0.0f, 1.6f,  3.0f}, {0.0f, 1.0f, 0.0f}, -90.0f, 0.0f);
     *   glfwSetCursorPosCallback(window, FirstPersonCamera::mouseCallback);
     *   glfwSetScrollCallback  (window, FirstPersonCamera::scrollCallback);
     *   // Hide cursor:
     *   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     *
     *   // In your render loop:
     *   float dt = ...;
     *   fpsCamera.processInput(window, dt);
     *   glm::mat4 view = fpsCamera.getViewMatrix();
     *   glm::mat4 proj = fpsCamera.getProjectionMatrix(width / (float)height);
     */
    class FirstPersonCamera : public Camera {
    public:
        FirstPersonCamera(
            const glm::vec3 &position,
            const glm::vec3 &worldUp,
            float yawDegrees = -90.0f, // default look toward -Z
            float pitchDegrees = 0.0f,
            float movementSpeed = 2.5f,
            float mouseSensitivity = 0.1f,
            float zoomDegrees = 45.0f
        );

        /**
        * @brief Polls keyboard and moves the camera.
        * @param window    GLFW window to read key state from.
        * @param deltaTime Time since the last frame, for consistent speed.
        */
        void processInput(GLFWwindow *window, float deltaTime);

        /**
         * @brief GLFW cursor‑position callback for mouse‑look.
         * Must be registered with glfwSetCursorPosCallback().
         */
        static void mouseCallback(GLFWwindow *window, double xPosition, double yPosition);

        /**
        * @brief GLFW scroll‑wheel callback for zoom.
        * Must be registered with glfwSetScrollCallback().
        */
        static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

    private:
        static bool _firstMouse;
        static float _lastX;
        static float _lastY;
    };
}
#endif //CBIT_FIRSTPERSONCAMERA_H
