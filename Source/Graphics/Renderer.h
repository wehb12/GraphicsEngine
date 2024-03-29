#pragma once

#include <memory>
#include <string>
#include <vector>

class GCamera;
class GMesh;
class GModel;
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

	std::vector<std::shared_ptr<GShader>> Shaders;

	std::vector<std::unique_ptr<GMesh>> Meshes;

	std::shared_ptr<const GCamera> CameraPtr;

	std::shared_ptr<GModel> Model;
};
