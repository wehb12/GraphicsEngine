#pragma once

#include <memory>

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

	void RenderScene();

	void Terminate();

private:
	std::unique_ptr<GWindow> Window;
};
