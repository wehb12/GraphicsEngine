#include "Graphics/Texture.h"
#include "Common/StringOperations.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image/stb_image.h>

struct GraphicsTexture
{
	GraphicsTexture(const GLuint& InTexture, const unsigned short InTextureUnit)
		: GLTexture(InTexture)
		, GLTextureUnit(InTextureUnit)
	{
	}

	operator GLuint()
	{
		return GLTexture;
	}

	const GLuint GLTexture = 0;
	const unsigned short GLTextureUnit = 0;
};

GTexture::GTexture(const std::string& TexturePath)
{
	GenerateTexture(ETexture::DIFFUSE);
	BindTexture(Textures[ETexture::DIFFUSE]);
	SetTextureParameters();
	LoadTexture(TexturePath);
}

GTexture::~GTexture()
{
}

void GTexture::GenerateTexture(const ETexture& TextureType)
{
	GLuint Texture;
	glGenTextures(1, &Texture);

	Textures[TextureType] = std::make_unique<GraphicsTexture>(Texture, GL_TEXTURE0 + TextureType);
}

void GTexture::BindTexture(std::unique_ptr<GraphicsTexture>& Texture)
{
	glActiveTexture(Texture->GLTextureUnit);
	glBindTexture(GL_TEXTURE_2D, Texture->GLTexture);
}

void GTexture::SetTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

bool GTexture::LoadTexture(const std::string& TexturePath)
{
	int Width;
	int Height;
	int Channels;
	unsigned char* TextureData = stbi_load(TexturePath.c_str(), &Width, &Height, &Channels, 0);

	std::string FileType = TexturePath.substr(TexturePath.rfind('.', std::string::npos));

	unsigned short ColourType = GL_RGB;
	if (CompareString(FileType, "png"))
	{
		ColourType = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, ColourType, GL_UNSIGNED_BYTE, TextureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(TextureData);

	return true;
}
