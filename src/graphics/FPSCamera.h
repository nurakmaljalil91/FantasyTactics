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

    virtual void SetPosition(const glm::vec3 &position);

    virtual void Rotate(float yaw, float pitch); // in degrees
    virtual void Move(const glm::vec3 &offsetPos);

private:
    void UpdateCameraVectors();
};


#endif //FPS_CAMERA_H
