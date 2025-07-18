//
// Created by User on 26/3/2025.
//

#ifndef FANTASYTACTICS_ISOMETRICCAMERA_H
#define FANTASYTACTICS_ISOMETRICCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
class IsometricCamera {
public:

    IsometricCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane);

    void setPosition(const glm::vec3& pos);

    void setAngles(float angleY, float angleX);

    // Accessors for final matrices
    const glm::mat4& getViewMatrix() const       { return mViewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return mProjectionMatrix; }
private:
    void _updateViewMatrix();
    void _updateProjectionMatrix();

    // Projection params
    float mLeft, mRight, mBottom, mTop, mNear, mFar;

    // Euler angles (in degrees) for isometric tilt
    float mAngleY = 45.0f;     // around Y axis
    float mAngleX = 35.264f;   // around X axis (approx classic 7:4 isometric angle)

    // Camera position
    glm::vec3 mPosition = glm::vec3(0.0f);

    // Final matrices
    glm::mat4 mViewMatrix       = glm::mat4(1.0f);
    glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
};


#endif //FANTASYTACTICS_ISOMETRICCAMERA_H
