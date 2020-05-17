#include "Graphics/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GCamera::GCamera(const float& FOV, const float& AspectRatio, const float& NearPlane, const float& FarPlane)
{
	ProjectionMatrix = std::make_shared<glm::mat4>(glm::perspective(glm::radians(FOV), AspectRatio, NearPlane, FarPlane));
	ViewMatrix = std::make_shared<glm::mat4>(1.0f);
	*ViewMatrix = glm::translate(*ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	ProjectionViewMatrix = std::make_shared<glm::mat4>(1.0f);
}

GCamera::~GCamera()
{
}

void GCamera::Tick()
{
	*ProjectionViewMatrix = *ProjectionMatrix * *ViewMatrix;
}
