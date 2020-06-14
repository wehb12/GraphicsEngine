#include "Graphics/Mesh/CubeMesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GCubeMesh::GCubeMesh(const float InSize, const glm::vec4& InColour, const std::shared_ptr<GTexture> InTexture)
{
	// Front face
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3

    AddVertex({ -0.5f, 0.5f, 0.5f }); //1
	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3

	// Left face
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4
	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0

	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0

	// Top face
	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1

	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ -0.5f, 0.5f, 0.5f }); //1

    // Right face
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3
	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	AddVertex({ 0.5f, 0.5f, 0.5f }); //2
	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	// Back face
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7
	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4

	AddVertex({ 0.5f, 0.5f, -0.5f }); //6
	AddVertex({ -0.5f, 0.5f, -0.5f }); //5
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4

	// Bottom face
	AddVertex({ -0.5f, -0.5f, -0.5f }); //4
	AddVertex({ -0.5f, -0.5f, 0.5f }); //0
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	AddVertex({ -0.5f, -0.5f, 0.5f }); //0
	AddVertex({ 0.5f, -0.5f, 0.5f }); //3
	AddVertex({ 0.5f, -0.5f, -0.5f }); //7

	for (int i = 0; i < 36; ++i)
	{
		AddColour(InColour);
	}

    Scale(InSize);
}
