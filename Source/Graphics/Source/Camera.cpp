#include "Graphics/Camera.h"
#include "Common/MathsMacros.h"
#include "Input/InputManager.h"
#include "Input/InputKeyMacros.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GCamera::GCamera(const float& FOV, const float& AspectRatio, const float& NearPlane, const float& FarPlane)
{
	CameraPosition = std::make_unique<glm::vec3>(0.0f, 0.0f, 3.0f);
	CameraUpVector = std::make_unique<glm::vec3>(0.0f, 1.0f, 0.0f);
	CameraForwardVector = std::make_unique<glm::vec3>(0.0f, 0.0f, -1.0f);
	CameraRightVector = std::make_unique<glm::vec3>(glm::cross(*CameraForwardVector, *CameraUpVector));

	ProjectionMatrix = std::make_shared<glm::mat4>(glm::perspective(glm::radians(FOV), AspectRatio, NearPlane, FarPlane));
	ViewMatrix = std::make_shared<glm::mat4>(glm::lookAt(*CameraPosition, *CameraPosition + *CameraForwardVector, *CameraUpVector));
	ProjectionViewMatrix = std::make_shared<glm::mat4>(1.0f);

	IInputManager::Get()->BindDelegate(this, &GCamera::HandleCursorMove);
	IInputManager::Get()->BindDelegate(this, &GCamera::HandleScroll);
}

GCamera::~GCamera()
{
}

void GCamera::Tick(const float& DeltaTime)
{
	HandleInputs(DeltaTime);

	RecalculateMatrices();
}

void GCamera::RecalculateMatrices()
{
	CameraForwardVector->x = glm::sin(glm::radians(CameraYaw)) * glm::cos(glm::radians(CameraPitch));
	CameraForwardVector->y = glm::sin(glm::radians(CameraPitch));
	CameraForwardVector->z = -glm::cos(glm::radians(CameraYaw)) * glm::cos(glm::radians(CameraPitch));
	*CameraForwardVector = glm::normalize(*CameraForwardVector);

	*CameraRightVector = glm::cross(*CameraForwardVector, *CameraUpVector);

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
	if (IInputManager::Get()->IsKeyPressed(UP_KEYS))
	{
		MoveUp(DeltaTime);
	}
	if (IInputManager::Get()->IsKeyPressed(DOWN_KEYS))
	{
		MoveDown(DeltaTime);
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
	*CameraPosition += CameraSpeed * DeltaTime * *CameraRightVector;
}

void GCamera::MoveLeft(const float& DeltaTime)
{
	*CameraPosition -= CameraSpeed * DeltaTime * *CameraRightVector;
}

void GCamera::MoveUp(const float& DeltaTime)
{
	*CameraPosition += CameraSpeed * DeltaTime * *CameraUpVector;
}

void GCamera::MoveDown(const float& DeltaTime)
{
	*CameraPosition -= CameraSpeed * DeltaTime * *CameraUpVector;
}

void GCamera::HandleCursorMove(double DeltaX, double DeltaY)
{
	CameraYaw += DeltaX * MouseSensitivity;
	CameraPitch -= DeltaY * MouseSensitivity;

	CameraPitch = INRANGE(CameraPitch, -90.0f, 90.0f);
}

void GCamera::HandleScroll(double DeltaScroll)
{
	*CameraPosition += *CameraForwardVector * (float)DeltaScroll;
}
