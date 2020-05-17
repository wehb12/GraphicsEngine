#include "Graphics/Renderer.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Window.h"
#include "Common/DebugMacros.h"
#include "Common/StringOperations.h"

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

    // TODO: Move this call to Resource Handler
    HelloTriangleShader = std::unique_ptr<GShader>(new GShader(
        {
            SHADER_PATH("HelloTriangle/VertexShader.glsl"),
            SHADER_PATH("HelloTriangle/FragmentShader.glsl")
        }));

    HelloTriangleShader->UseProgram();

    TriangleMesh = std::unique_ptr<GMesh>(new GMesh());
    TriangleMesh->AddVertex(
        { 0.0f, 0.5f, 0.0f },
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.5f, 1.0f }
    );

    TriangleMesh->AddVertex(
        { -0.5f, -0.5f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f }
    );

    TriangleMesh->AddVertex(
        { 0.5f, -0.5f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    );

    TriangleMesh->SetTexture(TEXTURE_PATH("funky_squares.jpg"));

    TriangleMesh->AddIndices(0, 1, 2);

    TriangleMesh->BindBuffers();

    HelloTriangleShader->BufferModelMatrix(TriangleMesh->GetModelMatrix());
}

void GRenderer::Run()
{
    WindowPtr->ProcessInput();

    RenderScene();

    WindowPtr->SwapBuffers();
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
    WindowPtr = GWindow::Get();
}

int GRenderer::PostWindowInit()
{
    WindowPtr->SetAsContext();
    WindowPtr->SetupViewport();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}

void GRenderer::RenderScene()
{
    WindowPtr->ClearToBackground();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GRenderer::Terminate()
{
    glfwTerminate();
}
