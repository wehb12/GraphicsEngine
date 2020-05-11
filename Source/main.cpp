#include "Graphics/Window/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <string>

class GRenderer
{
public:
	~GRenderer()
	{
		Terminate();
	}

	void Init()
	{
		InitGraphics();
		InitWindow();

		MakeWindow();

		PostWindowInit();
	}

	void Run()
	{
		while (!Window->ShouldClose())
		{
			Window->ProcessInput();

			RenderScene();

			Window->SwapBuffers();
		}
	}

private:
	void InitGraphics()
	{
		glfwInit();
	}

	void InitWindow()
	{
		// Tell GLFW to use GL version 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Needed on OS X
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	}

	void MakeWindow()
	{
		Window = std::unique_ptr<GWindow>(new GWindow("Hello Triangle!"));
	}

	int PostWindowInit()
	{
		Window->SetAsContext();
		Window->SetupViewport();

		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		return 0;
	}

	void RenderScene()
	{
		Window->ClearToBackground();
	}

	void Terminate()
	{
		glfwTerminate();
	}

private:
	std::unique_ptr<GWindow> Window;
};

int WinMain()
{
	GRenderer Renderer;
	Renderer.Init();
	Renderer.Run();

	return 0;
}
