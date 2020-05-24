#pragma once

#include <glm/fwd.hpp>

#include <memory>

class GCamera
{
public:
	// Field of view in degrees, 
	GCamera(const float& FOV = 60.0f, const float& AspectRatio = (4.0f / 3.0f), const float& NearPlane = 0.1f, const float& FarPlane = 1000.0f);

	~GCamera();

	void Tick();

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

private:
	void MoveForward();

	void MoveBackwards();

	void MoveRight();

	void MoveLeft();

private:
	std::shared_ptr<glm::mat4> ProjectionMatrix;
	std::shared_ptr<glm::mat4> ViewMatrix;
	std::shared_ptr<glm::mat4> ProjectionViewMatrix;

	std::unique_ptr<glm::vec3> CameraPosition;
	std::unique_ptr<const glm::vec3> CameraUpVector;
	std::unique_ptr<glm::vec3> CameraForwardVector;

	const float CameraSpeed = 0.05f;
};
