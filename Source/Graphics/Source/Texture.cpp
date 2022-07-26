#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Common/DebugMacros.h"
#include "Common/StringOperations.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image/stb_image.h>

#include <fstream>

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
	stbi_set_flip_vertically_on_load(1);
	GetTexturePaths(TexturePath);
	GenerateTextures();
	BindTextures();
	LoadTextures();
}

GTexture::~GTexture()
{
}

void GTexture::BufferToShader(std::shared_ptr<GShader> Shader)
{
	Shader->BufferIntUniform("Material.Diffuse", 0);
	Shader->BufferIntUniform("Material.Specular", 1);
	Shader->BufferIntUniform("Material.Normal", 2);
	BindTextures();
}

void GTexture::GenerateTextures()
{
	for (unsigned int TextureType = ETexture::DIFFUSE; TextureType < ETexture::MAX; ++TextureType)
	{
		GLuint Texture;
		glGenTextures(1, &Texture);

		Textures[TextureType] = std::make_shared<GraphicsTexture>(Texture, GL_TEXTURE0 + TextureType);
	}
}

void GTexture::BindTextures()
{
	for (std::shared_ptr<GraphicsTexture>& Texture : Textures)
	{
		glActiveTexture(Texture->GLTextureUnit);
		glBindTexture(GL_TEXTURE_2D, Texture->GLTexture);

		SetTextureParameters();
	}
}

void GTexture::SetTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

bool GTexture::LoadTextures()
{
	for (unsigned int TextureType = ETexture::DIFFUSE; TextureType < ETexture::MAX; ++TextureType)
	{
		const std::string SubTexturePath = TexturePaths[TextureType];

		int Width;
		int Height;
		int Channels;
		unsigned char* TextureData = stbi_load(SubTexturePath.c_str(), &Width, &Height, &Channels, 0);

		const char* FailureReason = stbi_failure_reason();
		ASSERT(TextureData);

		std::string FileType = SubTexturePath.substr(SubTexturePath.rfind('.', std::string::npos));

		unsigned short ColourType = GL_RGB;
		if (CompareString(FileType, "png"))
		{
			ColourType = GL_RGBA;
		}

		glActiveTexture(Textures[TextureType]->GLTextureUnit);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, ColourType, GL_UNSIGNED_BYTE, TextureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(TextureData);
	}

	return true;
}

void GTexture::GetTexturePaths(const std::string& TexturePath)
{
	for (unsigned int TextureType = ETexture::DIFFUSE; TextureType < ETexture::MAX; ++TextureType)
	{
		const std::string SubTexturePath = TexturePath + "_" + TextureAppendices[TextureType];
		std::ifstream TextureFile;
		TextureFile.open(SubTexturePath + ".jpg");
		if (TextureFile)
		{
			TexturePaths[TextureType] = SubTexturePath + ".jpg";
		}
		else
		{
			TextureFile.open(SubTexturePath + ".png");
			if (TextureFile)
			{
				TexturePaths[TextureType] = SubTexturePath + ".png";
			}
			else
			{
				// TODO: No texture found - handle this
			}
		}

		TextureFile.close();
	}
}
