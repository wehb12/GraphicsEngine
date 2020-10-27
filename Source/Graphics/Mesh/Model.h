#pragma once

#include <memory>
#include <string>
#include <vector>

class GMesh;
struct ModelLoaderMesh;
struct ModelLoaderNode;

class GModel
{
public:
	GModel(const std::string& ModelPath)
	{
		LoadModel(ModelPath);
	}

	void Draw();

private:
	void LoadModel(const std::string& ModelPath);

	void ProcessNode(ModelLoaderNode Node);

	std::shared_ptr<GMesh> ProcessMesh(ModelLoaderMesh Mesh);

private:
	std::vector<std::shared_ptr<GMesh>> Meshes;
	std::string ModelDirectory;
};
