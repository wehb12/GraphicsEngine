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
		MAX
	};

public:
	GTexture(const std::string& TexturePath);

	~GTexture();

	void BufferToShader(std::shared_ptr<GShader> Shader);

private:
	void GenerateTexture(const ETexture& TextureType);

	void BindTexture(std::unique_ptr<GraphicsTexture>& Texture);

	// Sets parameters for currently bound texture unit
	void SetTextureParameters();

	// Load texture from disc and buffer to graphics card
	bool LoadTexture(const std::string& TexturePath);

private:
	std::array<std::unique_ptr<GraphicsTexture>, ETexture::MAX> Textures;
};
