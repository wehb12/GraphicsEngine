#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

GRenderer::GRenderer()
{
}

GRenderer::~GRenderer()
{
    Terminate();
}

void GRenderer::Init()
{
    InitGraphics();
    InitWindow();

    MakeWindow();

    PostWindowInit();
}

void GRenderer::Run()
{
    while (!Window->ShouldClose())
    {
        Window->ProcessInput();

        RenderScene();

        Window->SwapBuffers();
    }
}

void GRenderer::InitGraphics()
{
    glfwInit();
}

void GRenderer::InitWindow()
{
    // Tell GLFW to use GL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Needed on OS X
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void GRenderer::MakeWindow()
{
    Window = std::unique_ptr<GWindow>(new GWindow("Hello Triangle!"));
}

int GRenderer::PostWindowInit()
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

void GRenderer::RenderScene()
{
    Window->ClearToBackground();
}

void GRenderer::Terminate()
{
    glfwTerminate();
}
