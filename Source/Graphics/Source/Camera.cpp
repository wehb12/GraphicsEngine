#include "Graphics/Camera.h"
#include "Input/InputManager.h"
#include "Input/InputKeyMacros.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GCamera::GCamera(const float& FOV, const float& AspectRatio, const float& NearPlane, const float& FarPlane)
{
	CameraPosition = std::make_unique<glm::vec3>(0.0f, 0.0f, 3.0f);
	CameraUpVector = std::make_unique<glm::vec3>(0.0f, 1.0f, 0.0f);
	CameraForwardVector = std::make_unique<glm::vec3>(0.0f, 0.0f, -1.0f);

	ProjectionMatrix = std::make_shared<glm::mat4>(glm::perspective(glm::radians(FOV), AspectRatio, NearPlane, FarPlane));
	ViewMatrix = std::make_shared<glm::mat4>(glm::lookAt(*CameraPosition, *CameraPosition + *CameraForwardVector, *CameraUpVector));
	ProjectionViewMatrix = std::make_shared<glm::mat4>(1.0f);
}

GCamera::~GCamera()
{
}

void GCamera::Tick(const float& DeltaTime)
{
	HandleInputs(DeltaTime);
	*ViewMatrix = glm::lookAt(*CameraPosition, *CameraPosition + *CameraForwardVector, *CameraUpVector);
	*ProjectionViewMatrix = *ProjectionMatrix * *ViewMatrix;
}

void GCamera::HandleInputs(const float& DeltaTime)
{
	if (IInputManager::Get()->IsKeyPressed(FORWARD_KEYS))
	{
		MoveForward(DeltaTime);
	}
	if (IInputManager::Get()->IsKeyPressed(BACKWARD_KEYS))
	{
		MoveBackwards(DeltaTime);
	}
	if (IInputManager::Get()->IsKeyPressed(LEFT_KEYS))
	{
		MoveLeft(DeltaTime);
	}
	if (IInputManager::Get()->IsKeyPressed(RIGHT_KEYS))
	{
		MoveRight(DeltaTime);
	}
}

void GCamera::MoveForward(const float& DeltaTime)
{
	*CameraPosition += CameraSpeed * DeltaTime * *CameraForwardVector;
}

void GCamera::MoveBackwards(const float& DeltaTime)
{
	*CameraPosition -= CameraSpeed * DeltaTime * *CameraForwardVector;
}

void GCamera::MoveRight(const float& DeltaTime)
{
	*CameraPosition += CameraSpeed * DeltaTime * glm::vec3(1.0f, 0.0f, 0.0f);
}

void GCamera::MoveLeft(const float& DeltaTime)
{
	*CameraPosition -= CameraSpeed * DeltaTime * glm::vec3(1.0f, 0.0f, 0.0f);
}
