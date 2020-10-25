#pragma once

#include "Graphics/Mesh/EditableMesh.h"

#include <glm/fwd.hpp>

#include <memory>

class GTexture;

class GCubeMesh final : public GEditableMesh
{
public:
	//TODO: Texture the cube with a normal texture or a cube map
	GCubeMesh(const float InSize, const glm::vec4& InColour);
};
