//
// Created by User on 26/3/2025.
//

#include "IsometricCamera.h"

IsometricCamera::IsometricCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
    : mLeft(left), mRight(right), mBottom(bottom), mTop(top), mNear(nearPlane), mFar(farPlane) {
    _updateProjectionMatrix();
    _updateViewMatrix();
}

void IsometricCamera::setPosition(const glm::vec3 &pos) {
    mPosition = pos;
    _updateViewMatrix();
}

void IsometricCamera::setAngles(float angleY, float angleX) {
    mAngleY = angleY;
    mAngleX = angleX;
    _updateViewMatrix();
}

void IsometricCamera::_updateViewMatrix() {
    // Start with identity
    glm::mat4 view = glm::mat4(1.0f);

    // Now translate by the negative position
    view = glm::translate(view, -mPosition);

    // Rotate around X first, then Y to achieve typical isometric tilt
    view = glm::rotate(view, glm::radians(mAngleX), glm::vec3(1.f, 0.f, 0.f));
    view = glm::rotate(view, glm::radians(mAngleY), glm::vec3(0.f, 1.f, 0.f));

    mViewMatrix = view;
}

void IsometricCamera::_updateProjectionMatrix() {
    // Orthographic projection for isometric
    mProjectionMatrix = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
}
