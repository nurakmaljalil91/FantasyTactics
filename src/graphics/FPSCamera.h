//
// Created by User on 8/1/2024.
//

#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H
#include "camera.h"


class FPSCamera : public Camera {
public:
    FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);

    // (yaw) initial angle faces -Z
    FPSCamera(glm::vec3 position, glm::vec3 target);

    virtual void setPosition(const glm::vec3 &position);

    virtual void rotate(float yaw, float pitch); // in degrees
    virtual void move(const glm::vec3 &offsetPos);

private:
    void _updateCameraVectors();
};


#endif //FPS_CAMERA_H
