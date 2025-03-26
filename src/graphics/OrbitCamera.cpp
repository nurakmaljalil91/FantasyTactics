//
// Created by User on 8/1/2024.
//

#include "OrbitCamera.h"


// OrbitCamera - constructor
OrbitCamera::OrbitCamera()
    : mRadius(10.0f) {
}

// OrbitCamera - Sets the target to look at
void OrbitCamera::setLookAt(const glm::vec3 &target) {
    mTargetPos = target;
}

// OrbitCamera - Sets the radius of camera to target distance
void OrbitCamera::setRadius(float radius) {
    // Clamp the radius
    mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

// OrbitCamera - Rotates the camera around the target look
// at position given yaw and pitch in degrees.
void OrbitCamera::rotate(float yaw, float pitch) {
    mYaw = glm::radians(yaw);
    mPitch = glm::radians(pitch);

    mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

    // Update Front, Right and Up Vectors using the updated Euler angles
    _updateCameraVectors();
}

//------------------------------------------------------------
// OrbitCamera - Calculates the front vector from the Camera's
// (updated) Euler Angles
//------------------------------------------------------------
void OrbitCamera::_updateCameraVectors() {
    // Spherical to Cartesian coordinates
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)
    mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
    mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
    mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}
