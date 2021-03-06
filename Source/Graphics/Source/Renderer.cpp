#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh/MaterialExamples.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/CubeMesh.h"
#include "Graphics/Mesh/PlaneMesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Window.h"
#include "Common/DebugMacros.h"
#include "Common/StringOperations.h"
#include "Input/InputManager.h"

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

	IInputManager::Get()->BindDelegate(EInputKey::F1, [this]()
		{
			const glm::vec3 LightPosition = glm::vec3(1.5f, 1.0f, -4.5f);
			const float LightAmbient[3] = { 0.2f, 0.2f, 0.2f };
			const float LightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
			const float LightSpecular[3] = { 1.0f, 1.0f, 1.0f };

			for (std::shared_ptr<GShader>& Shader : Shaders)
			{
				Shader->Recompile();

				// TODO: Move this to a LightSource class Tick()
				Shader->BufferFloatUniformVector3("Light.Ambient", LightAmbient);
				Shader->BufferFloatUniformVector3("Light.Diffuse", LightDiffuse);
				Shader->BufferFloatUniformVector3("Light.Specular", LightSpecular);
				Shader->BufferFloatUniformVector3("Light.Position", &LightPosition[0]);
			}
		}
	);

	Shaders.push_back(std::move(LightSourceShader));
	Shaders.push_back(std::move(SimpleShader));
    Shaders.push_back(std::move(TexturedShader));

	const glm::vec3 LightPosition = glm::vec3(1.5f, 1.0f, -4.5f);
	const float LightAmbient[3] = { 0.2f, 0.2f, 0.2f };
	const float LightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
	const float LightSpecular[3] = { 1.0f, 1.0f, 1.0f };

    // TODO: Move this to a LightSource class Tick()
    for (std::shared_ptr<GShader>& Shader : Shaders)
    {
		Shader->BufferFloatUniformVector3("Light.Ambient", LightAmbient);
		Shader->BufferFloatUniformVector3("Light.Diffuse", LightDiffuse);
		Shader->BufferFloatUniformVector3("Light.Specular", LightSpecular);
		Shader->BufferFloatUniformVector3("Light.Position", &LightPosition[0]);
    }

	Shaders[2]->UseProgram();
	std::unique_ptr<GCubeMesh> CubeMesh = std::make_unique<GCubeMesh>(2.0f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

    CubeMesh->SetShader(CubeMesh->HasTexCoords() ? Shaders[2] : Shaders[1]);
	CubeMesh->SetTexture(TEXTURE_PATH("WoodGate/WoodGate"));

	CubeMesh->SetTranslation({0.0f, -1.0f, -3.0f});

	CubeMesh->SetMaterial(MaterialExamples::Gold);

    CubeMesh->BindBuffers();

    std::unique_ptr<GCubeMesh> LightCubeMesh = std::make_unique<GCubeMesh>(0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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
		float LightPos[3] =
		{
			Meshes[1]->GetPosition()[0],
			Meshes[1]->GetPosition()[1],
			Meshes[1]->GetPosition()[2]
		};
		Shader->BufferFloatUniformVector3("Light.Position", LightPos);

		const glm::vec3 CameraPosVector = CameraPtr->GetPosition();
		float CameraPos[3] =
		{
			CameraPosVector[0],
			CameraPosVector[1],
			CameraPosVector[2]
		};
		Shader->BufferFloatUniformVector3("CameraPosition", CameraPos);

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
