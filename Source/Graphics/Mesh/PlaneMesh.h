#pragma once

#include "Graphics/Mesh/Mesh.h"

#include <glm/fwd.hpp>

#include <memory>

class GTexture;

class GPlaneMesh : public GMesh
{
public:
	// TODO: Construct using Normal vector
	// TODO: Only does uniform scale atm, change to be able to have rectangle planes
	GPlaneMesh(const glm::vec2& InSize, const glm::vec4& InColour, const std::shared_ptr<GTexture> InTexture = nullptr);
};
