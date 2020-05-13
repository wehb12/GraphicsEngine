#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Window/Window.h"

bool ShouldClose();

int WinMain()
{
	GRenderer Renderer;
	Renderer.Init();

	while (!ShouldClose())
	{
		Renderer.Run();
	}

	return 0;
}

bool ShouldClose()
{
	return GWindow::Get()->ShouldClose();
}
