/**
 * @file    IsometricCamera.h
 * @brief   Header for an isometric (orthographic) camera.
 * @details Defines a camera that looks down at a target from the
 *          standard isometric angles (yaw = 225°, pitch = –35.264°)
 *          and uses an orthographic projection.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-21
 */

#ifndef FANTASYTACTICS_ISOMETRICCAMERA_H
#define FANTASYTACTICS_ISOMETRICCAMERA_H

#include "OpenGLInclude.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

//
// A basic isometric camera using an orthographic projection.
// By default, we rotate ~35.264° around X, then 45° around Y.
//
// Usage:
// IsometricCamera isoCam(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
// isoCam.setPosition({ 0.f, 0.f, 10.f }); // optional
// isoCam.setAngles(45.f, 35.264f);        // optional
//
// // Then in your render loop:
// glm::mat4 view = isoCam.getViewMatrix();
// glm::mat4 proj = isoCam.getProjectionMatrix();
// // pass them to your shader as "uView", "uProjection", etc.
//
class IsometricCamera : public Camera {
public:
    /**
        * @brief Constructs an isometric camera.
        * @param center     The world‑space point to orbit around.
        * @param size       Half the height of the ortho view volume (world units).
        * @param distance   Distance from a center along the isometric direction.
        */
    explicit IsometricCamera(
        const glm::vec3 &center = glm::vec3(0.0f),
        float size = 10.0f,
        float distance = 20.0f
    );

    /**
     * @brief Recompute _position, _right, _up based on center/direction.
     * Call once per frame before getViewMatrix().
     */
    void updateCamera();

    /**
     * @brief Zooms the ortho volume in/out.
     * @param yOffset  Scroll wheel delta.
     */
    void processMouseScroll(float yOffset);

    /** GLFW scroll callback: hook with glfwSetScrollCallback */
    static void scrollCallback(GLFWwindow *window, double /*dx*/, double dy);

    /**
     * @brief Returns an orthographic projection matrix.
     * @param aspectRatio  viewport width/height
     * @return 4×4 ortho projection
     */
    [[nodiscard]] glm::mat4 getProjectionMatrix(float aspectRatio) const;

    /**
     * @brief Sets the camera's yaw and pitch angles.
     * @param yaw    Yaw angle in degrees (rotation around Y axis).
     * @param pitch  Pitch angle in degrees (rotation around X axis).
     */
    void setAngles(float yaw, float pitch);

    /**
     * @brief Rotates the camera around the Y axis (yaw).
     * @param delta  Change in yaw angle in degrees.
     */
    void rotateYaw(float delta);

    /**
     * @brief gets the current yaw angle.
     * @return yaw angle in degrees.
     */
    [[nodiscard]] float getYaw() const { return _yaw; }

    /**
     * @brief gets the current pitch angle.
     * @return pitch angle in degrees.
     */
    [[nodiscard]] float getPitch() const { return _pitch; }

private:
    glm::vec3 _center; ///< Target to look at
    float _size; ///< Half‑height of ortho box
    float _distance; ///< Distance from a center along iso direction
};


#endif //FANTASYTACTICS_ISOMETRICCAMERA_H
