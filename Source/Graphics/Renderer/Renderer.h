#pragma once

#include <vector>
#include <memory>
#include <string>

class GWindow;
class GraphicsShader;

class GRenderer
{
public:
	GRenderer();

	~GRenderer();

	void Init();

	void Run();

private:
	void InitGraphics();

	void InitWindow();

	void MakeWindow();

	int PostWindowInit();

	// Shaders
	void LoadShaders();

	std::string LoadShader(const char* ShaderPath);

	void CompileShaders();

	bool CompileShader(const char* ShaderSource, GraphicsShader& Shader);

	// Vertex buffer operations
	void GenerateVertexBuffer();

	void GenerateElementBuffer();

	void GenerateVertexArray();

	void BindVertexArray(unsigned int& VAO);

	void BindVertexBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject);

	void BindElementBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject);

	void SetVertexAttributePointer();

	bool LinkShader(GraphicsShader& ShaderProgram, std::vector<GraphicsShader> ShaderArray);

	void RenderScene();

	void Terminate();

private:
	std::shared_ptr<GWindow> WindowPtr;

	// Hello Triangle
	std::unique_ptr<GraphicsShader> VertexShaderPtr;
	std::unique_ptr<GraphicsShader> FragmentShaderPtr;
	std::unique_ptr<GraphicsShader> ShaderProgramPtr;

	std::string VertexShaderSource;
	std::string FragmentShaderSource;

	unsigned int VertexArrayObject = 0;
	unsigned int VertexBufferObject = 0;
	unsigned int IndexBufferObject = 0;
};
