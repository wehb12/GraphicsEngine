#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
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

    EnableDepthTest();

    // TODO: Move this call to Resource Handler
    HelloTriangleShader = std::unique_ptr<GShader>(new GShader(
        {
            SHADER_PATH("HelloTriangle/VertexShader.glsl"),
            SHADER_PATH("HelloTriangle/FragmentShader.glsl")
        }));

    HelloTriangleShader->UseProgram();

    std::unique_ptr<GMesh> TriangleMesh = std::make_unique<GMesh>();

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

    TriangleMesh->AddVertex(
        { 0.5f, 0.5f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    );

    TriangleMesh->AddVertex(
        { -0.5f, 0.5f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    );

    TriangleMesh->SetTexture(TEXTURE_PATH("funky_squares.jpg"));

    // Triangle
    //TriangleMesh->AddIndices(0, 1, 2);

    // Rectangle
    TriangleMesh->AddIndices(1, 4, 2, 2, 4, 3);

    TriangleMesh->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    TriangleMesh->Scale(3.0f);

    TriangleMesh->BindBuffers();

    std::unique_ptr<GMesh> LightCubeMesh = std::make_unique<GMesh>();

    LightCubeMesh = std::unique_ptr<GMesh>(new GMesh());
    LightCubeMesh->AddVertex(
        { -0.5f, -0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    LightCubeMesh->AddVertex(
        { -0.5f, 0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    LightCubeMesh->AddVertex(
        { 0.5f, 0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    LightCubeMesh->AddVertex(
        { 0.5f, -0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    LightCubeMesh->AddIndices(0, 1, 3, 1, 2, 3);
    LightCubeMesh->BindBuffers();

    Meshes.push_back(std::move(TriangleMesh));
    Meshes.push_back(std::move(LightCubeMesh));
}

void GRenderer::Tick(const float& DeltaTime)
{
    for (std::unique_ptr<GMesh>& Mesh : Meshes)
    {
        Mesh->Tick(DeltaTime);
    }

    HelloTriangleShader->BufferProjectionViewMatrix(CameraPtr->GetProjectionViewMatrix());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void GRenderer::EnableDepthTest(bool bEnable)
{
    if (bEnable)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void GRenderer::RenderScene()
{
    WindowPtr->ClearToBackground();

    for (std::unique_ptr<GMesh>& Mesh : Meshes)
    {
        HelloTriangleShader->BufferModelMatrix(Mesh->GetModelMatrix());
        Mesh->BindVertexArray();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void GRenderer::Terminate()
{
    glfwTerminate();
}
