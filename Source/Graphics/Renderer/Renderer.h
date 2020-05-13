#pragma once

#include <vector>
#include <memory>

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

	void CompileShaders();

	bool CompileShader(const char* ShaderSource, GraphicsShader& Shader);

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

	unsigned int VertexArrayObject = 0;
	unsigned int VertexBufferObject = 0;
	unsigned int IndexBufferObject = 0;
};
