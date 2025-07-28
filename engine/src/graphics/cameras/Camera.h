/**
 * @file    Camera.h
 * @brief   Header file for the Camera class.
 * @details This file contains the definition of the Camera class which is responsible for managing
 *          the camera in the game. The Camera class is responsible for setting up the camera and updating
 *          the view matrix.
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-07
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class Camera
 * @brief Encapsulates a 3D camera with position, orientation, and projection controls.
 *
 * The Camera class manages:
 *   - Position and orientation in world space (using Euler angles: yaw, pitch).
 *   - Movement via keyboard input.
 *   - Rotation via mouse input.
 *   - Zoom via mouse scroll.
 *   - Retrieval of view and projection matrices for rendering.
 */
class Camera {
public:
    /** Default constructor (leaves everything at defaults). */
    Camera() = default; // now trivial
    /** Default destructor. */
    ~Camera() = default;

    // call this once after default construction
    void setup(glm::vec3 position,
               glm::vec3 worldUp,
               float yawDegrees,
               float pitchDegrees,
               float movementSpeed = 2.5f,
               float mouseSensitivity = 0.1f,
               float zoomDegrees = 45.0f);

    /**
     * @brief Constructs and initializes a camera in one call.
     * @param position         Initial world-space position.
     * @param worldUp          World up vector.
     * @param yawDegrees       Initial yaw.
     * @param pitchDegrees     Initial pitch.
     * @param movementSpeed    Movement speed.
     * @param mouseSensitivity Mouse sensitivity.
     * @param zoomDegrees      FOV angle for projection.
     */
    Camera(const glm::vec3 &position,
           const glm::vec3 &worldUp,
           float yawDegrees = -90.0f, // default look toward -Z
           float pitchDegrees = 0.0f,
           float movementSpeed = 2.5f,
           float mouseSensitivity = 0.1f,
           float zoomDegrees = 45.0f);

    /**
     * @brief  Returns the view matrix, computed with glm::lookAt.
     * @return A 4×4 view matrix.
     */
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    /**
     * @brief  Returns the perspective projection matrix.
     * @param  aspectRatio Width/height of the viewport.
     * @return A 4×4 projection matrix.
     */
    [[nodiscard]] glm::mat4 getProjectionMatrix(float aspectRatio) const;

    /**
    * @brief Moves the camera based on keyboard input.
    * @param deltaTime Time has elapsed since the last frame (in seconds).
    * @param forward   True to move forward.
    * @param backward  True to move backward.
    * @param left      True to move left.
    * @param right     True to move right.
    */
    void processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right);

    /**
     * @brief Rotates the camera based on mouse movement.
     * @param xOffset Horizontal mouse movement delta.
     * @param yOffset Vertical mouse movement delta.
     */
    void processMouseMovement(float xOffset, float yOffset);

    /**
    * @brief Zooms the camera in/out based on scroll wheel.
    * @param yOffset Scroll delta (positive = scroll up/zoom in).
    */
    void processMouseScroll(float yOffset);

    /**
    * @brief  Retrieves the camera’s current world-space position.
    * @return A 3D position vector.
    */
    [[nodiscard]] glm::vec3 getPosition() const { return _position; }

protected:
    /**
    * @brief Recalculates front, right, and up vectors from current yaw and pitch.
    */
    void updateCameraVectors();

    // stored state
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 _right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float _yaw = -90.0f; ///< Yaw angle, initialized to look toward -Z
    float _pitch = 0.0f; ///< Pitch angle
    float _movementSpeed = 2.5f; ///< Movement speed multiplier
    float _mouseSensitivity = 0.1f; ///< Mouse look sensitivity
    float _zoom = 45.0f; ///< Field‑of‑view (degrees)
};

#endif //CAMERA_H
