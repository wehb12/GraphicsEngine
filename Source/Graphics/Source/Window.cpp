#include "Graphics/Window.h"
#include "Common/DebugMacros.h"
#include "Input/InputManager.h"

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

    glfwSetInputMode((GLFWwindow*)GraphicsWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};

GWindow::~GWindow()
{
    glfwDestroyWindow((GLFWwindow*)GraphicsWindow);
}

void GWindow::Init()
{
    IInputManager::Get()->BindDelegate(EInputKey::ESCAPE, this, &GWindow::CloseWindow);
}

void GWindow::SetAsContext()
{
    glfwMakeContextCurrent((GLFWwindow*)GraphicsWindow);
}

void ViewPortSizeCallback(GLFWwindow* Window, int NewWidth, int NewHeight)
{
    glViewport(0, 0, NewWidth, NewHeight);
}

void GWindow::SetupViewport()
{
    glfwSetFramebufferSizeCallback((GLFWwindow*)GraphicsWindow, ViewPortSizeCallback);
}

void GWindow::ClearToBackground()
{
    glClearColor(BackGroundColour[0], BackGroundColour[1], BackGroundColour[2], BackGroundColour[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool GWindow::ShouldClose()
{
    return glfwWindowShouldClose((GLFWwindow*)GraphicsWindow);
}

void GWindow::SwapBuffers()
{
    glfwSwapBuffers((GLFWwindow*)GraphicsWindow);
    glfwPollEvents();
}

void GWindow::CloseWindow()
{
    glfwSetWindowShouldClose((GLFWwindow*)GraphicsWindow, true);
}
