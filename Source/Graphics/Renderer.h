#pragma once

#include <vector>
#include <memory>
#include <string>

class GMesh;
class GShader;
class GWindow;

class GRenderer
{
public:
	GRenderer();

	~GRenderer();

	void Init();

	void Run();

private:
	void InitGraphics();

	void InitWindow();

	void MakeWindow();

	int PostWindowInit();

	// Vertex buffer operations
	void BindElementBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject);

	void RenderScene();

	void Terminate();

private:
	std::shared_ptr<GWindow> WindowPtr;

	std::unique_ptr<GShader> HelloTriangleShader;

	std::unique_ptr<GMesh> TriangleMesh;
};
