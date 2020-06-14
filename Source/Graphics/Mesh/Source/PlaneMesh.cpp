#include "Graphics/Mesh/PlaneMesh.h"

#include "glm/glm.hpp"

GPlaneMesh::GPlaneMesh(const glm::vec2& InSize, const glm::vec4& InColour, const std::shared_ptr<GTexture> InTexture)
{
	AddVertex({ -0.5f, -0.5f, 0.0f });
	AddColour(InColour);

    AddVertex({ 0.5f, -0.5f, 0.0f });
	AddColour(InColour);

    AddVertex({ 0.5f, 0.5f, 0.0f });
	AddColour(InColour);

    AddVertex({ -0.5f, 0.5f, 0.0f });
	AddColour(InColour);

    if (InTexture)
    {
        AddTexCoord({ 0.0f, 0.0f });
        AddTexCoord({ 1.0f, 0.0f });
        AddTexCoord({ 1.0f, 1.0f });
        AddTexCoord({ 0.0f, 1.0f });
    }

	AddIndices(0, 3, 1, 1, 3, 2);

	SetTexture(InTexture);

	Scale(InSize[0]);
}
