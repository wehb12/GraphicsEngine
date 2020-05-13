#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Window/Window.h"
#include "Common/DebugMacros.h"
#include "Common/StringOperations.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>

class GraphicsShader
{
public:
    GraphicsShader()
    {
    }

    GraphicsShader(const GLuint& InShader)
        : GLShader(InShader)
    {
    }

    operator GLuint()
    {
        return GLShader;
    }

    GLuint GLShader = 0;
};

GRenderer::GRenderer()
{
}

GRenderer::~GRenderer()
{
    glDeleteShader(*VertexShaderPtr);
    glDeleteShader(*FragmentShaderPtr);
    glDeleteProgram(*ShaderProgramPtr);

    VertexShaderPtr.release();
    FragmentShaderPtr.release();
    ShaderProgramPtr.release();

    Terminate();
}

void GRenderer::Init()
{
    InitGraphics();
    InitWindow();

    MakeWindow();

    PostWindowInit();

    LoadShaders();
    CompileShaders();

    float Vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    unsigned int Indices[] = {
        0, 2, 1,
        2, 3, 1
    };

    GenerateVertexArray();
    BindVertexArray(VertexArrayObject);

    GenerateVertexBuffer();
    BindVertexBuffer(Vertices, sizeof(Vertices), VertexBufferObject);

    GenerateElementBuffer();
    BindElementBuffer(Indices, sizeof(Indices), IndexBufferObject);

    SetVertexAttributePointer();
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

void GRenderer::LoadShaders()
{
    VertexShaderSource = LoadShader(SHADER_PATH("HelloTriangle/VertexShader.glsl"));
    FragmentShaderSource = LoadShader(SHADER_PATH("HelloTriangle/FragmentShader.glsl"));
}

std::string GRenderer::LoadShader(const char* ShaderPath)
{
    // TODO: Move to Resource Handler
    std::ifstream ShaderFile(ShaderPath);
    return std::string((std::istreambuf_iterator<char>(ShaderFile)), std::istreambuf_iterator<char>());
}

void GRenderer::CompileShaders()
{
    VertexShaderPtr = std::unique_ptr<GraphicsShader>(new GraphicsShader(glCreateShader(GL_VERTEX_SHADER)));
    FragmentShaderPtr = std::unique_ptr<GraphicsShader>(new GraphicsShader(glCreateShader(GL_FRAGMENT_SHADER)));

    ASSERT(
        CompileShader(VertexShaderSource.c_str(), *VertexShaderPtr) &&
        CompileShader(FragmentShaderSource.c_str(), *FragmentShaderPtr)
    );

    ShaderProgramPtr = std::unique_ptr<GraphicsShader>(new GraphicsShader(glCreateProgram()));

    std::vector<GraphicsShader> ShaderArray(2);
    ShaderArray.push_back(*VertexShaderPtr);
    ShaderArray.push_back(*FragmentShaderPtr);
    LinkShader(*ShaderProgramPtr, ShaderArray);

    glUseProgram(*ShaderProgramPtr);
}

bool GRenderer::CompileShader(const char* ShaderSource, GraphicsShader& Shader)
{
    glShaderSource(Shader, 1, &ShaderSource, NULL);
    glCompileShader(Shader);

    int Success = -1;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);

    if (!Success)
    {
        char InfoLog[512] = { 0 };
        glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
        std::cout << "Error: Shader compilation failed - " << InfoLog << std::endl;
    }

    return Success == 0;
}

void GRenderer::GenerateVertexBuffer()
{
    glGenBuffers(1, &VertexBufferObject);
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

void GRenderer::SetVertexAttributePointer()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

bool GRenderer::LinkShader(GraphicsShader& ShaderProgram, std::vector<GraphicsShader> ShaderArray)
{
    for (GraphicsShader Shader : ShaderArray)
    {
        glAttachShader(*ShaderProgramPtr, Shader);
    }

    glLinkProgram(*ShaderProgramPtr);

    int Success = -1;
    glGetProgramiv(*ShaderProgramPtr, GL_LINK_STATUS, &Success);

    if (!Success)
    {
        char InfoLog[512] = { 0 };
        glGetProgramInfoLog(*ShaderProgramPtr, 512, NULL, InfoLog);
        std::cout << "Error: Shader linking failed - " << InfoLog << std::endl;
    }

    return Success == 0;
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