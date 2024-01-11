//
// Created by User on 7/1/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


class Camera {
public:
    glm::mat4 GetViewMatrix() const;

    virtual void SetPosition(const glm::vec3 &position) {
    }

    virtual void Rotate(float yaw, float pitch) {
    } // in degrees
    virtual void Move(const glm::vec3 &offsetPos) {
    }

    const glm::vec3 &GetLook() const;

    const glm::vec3 &GetRight() const;

    const glm::vec3 &GetUp() const;

    const glm::vec3 &GetPosition() const;

    float GetFOV() const { return mFOV; }
    void SetFOV(float fov) { mFOV = fov; } // in degrees

protected:
    Camera();

    virtual void UpdateCameraVectors() {
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
