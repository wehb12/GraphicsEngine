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
#include <fstream>

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

	const glm::vec3 LightPosition = glm::vec3(1.5f, 1.0f, -4.5f);

    // TODO: Move this to a LightSource class Tick()
    for (std::shared_ptr<GShader>& Shader : Shaders)
    {
        const float LightColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		Shader->BufferFloatUniformVector4("LightColour", LightColour);
		Shader->BufferFloatUniformVector3("LightPosition", &LightPosition[0]);
    }

	Shaders[1]->UseProgram();
	std::shared_ptr<GTexture> FunkySquaresTexture = std::make_shared<GTexture>(TEXTURE_PATH("funky_squares.jpg"));
	std::unique_ptr<GCubeMesh> CubeMesh = std::make_unique<GCubeMesh>(2.0f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)/*, FunkySquaresTexture*/);

    CubeMesh->SetShader(CubeMesh->HasTexCoords() ? Shaders[2] : Shaders[1]);

	CubeMesh->SetTranslation({0.0f, -1.0f, -3.0f});

    CubeMesh->BindBuffers();

    std::unique_ptr<GCubeMesh> LightCubeMesh = std::make_unique<GCubeMesh>(0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), nullptr);

    LightCubeMesh->SetTranslation(LightPosition);

    LightCubeMesh->SetShader(Shaders[0]);

    LightCubeMesh->BindBuffers();

    Meshes.push_back(std::move(CubeMesh));
    Meshes.push_back(std::move(LightCubeMesh));
}

void GRenderer::Tick(const float& DeltaTime)
{
	Meshes[1]->RotateAroundPoint(DeltaTime, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f));

    for (std::unique_ptr<GMesh>& Mesh : Meshes)
    {
        Mesh->Tick(DeltaTime);
    }

    for (std::shared_ptr<GShader>& Shader : Shaders)
    {
		float Pos[3] =
		{
			Meshes[1]->GetPosition()[0],
			Meshes[1]->GetPosition()[1],
			Meshes[1]->GetPosition()[2]
		};
		Shader->BufferFloatUniformVector3("LightPosition", Pos);
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
		Mesh->Draw();
    }
}

void GRenderer::Terminate()
{
    glfwTerminate();
}
