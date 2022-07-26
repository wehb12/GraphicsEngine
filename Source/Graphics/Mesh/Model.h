#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

enum aiTextureType;
class GMesh;
class GShader;
class GTexture;
struct aiMaterial;
struct ModelLoaderMesh;
struct ModelLoaderNode;

class GModel
{
public:
	GModel(const std::string& ModelPath, std::shared_ptr<GShader> InShader)
		: Shader(InShader)
	{
		LoadModel(ModelPath);
	}

	void Draw();

private:
	void LoadModel(const std::string& ModelPath);

	void ProcessNode(ModelLoaderNode Node);

	std::shared_ptr<GMesh> ProcessMesh(ModelLoaderMesh Mesh);

	std::vector<std::shared_ptr<GTexture>> LoadMaterialTextures(aiMaterial* Material, aiTextureType TextureType);

private:
	std::vector<std::shared_ptr<GMesh>> Meshes;
	std::string ModelDirectory;
	std::shared_ptr<GShader> Shader;

	/*
	 * Some resource management so textures aren't loaded for every mesh in the Model
	 * Remove when resource manager is created
	 */
	std::map<std::string, bool> LoadedTextures;
};
