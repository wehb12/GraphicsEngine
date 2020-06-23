#pragma once

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    void UseProgram() const;

    inline void BufferModelMatrix(const std::shared_ptr<glm::mat4> ModelMatrix)
    {
        BufferFloatUniformMatrix4x4("ModelMatrix", glm::value_ptr(*ModelMatrix));
    }

    inline void BufferProjectionViewMatrix(const std::shared_ptr<glm::mat4> ProjectionViewMatrix)
    {
        BufferFloatUniformMatrix4x4("ProjectionViewMatrix", glm::value_ptr(*ProjectionViewMatrix));
    }

    void BufferFloatUniformMatrix4x4(const char* UniformName, const float* UniformData) const;
    void BufferFloatUniformVector3(const char* UniformName, const float* UniformData) const;
    void BufferFloatUniformVector4(const char* UniformName, const float* UniformData) const;

	void Recompile();

private:
    void DetermineShaderStageTypes(const std::vector<std::string>& ShaderList);

    std::vector<std::string> LoadShaders(const std::vector<std::string>& ShaderPaths);

    void CompileShaders(const std::vector<std::string>& ShaderSource);

    bool CompileShader(const char* ShaderSource, GraphicsShader& Shader);

    bool LinkShader();

private:
    std::vector<unsigned int> ShaderStageTypes;

    std::vector<std::unique_ptr<GraphicsShader>> ShaderPtrs;
	std::vector<std::string> ShaderPaths;
	std::vector<std::string> ShaderSource;

    std::unique_ptr<GraphicsShader> ShaderProgramPtr;
};
