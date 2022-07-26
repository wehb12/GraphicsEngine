#pragma once

#include <memory>
#include <string>

struct GGraphicsWindow;

class GWindow
{
public:
	friend class IInputManager;

	~GWindow();

	inline static std::shared_ptr<GWindow> Get()
	{
		return WindowSingleton ? WindowSingleton : (WindowSingleton = std::shared_ptr<GWindow>(new GWindow("Hello Triangle!")));
	}

	void Init();

	void SetAsContext();

	void SetupViewport();

	void ClearToBackground();

	bool ShouldClose();

	void SwapBuffers();

	void CloseWindow();

private:
	GWindow(const std::string& InName, const int InWindowWidth = 1600, const int InWindowHeight = 1200);

	static std::shared_ptr<GWindow> WindowSingleton;

	inline void* GetRawWindowPtr()
	{
		return (void*)GraphicsWindow;
	}
    
protected:
	std::string WindowName;
	int WindowWidth = 0;
	int WindowHeight = 0;
	float BackGroundColour[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

private:
	void* GraphicsWindow;
};
