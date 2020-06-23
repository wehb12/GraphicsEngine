#pragma once

#include <glm/fwd.hpp>

#include <memory>

class GCamera
{
public:
	// Field of view in degrees, 
	GCamera(const float& FOV = 60.0f, const float& AspectRatio = (4.0f / 3.0f), const float& NearPlane = 0.1f, const float& FarPlane = 1000.0f);

	~GCamera();

	void Tick(const float& DeltaTime);

	inline const std::shared_ptr<glm::mat4> GetProjectionMatrix() const
	{
		return ProjectionMatrix;
	}

	inline const std::shared_ptr<glm::mat4> GetViewMatrix() const
	{
		return ViewMatrix;
	}

	inline const std::shared_ptr<glm::mat4> GetProjectionViewMatrix() const
	{
		return ProjectionViewMatrix;
	}

	inline const glm::vec3& GetPosition() const
	{
		return *CameraPosition;
	}

private:
	void RecalculateMatrices();

	void HandleInputs(const float& DeltaTime);

	void MoveForward(const float& DeltaTime);

	void MoveBackwards(const float& DeltaTime);

	void MoveRight(const float& DeltaTime);

	void MoveLeft(const float& DeltaTime);

	void MoveUp(const float& DeltaTime);

	void MoveDown(const float& DeltaTime);

	void HandleCursorMove(double DeltaX, double DeltaY);

	void HandleScroll(double DeltaScroll);

private:
	std::shared_ptr<glm::mat4> ProjectionMatrix;
	std::shared_ptr<glm::mat4> ViewMatrix;
	std::shared_ptr<glm::mat4> ProjectionViewMatrix;

	std::unique_ptr<glm::vec3> CameraPosition;
	std::unique_ptr<const glm::vec3> CameraUpVector;
	std::unique_ptr<glm::vec3> CameraForwardVector;
	std::unique_ptr<glm::vec3> CameraRightVector;

	// Pitch, yaw and roll stored in degrees
	float CameraPitch = 0.0f;
	float CameraYaw = 0.0f;
	float CameraRoll = 0.0f;

	const float CameraSpeed = 10.0f;
	const float MouseSensitivity = 0.1f;
};
