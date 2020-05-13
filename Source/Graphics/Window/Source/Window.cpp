#include "Graphics/Window/Window.h"
#include "Common/DebugMacros.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<GWindow> GWindow::WindowSingleton;

GWindow::GWindow(const std::string& InName, const int InWindowWidth /*= 800*/, const int InWindowHeight /*= 600*/)
    : WindowName(InName)
    , WindowWidth(InWindowWidth)
    , WindowHeight(InWindowHeight)
{
    GraphicsWindow = glfwCreateWindow(WindowWidth, WindowHeight, WindowName.c_str(), NULL, NULL);

    ASSERT_FAIL(GraphicsWindow, glfwTerminate());
};

GWindow::~GWindow()
{
    glfwDestroyWindow(GraphicsWindow);
}

void GWindow::SetAsContext()
{
    glfwMakeContextCurrent(GraphicsWindow);
}

void GWindow::SetupViewport()
{
    glfwSetFramebufferSizeCallback(GraphicsWindow, GWindow::ViewPortSizeCallback);
}

void GWindow::ClearToBackground()
{
    glClearColor(BackGroundColour[0], BackGroundColour[1], BackGroundColour[2], BackGroundColour[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool GWindow::ShouldClose()
{
    return glfwWindowShouldClose(GraphicsWindow);
}

void GWindow::SwapBuffers()
{
    glfwSwapBuffers(GraphicsWindow);
    glfwPollEvents();
}

void GWindow::ProcessInput()
{
    if (glfwGetKey(GraphicsWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(GraphicsWindow, true);
    }
}

void GWindow::ViewPortSizeCallback(GGraphicsWindow* Window, int NewWidth, int NewHeight)
{
    glViewport(0, 0, NewWidth, NewHeight);
}