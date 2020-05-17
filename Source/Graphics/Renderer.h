#pragma once

#include <vector>
#include <memory>
#include <string>

class GCamera;
class GMesh;
class GShader;
class GWindow;

class GRenderer
{
public:
	GRenderer();

	~GRenderer();

	void Init();

	void Tick();

	inline void AddCamera(const std::shared_ptr<const GCamera> Camera)
	{
		CameraPtr = Camera;
	}

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

	std::shared_ptr<const GCamera> CameraPtr;
};
