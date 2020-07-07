#include "Graphics/Mesh/CubeMesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GCubeMesh::GCubeMesh(const float InSize, const glm::vec4& InColour)
{
	// Front face
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3

	AddVertex({ -0.5f, 0.5f, 0.5f }); //1
	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3

	AddTexCoord({ -0.5f, 0.5f }); //0
	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, 0.5f }); //3

	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, -0.5f }); //2
	AddTexCoord({ 0.5f, 0.5f }); //3

	for (int i = 0; i < 6; ++i)
	{
		AddNormal({ 0.0f, 0.0f, 1.0f });
		AddTangent({ 1.0f, 0.0f, 0.0f });
	}

	// Left face
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4
	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0

	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0

	AddTexCoord({ -0.5f, 0.5f }); //0
	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, 0.5f }); //3

	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, -0.5f }); //2
	AddTexCoord({ 0.5f, 0.5f }); //3

	for (int i = 0; i < 6; ++i)
	{
		AddNormal({ -1.0f, 0.0f, 0.0f });
		AddTangent({ 0.0f, 0.0f, 1.0f });
	}

	// Top face
	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1

	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1

	AddTexCoord({ -0.5f, 0.5f }); //0
	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, 0.5f }); //3

	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, -0.5f }); //2
	AddTexCoord({ 0.5f, 0.5f }); //3

	for (int i = 0; i < 6; ++i)
	{
		AddNormal({ 0.0f, 1.0f, 0.0f });
		AddTangent({ 1.0f, 0.0f, 0.0f });
	}

	// Right face
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3
	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	AddTexCoord({ -0.5f, 0.5f }); //0
	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, 0.5f }); //3

	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, -0.5f }); //2
	AddTexCoord({ 0.5f, 0.5f }); //3

	for (int i = 0; i < 6; ++i)
	{
		AddNormal({ 1.0f, 0.0f, 0.0f });
		AddTangent({ 0.0f, 0.0f, -1.0f });
	}

	// Back face
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7
	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4

	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4

	AddTexCoord({ -0.5f, 0.5f }); //0
	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, 0.5f }); //3

	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, -0.5f }); //2
	AddTexCoord({ 0.5f, 0.5f }); //3

	for (int i = 0; i < 6; ++i)
	{
		AddNormal({ 0.0f, 0.0f, -1.0f });
		AddTangent({ -1.0f, 0.0f, 0.0f });
	}

	// Bottom face
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	AddVertex({ -0.5f, -0.5f, 0.5f }); //0
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	AddTexCoord({ -0.5f, 0.5f }); //0
	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, 0.5f }); //3

	AddTexCoord({ -0.5f, -0.5f }); //1
	AddTexCoord({ 0.5f, -0.5f }); //2
	AddTexCoord({ 0.5f, 0.5f }); //3

	for (int i = 0; i < 6; ++i)
	{
		AddNormal({ 0.0f, -1.0f, 0.0f });
		AddTangent({ 1.0f, 0.0f, 0.0f });
	}

	for (int i = 0; i < 36; ++i)
	{
		AddColour(InColour);
	}

	Scale(InSize);
}
