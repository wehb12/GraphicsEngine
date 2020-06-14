#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/CubeMesh.h"
#include "Graphics/Mesh/PlaneMesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
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
    std::shared_ptr<GShader> LightSourceShader = std::shared_ptr<GShader>(new GShader(
        {
            SHADER_PATH("LightSourceShaders/VertexShader.glsl"),
            SHADER_PATH("LightSourceShaders/FragmentShader.glsl")
        }
    ));

	std::shared_ptr<GShader> SimpleShader = std::shared_ptr<GShader>(new GShader(
		{
			SHADER_PATH("Simple/VertexShader.glsl"),
			SHADER_PATH("Simple/FragmentShader.glsl")
		}
	));

    std::shared_ptr<GShader> TexturedShader = std::shared_ptr<GShader>(new GShader(
        {
            SHADER_PATH("Textured/VertexShader.glsl"),
            SHADER_PATH("Textured/FragmentShader.glsl")
        }
    ));

	Shaders.push_back(std::move(LightSourceShader));
	Shaders.push_back(std::move(SimpleShader));
    Shaders.push_back(std::move(TexturedShader));

    // TODO: Move this to a LightSource class
    for (std::shared_ptr<GShader>& Shader : Shaders)
    {
        const float LightColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        Shader->BufferFloatUniformVector4("LightColour", LightColour);
    }

	Shaders[1]->UseProgram();
	std::shared_ptr<GTexture> FunkySquaresTexture = std::make_shared<GTexture>(TEXTURE_PATH("funky_squares.jpg"));
	std::unique_ptr<GCubeMesh> CubeMesh = std::make_unique<GCubeMesh>(2.0f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)/*, FunkySquaresTexture*/);

    CubeMesh->SetShader(CubeMesh->HasTexCoords() ? Shaders[2] : Shaders[1]);

    CubeMesh->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    CubeMesh->BindBuffers();

    std::unique_ptr<GCubeMesh> LightCubeMesh = std::make_unique<GCubeMesh>(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);

    LightCubeMesh->Translate(glm::vec3(1.0f, 3.0f, -2.0f));

    LightCubeMesh->SetShader(Shaders[0]);

    LightCubeMesh->BindBuffers();

    Meshes.push_back(std::move(CubeMesh));
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
