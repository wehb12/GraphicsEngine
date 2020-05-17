#include "Graphics/Shader.h"
#include "Common/DebugMacros.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

struct GraphicsShader
{
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


GShader::GShader(const std::initializer_list<std::string>& ShaderList)
{
	std::vector<std::string> ShaderPaths;
	ShaderPaths.insert(ShaderPaths.end(), ShaderList.begin(), ShaderList.end());

    DetermineShaderStageTypes(ShaderList);
	const std::vector<std::string> ShaderSource = LoadShaders(ShaderPaths);

    CompileShaders(ShaderSource);
}

GShader::~GShader()
{
    for (std::unique_ptr<GraphicsShader>& ShaderPtr : ShaderPtrs)
    {
        glDeleteShader(*ShaderPtr);
    }
    glDeleteProgram(*ShaderProgramPtr);
}

void GShader::UseProgram()
{
    ASSERT(ShaderProgramPtr);
    glUseProgram(*ShaderProgramPtr);
}

void GShader::BufferModelMatrix(const std::shared_ptr<glm::mat4> ModelMatrix)
{
    unsigned int Loc = glGetUniformLocation(*ShaderProgramPtr, "ModelMatrix");
    glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(*ModelMatrix));
}

void GShader::DetermineShaderStageTypes(const std::vector<std::string>& ShaderList)
{
    // TODO: Make these checks more robust, maybe pass a flag into constructor to verify makeup of shader
    if (ShaderList.size() == 2)
    {
        ShaderStageTypes.push_back(GL_VERTEX_SHADER);
        ShaderStageTypes.push_back(GL_FRAGMENT_SHADER);
    }
    else if (ShaderList.size() == 3)
    {
        ShaderStageTypes.push_back(GL_VERTEX_SHADER);
        ShaderStageTypes.push_back(GL_GEOMETRY_SHADER);
        ShaderStageTypes.push_back(GL_FRAGMENT_SHADER);
    }
    else if (ShaderList.size() == 4)
    {
        ShaderStageTypes.push_back(GL_VERTEX_SHADER);
        ShaderStageTypes.push_back(GL_TESS_CONTROL_SHADER);
        ShaderStageTypes.push_back(GL_TESS_EVALUATION_SHADER);
        ShaderStageTypes.push_back(GL_FRAGMENT_SHADER);
    }
    else if (ShaderList.size() == 5)
    {
        ShaderStageTypes.push_back(GL_VERTEX_SHADER);
        ShaderStageTypes.push_back(GL_TESS_CONTROL_SHADER);
        ShaderStageTypes.push_back(GL_TESS_EVALUATION_SHADER);
        ShaderStageTypes.push_back(GL_GEOMETRY_SHADER);
        ShaderStageTypes.push_back(GL_FRAGMENT_SHADER);
    }
}

std::vector<std::string> GShader::LoadShaders(const std::vector<std::string>& ShaderPaths)
{
    std::vector<std::string> ShaderSource;
	for (const std::string& ShaderPath : ShaderPaths)
	{
		std::ifstream ShaderFile(ShaderPath);

        ASSERT_FAIL(ShaderFile.is_open(), std::cout << "Shader file " << ShaderPath << " does not exist" << std::endl);
        ShaderSource.push_back(std::string((std::istreambuf_iterator<char>(ShaderFile)), std::istreambuf_iterator<char>()));
	}

    return ShaderSource;
}

void GShader::CompileShaders(const std::vector<std::string>& ShaderSource)
{
    for (unsigned int Index = 0; Index < ShaderSource.size(); ++Index)
    {
        ShaderPtrs.push_back(std::unique_ptr<GraphicsShader>(new GraphicsShader(glCreateShader(ShaderStageTypes[Index]))));

        ASSERT(CompileShader(ShaderSource[Index].c_str(), *ShaderPtrs[Index]));
    }

    ShaderProgramPtr = std::unique_ptr<GraphicsShader>(new GraphicsShader(glCreateProgram()));

    LinkShader();
}

bool GShader::CompileShader(const char* ShaderSource, GraphicsShader& Shader)
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

    return Success != 0;
}

bool GShader::LinkShader()
{
    for (std::unique_ptr<GraphicsShader>& Shader : ShaderPtrs)
    {
        glAttachShader(*ShaderProgramPtr, *Shader);
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
