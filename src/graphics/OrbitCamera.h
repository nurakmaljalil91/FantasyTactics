//
// Created by User on 8/1/2024.
//

#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H
#include "Camera.h"


class OrbitCamera : public Camera {
public:
    OrbitCamera();

    virtual void rotate(float yaw, float pitch); // in degrees

    // Camera Controls
    void setLookAt(const glm::vec3 &target);

    void setRadius(float radius);

private:
    void updateCameraVectors();

    // Camera parameters
    float mRadius;
};


#endif //ORBITCAMERA_H
