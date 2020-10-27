#include "Graphics/Mesh/Model.h"
#include "Common/DebugMacros.h"
#include "Common/Vector.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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
	const aiScene* Scene = Importer.ReadFile(ModelPath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

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
	}

	for (unsigned int i = 0; i < Node.Node->mNumChildren; i++)
	{
		ProcessNode({ Node.Node->mChildren[i], Node.Scene });
	}
}

std::vector<std::shared_ptr<GTexture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<std::shared_ptr<GTexture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::shared_ptr<GTexture> texture = std::make_shared<GTexture>(str.C_Str());
		//texture.id = TextureFromFile(str.C_Str(), directory);
		//texture.type = typeName;
		//texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}

std::shared_ptr<GMesh> GModel::ProcessMesh(ModelLoaderMesh Mesh)
{
	std::vector<GVector3> Vertices;
	std::vector<GVector3> Normals;
	std::vector<GVector2> TexCoords;
	std::vector<unsigned int> Indices;
	std::vector<std::shared_ptr<GTexture>> Textures;

	for (unsigned int i = 0; i < Mesh.Mesh->mNumVertices; i++)
	{
		GVector3 Vertex;
		Vertex.X = Mesh.Mesh->mVertices[i].x;
		Vertex.Y = Mesh.Mesh->mVertices[i].y;
		Vertex.Z = Mesh.Mesh->mVertices[i].z;
		Vertices.push_back(Vertex);

		GVector3 Normal;
		Normal.X = Mesh.Mesh->mNormals[i].x;
		Normal.Y = Mesh.Mesh->mNormals[i].y;
		Normal.Z = Mesh.Mesh->mNormals[i].z;
		Normals.push_back(Normal);

		if (Mesh.Mesh->mTextureCoords[0])
		{
			GVector2 TexCoord;
			TexCoord.X = Mesh.Mesh->mTextureCoords[0][i].x;
			TexCoord.Y = Mesh.Mesh->mTextureCoords[0][i].y;
			TexCoords.push_back(TexCoord);
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

		std::vector<std::shared_ptr<GTexture>> DiffuseMaps = loadMaterialTextures(Material, aiTextureType_DIFFUSE, "texture_diffuse");
		Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

		std::vector<std::shared_ptr<GTexture>> SpecularMaps = loadMaterialTextures(Material, aiTextureType_SPECULAR, "texture_specular");
		Textures.insert(Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
	}

	return std::make_shared<GMesh>(Vertices, Normals, TexCoords, Indices, Textures);
}
