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

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


class Camera {
public:
    glm::mat4 getViewMatrix() const;

    virtual void setPosition(const glm::vec3 &position) {
    }

    virtual void rotate(float yaw, float pitch) {
    } // in degrees
    virtual void move(const glm::vec3 &offsetPos) {
    }

    const glm::vec3 &getLook() const;

    const glm::vec3 &getRight() const;

    const glm::vec3 &getUp() const;

    const glm::vec3 &getPosition() const;

    float getFov() const { return mFOV; }
    void setFov(float fov) { mFOV = fov; } // in degrees

protected:
    Camera();

    virtual void _updateCameraVectors() {
    }

    glm::vec3 mPosition;
    glm::vec3 mTargetPos;
    glm::vec3 mLook;
    glm::vec3 mUp;
    glm::vec3 mRight;
    const glm::vec3 WORLD_UP;

    // Euler Angles (in radians)
    float mYaw;
    float mPitch;

    // Camera parameters
    float mFOV; // degrees
};


#endif //CAMERA_H
