#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <string>

#define ASSERT(Statement) if (!Statement) __debugbreak;
#define ASSERT_FAIL(Statement, Fail) if (!Statement) { __debugbreak; Fail; }
#define PAUSE system("pause")

typedef GLFWwindow GRawWindow;

class GWindow
{
public:
	GWindow(const std::string& InName, const int InWindowWidth = 800, const int InWindowHeight = 600)
		: WindowName(InName)
		, WindowWidth(InWindowWidth)
		, WindowHeight(InWindowHeight)
	{
		RawWindow = glfwCreateWindow(WindowWidth, WindowHeight, WindowName.c_str(), NULL, NULL);

		ASSERT_FAIL(RawWindow, glfwTerminate());
	};

	~GWindow()
	{
		glfwDestroyWindow(RawWindow);
	}

	void SetAsContext()
	{
		glfwMakeContextCurrent(RawWindow);
	}

	void SetupViewport()
	{
		glfwSetFramebufferSizeCallback(RawWindow, GWindow::ViewPortSizeCallback);
	}

	void ClearToBackground()
	{
		glClearColor(BackGroundColour[0], BackGroundColour[1], BackGroundColour[2], BackGroundColour[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	inline bool ShouldClose()
	{
		return glfwWindowShouldClose(RawWindow);
	}

	void SwapBuffers()
	{
		glfwSwapBuffers(RawWindow);
		glfwPollEvents();
	}

	void ProcessInput()
	{
		if (glfwGetKey(RawWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(RawWindow, true);
		}
	}

private:
	static void ViewPortSizeCallback(GRawWindow* Window, int NewWidth, int NewHeight)
	{
		glViewport(0, 0, NewWidth, NewHeight);
	}

protected:
	std::string WindowName;
	int WindowWidth = 0;
	int WindowHeight = 0;
	float BackGroundColour[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

private:
	GRawWindow* RawWindow;
};

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

int main()
{
	GRenderer Renderer;
	Renderer.Init();
	Renderer.Run();

	return 0;
}
