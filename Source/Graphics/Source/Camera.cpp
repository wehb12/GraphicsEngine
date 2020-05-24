#include "Graphics/Camera.h"
#include "Input/InputManager.h"

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

	IInputManager::Get()->BindDelegate(EInputKey::ARROW_UP, this, &GCamera::MoveForward);
	IInputManager::Get()->BindDelegate(EInputKey::W, this, &GCamera::MoveForward);

	IInputManager::Get()->BindDelegate(EInputKey::ARROW_DOWN, this, &GCamera::MoveBackwards);
	IInputManager::Get()->BindDelegate(EInputKey::S, this, &GCamera::MoveBackwards);

	IInputManager::Get()->BindDelegate(EInputKey::ARROW_LEFT, this, &GCamera::MoveLeft);
	IInputManager::Get()->BindDelegate(EInputKey::A, this, &GCamera::MoveLeft);

	IInputManager::Get()->BindDelegate(EInputKey::ARROW_RIGHT, this, &GCamera::MoveRight);
	IInputManager::Get()->BindDelegate(EInputKey::D, this, &GCamera::MoveRight);
}

GCamera::~GCamera()
{
}

void GCamera::Tick()
{
	*ViewMatrix = glm::lookAt(*CameraPosition, *CameraPosition + *CameraForwardVector, *CameraUpVector);
	*ProjectionViewMatrix = *ProjectionMatrix * *ViewMatrix;
}

void GCamera::MoveForward()
{
	*CameraPosition += CameraSpeed * *CameraForwardVector;
}

void GCamera::MoveBackwards()
{
	*CameraPosition -= CameraSpeed * *CameraForwardVector;
}

void GCamera::MoveRight()
{
	*CameraPosition += CameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
}

void GCamera::MoveLeft()
{
	*CameraPosition -= CameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
}
