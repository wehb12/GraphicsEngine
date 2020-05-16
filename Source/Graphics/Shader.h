#pragma once

#include <initializer_list>
#include <string>
#include <memory>
#include <vector>

struct GraphicsShader;

class GShader
{
public:
    GShader(const std::initializer_list<std::string>& ShaderList);

    ~GShader();

    void UseProgram();

private:
    void DetermineShaderStageTypes(const std::vector<std::string>& ShaderList);

    std::vector<std::string> LoadShaders(const std::vector<std::string>& ShaderPaths);

    void CompileShaders(const std::vector<std::string>& ShaderSource);

    bool CompileShader(const char* ShaderSource, GraphicsShader& Shader);

    bool LinkShader();

private:
    std::vector<unsigned int> ShaderStageTypes;

    std::vector<std::unique_ptr<GraphicsShader>> ShaderPtrs;
    std::vector<std::string> ShaderSource;

    std::unique_ptr<GraphicsShader> ShaderProgramPtr;
};
