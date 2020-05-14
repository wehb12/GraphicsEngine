#pragma once

#include <vector>
#include <memory>
#include <string>

class GShader;
class GWindow;

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

	// Vertex buffer operations
	void GenerateVertexBuffers();

	void GenerateElementBuffer();

	void GenerateVertexArray();

	void BindVertexArray(unsigned int& VAO);

	void BindVertexBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject);

	void BindElementBuffer(void* Array, const unsigned int ArraySize, unsigned int& BufferObject);

	void SetVertexAttributePointer(const EVertexBuffer::Type& VertexBufferType, const unsigned int& VectorSize);

	void RenderScene();

	void Terminate();

private:
	std::shared_ptr<GWindow> WindowPtr;

	std::unique_ptr<GShader> HelloTriangleShader;

	unsigned int VertexArrayObject = 0;
	unsigned int VertexBufferObjects[EVertexBuffer::MAX] = { 0 };
	unsigned int IndexBufferObject = 0;
};
