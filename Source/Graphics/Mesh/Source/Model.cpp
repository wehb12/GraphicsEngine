#include "Graphics/Mesh/Model.h"
#include "Common/DebugMacros.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <iostream>

struct ModelLoaderNode
{
	aiNode* Node;
	const aiScene* Scene;
};

struct ModelLoaderMesh
{
	aiMesh* Mesh;
	const aiScene* Scene;
};

void GModel::LoadModel(const std::string& ModelPath)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(ModelPath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "Failed to load model " << ModelPath << " with error: " << Importer.GetErrorString() << std::endl;
		return;
	}

	ModelDirectory = ModelPath.substr(0, ModelPath.find_last_of('/'));

	ProcessNode({ Scene->mRootNode, Scene });
}

void GModel::Draw()
{
	for (std::shared_ptr<GMesh> Mesh : Meshes)
	{
		Mesh->Draw();
	}
}

void GModel::ProcessNode(ModelLoaderNode Node)
{
	for (unsigned int i = 0; i < Node.Node->mNumMeshes; i++)
	{
		aiMesh* Mesh = Node.Scene->mMeshes[Node.Node->mMeshes[i]];
		Meshes.push_back(ProcessMesh({ Mesh, Node.Scene }));
		Meshes.back()->SetShader(Shader);
	}

	for (unsigned int i = 0; i < Node.Node->mNumChildren; i++)
	{
		ProcessNode({ Node.Node->mChildren[i], Node.Scene });
	}
}

std::vector<std::shared_ptr<GTexture>> GModel::LoadMaterialTextures(aiMaterial* Material, aiTextureType TextureType)
{
	std::vector<std::shared_ptr<GTexture>> Textures;
	for (unsigned int TextureIndex = 0; TextureIndex < Material->GetTextureCount(TextureType); TextureIndex++)
	{
		aiString TexturePath;
		Material->GetTexture(TextureType, 0, &TexturePath);
		std::filesystem::path BaseDir = ModelDirectory.c_str();
		std::filesystem::path FullPath = BaseDir / TexturePath.C_Str();
		std::string FullPathString = FullPath.string();
		if (!LoadedTextures.contains(FullPathString))
		{
			std::shared_ptr<GTexture> Texture = std::make_shared<GTexture>(FullPathString);
			LoadedTextures.emplace(std::make_pair(FullPathString, true));
			Textures.push_back(Texture);
		}
	}
	return Textures;
}

std::shared_ptr<GMesh> GModel::ProcessMesh(ModelLoaderMesh Mesh)
{
	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec3> Tangents;
	std::vector<glm::vec3> Bitangents;
	std::vector<glm::vec2> TexCoords;
	std::vector<unsigned int> Indices;
	std::vector<std::shared_ptr<GTexture>> Textures;

	for (unsigned int i = 0; i < Mesh.Mesh->mNumVertices; i++)
	{
		glm::vec3 Vertex;
		Vertex.x = Mesh.Mesh->mVertices[i].x;
		Vertex.y = Mesh.Mesh->mVertices[i].y;
		Vertex.z = Mesh.Mesh->mVertices[i].z;
		Vertices.push_back(Vertex);

		glm::vec3 Normal;
		Normal.x = Mesh.Mesh->mNormals[i].x;
		Normal.y = Mesh.Mesh->mNormals[i].y;
		Normal.z = Mesh.Mesh->mNormals[i].z;
		Normals.push_back(Normal);

		if (Mesh.Mesh->mTextureCoords[0])
		{
			glm::vec2 TexCoord;
			TexCoord.x = Mesh.Mesh->mTextureCoords[0][i].x;
			TexCoord.y = Mesh.Mesh->mTextureCoords[0][i].y;
			TexCoords.push_back(TexCoord);

			glm::vec3 Tangent;
			Tangent.x = Mesh.Mesh->mTangents[i].x;
			Tangent.y = Mesh.Mesh->mTangents[i].y;
			Tangent.z = Mesh.Mesh->mTangents[i].z;
			Tangents.push_back(Tangent);

			glm::vec3 Bitangent;
			Bitangent.x = Mesh.Mesh->mBitangents[i].x;
			Bitangent.y = Mesh.Mesh->mBitangents[i].y;
			Bitangent.z = Mesh.Mesh->mBitangents[i].z;
			Bitangents.push_back(Bitangent);
		}
	}

	for (unsigned int i = 0; i < Mesh.Mesh->mNumFaces; i++)
	{
		aiFace Face = Mesh.Mesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}

	if (Mesh.Mesh->mMaterialIndex >= 0)
	{
		aiMaterial* Material = Mesh.Scene->mMaterials[Mesh.Mesh->mMaterialIndex];

		std::vector<std::shared_ptr<GTexture>> DiffuseMaps = LoadMaterialTextures(Material, aiTextureType_DIFFUSE);
		Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

		std::vector<std::shared_ptr<GTexture>> SpecularMaps = LoadMaterialTextures(Material, aiTextureType_SPECULAR);
		Textures.insert(Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
	}

	return std::make_shared<GMesh>(Vertices, Normals, TexCoords, Indices, Textures, Tangents);
}
