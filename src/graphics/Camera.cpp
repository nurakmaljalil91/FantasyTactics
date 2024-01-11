//
// Created by User on 7/1/2024.
//

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

// Default camera values
const float DEF_FOV = 45.0f; // degrees

//------------------------------------------------------------
// Base Camera class constructor
//------------------------------------------------------------
Camera::Camera()
: mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
  mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
  mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
  mRight(0.0f, 0.0f, 0.0f),
  WORLD_UP(0.0f, 1.0f, 0.0f),
  mYaw(glm::pi<float>()),
  mPitch(0.0f),
  mFOV(DEF_FOV)
{
}

//------------------------------------------------------------
// Base Camera - Returns view matrix
//------------------------------------------------------------
glm::mat4 Camera::GetViewMatrix()const
{
    return glm::lookAt(mPosition, mTargetPos, mUp);
}

//------------------------------------------------------------
// Base Camera - Returns camera's local look vector
//------------------------------------------------------------
const glm::vec3& Camera::GetLook() const
{
    return mLook;
}

//------------------------------------------------------------
// Base Camera - Returns camera's local right vector
//------------------------------------------------------------
const glm::vec3& Camera::GetRight() const
{
    return mRight;
}

//------------------------------------------------------------
// Base Camera - Returns camera's local up vector
//------------------------------------------------------------
const glm::vec3& Camera::GetUp() const
{
    return mUp;
}


// Base Camera - Returns camera's position
//------------------------------------------------------------
const glm::vec3& Camera::GetPosition() const
{
    return mPosition;
}