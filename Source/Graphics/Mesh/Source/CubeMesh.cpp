#include "Graphics/Mesh/CubeMesh.h"

GCubeMesh::GCubeMesh(const float InSize, const glm::vec4& InColour, const std::shared_ptr<GTexture> InTexture)
{
    // Front Bottom Left
    AddVertex({ -0.5f, -0.5f, 0.5f });
	AddColour(InColour);
    // Front Top Left
    AddVertex({ -0.5f, 0.5f, 0.5f });
	AddColour(InColour);
	// Front Top Right
    AddVertex({ 0.5f, 0.5f, 0.5f });
	AddColour(InColour);
	// Front Bottom Right
    AddVertex({ 0.5f, -0.5f, 0.5f });
	AddColour(InColour);
	// Back Bottom Left
    AddVertex({ -0.5f, -0.5f, -0.5f });
	AddColour(InColour);
	// Back Top Left
    AddVertex({ -0.5f, 0.5f, -0.5f });
	AddColour(InColour);
	// Back Top Right
    AddVertex({ 0.5f, 0.5f, -0.5f });
	AddColour(InColour);
	// Back Bottom Right
    AddVertex({ 0.5f, -0.5f, -0.5f });
	AddColour(InColour);

    AddIndices(
        // Front face
        0, 1, 3,
        1, 2, 3,
        // Left face
        4, 5, 0,
        5, 1, 0,
        // Top face
        5, 6, 1,
        6, 2, 1,
        // Right face
        3, 2, 7,
        2, 6, 7,
        // Back face
        7, 6, 4,
        6, 5, 4,
        // Bottom face
        4, 0, 7,
        0, 3, 7
    );

    Scale(InSize);
}
