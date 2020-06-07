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

	void Tick(const float& DeltaTime);

	inline void AddCamera(const std::shared_ptr<const GCamera> Camera)
	{
		CameraPtr = Camera;
	}

private:
	void InitGraphics();

	void InitWindow();

	void MakeWindow();

	int PostWindowInit();

	void EnableDepthTest(bool bEnable = true);

	void RenderScene();

	void Terminate();

private:
	std::shared_ptr<GWindow> WindowPtr;

	std::unique_ptr<GShader> HelloTriangleShader;

	std::vector<std::unique_ptr<GMesh>> Meshes;

	std::shared_ptr<const GCamera> CameraPtr;
};
