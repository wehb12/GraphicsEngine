#include "Graphics/Renderer.h"
#include "Graphics/Window.h"

bool ShouldClose();

int WinMain()
{
	GRenderer Renderer;
	Renderer.Init();

	while (!ShouldClose())
	{
		Renderer.Tick();
	}

	return 0;
}

bool ShouldClose()
{
	return GWindow::Get()->ShouldClose();
}
