#include "Graphics/Camera.h"
#include "Input/InputManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Window.h"

#include <memory>

bool ShouldClose();

int WinMain()
{
	GRenderer Renderer;
	Renderer.Init();

	// Init Window singleton (bind keypress delegates)
	GWindow::Get()->Init();

	std::shared_ptr<GCamera> Camera = std::make_shared<GCamera>();
	Renderer.AddCamera(Camera);

	while (!ShouldClose())
	{
		Camera->Tick();
		Renderer.Tick();
	}

	return 0;
}

bool ShouldClose()
{
	return GWindow::Get()->ShouldClose();
}
