#include "Core/Timing/GlobalTimer.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Graphics/Window.h"
#include "Input/InputManager.h"

#include <memory>

bool ShouldClose();

int WinMain()
{
	// Create global timer singleton and lock in start time
	CGlobalTimer::Get();

	GRenderer Renderer;
	Renderer.Init();

	// Init Window singleton (bind keypress delegates)
	GWindow::Get()->Init();

	std::shared_ptr<GCamera> Camera = std::make_shared<GCamera>();
	Renderer.AddCamera(Camera);

	while (!ShouldClose())
	{
		CGlobalTimer::Get()->Tick();

		const float DeltaTime = (float)CGlobalTimer::Get()->GetFrameTime();
		Camera->Tick(DeltaTime);
		Renderer.Tick(DeltaTime);
	}

	return 0;
}

bool ShouldClose()
{
	return GWindow::Get()->ShouldClose();
}
