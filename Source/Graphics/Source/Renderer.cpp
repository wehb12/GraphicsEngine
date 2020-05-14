#include "Graphics/Renderer.h"
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

    float Vertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    float Colours[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    unsigned int Indices[] = {
        0, 1, 2
    };

    GenerateVertexArray();
    BindVertexArray(VertexArrayObject);

    GenerateVertexBuffers();

    BindVertexBuffer(Vertices, sizeof(Vertices), VertexBufferObjects[EVertexBuffer::VERTEX_BUFFER]);
    SetVertexAttributePointer(EVertexBuffer::VERTEX_BUFFER, 3);

    BindVertexBuffer(Colours, sizeof(Colours), VertexBufferObjects[EVertexBuffer::COLOUR_BUFFER]);
    SetVertexAttributePointer(EVertexBuffer::COLOUR_BUFFER, 4);

    GenerateElementBuffer();
    BindElementBuffer(Indices, sizeof(Indices), IndexBufferObject);
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

void GRenderer::GenerateVertexBuffers()
{
    for (unsigned int& VertexBufferObject : VertexBufferObjects)
    {
        glGenBuffers(1, &VertexBufferObject);
    }
}

void GRenderer::GenerateElementBuffer()
{
    glGenBuffers(1, &IndexBufferObject);
}

void GRenderer::GenerateVertexArray()
{
    glGenVertexArrays(1, &VertexArrayObject);
}

void GRenderer::BindVertexArray(unsigned int& VAO)
{
    glBindVertexArray(VAO);
}

void GRenderer::BindVertexBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject)
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
    glBufferData(GL_ARRAY_BUFFER, ArraySize, Array, GL_STATIC_DRAW);
}

void GRenderer::BindElementBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ArraySize, Array, GL_STATIC_DRAW);
}

void GRenderer::SetVertexAttributePointer(const EVertexBuffer::Type& VertexBufferType, const const unsigned int& VectorSize)
{
    glVertexAttribPointer(VertexBufferType, VectorSize, GL_FLOAT, GL_FALSE, VectorSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(VertexBufferType);
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
