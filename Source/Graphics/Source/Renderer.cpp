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
    std::shared_ptr<GShader> SimpleShader = std::shared_ptr<GShader>(new GShader(
        {
            SHADER_PATH("LightSourceShaders/VertexShader.glsl"),
            SHADER_PATH("LightSourceShaders/FragmentShader.glsl")
        }
    ));

    std::shared_ptr<GShader> TexturedShader = std::shared_ptr<GShader>(new GShader(
        {
            SHADER_PATH("Textured/VertexShader.glsl"),
            SHADER_PATH("Textured/FragmentShader.glsl")
        }
    ));

    Shaders.push_back(std::move(SimpleShader));
    Shaders.push_back(std::move(TexturedShader));

    // TODO: Move this to a LightSource class
    for (std::shared_ptr<GShader>& Shader : Shaders)
    {
        const float LightColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        Shader->BufferFloatUniformVector4("LightColour", LightColour);
    }

    std::unique_ptr<GMesh> TriangleMesh = std::make_unique<GMesh>();

    TriangleMesh->AddVertex(
        { 0.0f, 0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.5f, 1.0f }
    );

    TriangleMesh->AddVertex(
        { -0.5f, -0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f }
    );

    TriangleMesh->AddVertex(
        { 0.5f, -0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f }
    );

    TriangleMesh->AddVertex(
        { 0.5f, 0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f }
    );

    TriangleMesh->AddVertex(
        { -0.5f, 0.5f, 0.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f }
    );

    TriangleMesh->SetShader(TriangleMesh->HasTexCoords() ? Shaders[1] : Shaders[0]);

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
    // Front Bottom Left
    LightCubeMesh->AddVertex(
        { -0.5f, -0.5f, 0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    // Front Top Left
    LightCubeMesh->AddVertex(
        { -0.5f, 0.5f, 0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    // Front Top Right
    LightCubeMesh->AddVertex(
        { 0.5f, 0.5f, 0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    // Front Bottom Right
    LightCubeMesh->AddVertex(
        { 0.5f, -0.5f, 0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    // Back Bottom Left
    LightCubeMesh->AddVertex(
        { -0.5f, -0.5f, -0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    // Back Top Left
    LightCubeMesh->AddVertex(
        { -0.5f, 0.5f, -0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    // Back Top Right
    LightCubeMesh->AddVertex(
        { 0.5f, 0.5f, -0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    // Back Bottom Right
    LightCubeMesh->AddVertex(
        { 0.5f, -0.5f, -0.5f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    );
    LightCubeMesh->AddIndices(
        // Front face
        0, 1, 3,
        1, 2, 3,
        // Left face
        4, 5, 0,
        5, 1, 0,
        // Top face
        5, 6, 1,
        6, 2, 1,
        // Right face
        3, 2, 7,
        2, 6, 7,
        // Back face
        7, 6, 4,
        6, 5, 4,
        // Bottom face
        4, 0, 7,
        0, 3, 7
       );

    LightCubeMesh->Translate(glm::vec3(1.0f, 2.0f, -1.0f));
    LightCubeMesh->Scale(0.5f);

    LightCubeMesh->SetShader(LightCubeMesh->HasTexCoords() ? Shaders[1] : Shaders[0]);

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

    for (std::shared_ptr<GShader>& Shader : Shaders)
    {
        Shader->BufferProjectionViewMatrix(CameraPtr->GetProjectionViewMatrix());
    }

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
        Mesh->BufferModelMatrixToShader(Mesh->GetModelMatrix());
        Mesh->BindVertexArray();
        glDrawElements(GL_TRIANGLES, Mesh->GetDrawCount(), GL_UNSIGNED_INT, 0);
    }
}

void GRenderer::Terminate()
{
    glfwTerminate();
}
