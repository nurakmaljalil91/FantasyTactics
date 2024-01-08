//
// Created by User on 8/1/2024.
//

#include "FPSCamera.h"


// FPSCamera - Constructor
FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
	mPosition = position;
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);
}

// FPSCamera - Constructor
// Given camera staring point and starting target point to look at
FPSCamera::FPSCamera(glm::vec3 position, glm::vec3 target)
{
	mPosition = position;
	mTargetPos = target;

	// Calculate the vector that looks at the target from the camera position
	glm::vec3 lookDir = position - target;

	// Now Calculate the pitch and yaw from the target look vector.  (radians)
	mPitch = -atan2(lookDir.y, sqrt(lookDir.x * lookDir.x + lookDir.z * lookDir.z));
	mYaw = atan2(lookDir.x, lookDir.z) + glm::pi<float>();
}

// FPSCamera - Sets the camera position in world space
void FPSCamera::SetPosition(const glm::vec3& position)
{
	mPosition = position;
}

// FPSCamera - Sets the incremental position of the camera in world space
void FPSCamera::Move(const glm::vec3& offsetPos)
{
	mPosition += offsetPos;
	UpdateCameraVectors();
}

// FPSCamera - Sets the incremental orientation of the camera
void FPSCamera::Rotate(float yaw, float pitch)
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);


	// Constrain the pitch
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	// Constraint the yaw [0, 2*pi]
	if (mYaw > glm::two_pi<float>())
		mYaw -= glm::two_pi<float>();
	else if (mYaw < 0.0)
		mYaw += glm::two_pi<float>();


	//std::cout << glm::degrees(mPitch) << " " << glm::degrees(mYaw) << std::endl;

	UpdateCameraVectors();
}

// FPSCamera - Calculates the front vector from the Camera's (updated) Euler Angles
void FPSCamera::UpdateCameraVectors()
{
	// Spherical to Cartesian coordinates
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: Our coordinate sys has Y up not Z)

	// Calculate the view direction vector based on yaw and pitch angles (roll not considered)
	// radius is 1 for normalized length
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	// Re-calculate the Right and Up vector.  For simplicity the Right vector will
	// be assumed horizontal w.r.t. the world's Up vector.
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
}

