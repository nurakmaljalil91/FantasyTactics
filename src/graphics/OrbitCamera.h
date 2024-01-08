//
// Created by User on 8/1/2024.
//

#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H
#include "Camera.h"


class OrbitCamera : public Camera {
public:
    OrbitCamera();

    virtual void Rotate(float yaw, float pitch); // in degrees

    // Camera Controls
    void SetLookAt(const glm::vec3 &target);

    void SetRadius(float radius);

private:
    void UpdateCameraVectors();

    // Camera parameters
    float mRadius;
};


#endif //ORBITCAMERA_H
