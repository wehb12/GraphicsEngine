#pragma once

#include <vector>
#include <memory>
#include <string>

class GWindow;
class GraphicsShader;

// TODO: Move to a mesh or vertex class
namespace EVertexBuffer
{
	enum Type : unsigned int
	{
		VERTEX_BUFFER = 0,
		COLOUR_BUFFER,
		MAX
	};
}

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
	void GenerateVertexBuffers();

	void GenerateElementBuffer();

	void GenerateVertexArray();

	void BindVertexArray(unsigned int& VAO);

	void BindVertexBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject);

	void BindElementBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject);

	void SetVertexAttributePointer(const EVertexBuffer::Type& VertexBufferType, const unsigned int& VectorSize);

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
	unsigned int VertexBufferObjects[EVertexBuffer::MAX] = { 0 };
	unsigned int IndexBufferObject = 0;
};
