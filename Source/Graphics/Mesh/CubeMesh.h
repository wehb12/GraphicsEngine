#pragma once

#include "Graphics/Mesh/Mesh.h"

#include <glm/fwd.hpp>

#include <memory>

class GTexture;

class GCubeMesh : public GMesh
{
public:
	//TODO: Texture the cube with a normal texture or a cube map
	GCubeMesh(const float InSize, const glm::vec4& InColour, const std::shared_ptr<GTexture> InTexture = nullptr);
};
