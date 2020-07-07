#pragma once

#include <array>
#include <memory>
#include <string>

class GShader;
struct GraphicsTexture;

class GTexture
{
	enum ETexture : unsigned char
	{
		DIFFUSE = 0,
		SPECULAR = 1,
		NORMAL = 2,
		MAX
	};

	std::array<std::string, ETexture::MAX> TextureAppendices =
	{
		"Diffuse",
		"Specular",
		"Normal"
	};

public:
	GTexture(const std::string& TexturePath);

	~GTexture();

	void BufferToShader(std::shared_ptr<GShader> Shader);

private:
	void GenerateTextures();

	void BindTextures();

	// Sets parameters for currently bound texture unit
	void SetTextureParameters();

	// Load texture from disc and buffer to graphics card
	bool LoadTextures();

	// Finds all similarly named textures in the path and returns the paths
	void GetTexturePaths(const std::string& TexturePath);

private:
	std::array<std::unique_ptr<GraphicsTexture>, ETexture::MAX> Textures;
	std::array<std::string, ETexture::MAX> TexturePaths;
};
