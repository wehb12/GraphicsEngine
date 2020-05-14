#pragma once

#include <memory>
#include <string>

struct GLFWwindow;
typedef GLFWwindow GGraphicsWindow;

class GWindow
{
public:
	~GWindow();

	inline static std::shared_ptr<GWindow> Get()
	{
		return WindowSingleton ? WindowSingleton : (WindowSingleton = std::shared_ptr<GWindow>(new GWindow("Hello Triangle!")));
	}

	void SetAsContext();

	void SetupViewport();

	void ClearToBackground();

	bool ShouldClose();

	void SwapBuffers();

	void ProcessInput();

private:
	GWindow(const std::string& InName, const int InWindowWidth = 800, const int InWindowHeight = 600);

	static std::shared_ptr<GWindow> WindowSingleton;

	static void ViewPortSizeCallback(GGraphicsWindow* Window, int NewWidth, int NewHeight);
    
protected:
	std::string WindowName;
	int WindowWidth = 0;
	int WindowHeight = 0;
	float BackGroundColour[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

private:
	GGraphicsWindow* GraphicsWindow;
};
