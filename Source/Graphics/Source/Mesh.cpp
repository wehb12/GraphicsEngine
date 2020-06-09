#include "Graphics/Mesh.h"
#include "Common/DebugMacros.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <ctime>

struct GraphicsMesh
{
	GraphicsMesh()
	{
	}

	GraphicsMesh(const GLuint& InMesh)
		: GLMesh(InMesh)
	{
	}

	operator GLuint()
	{
		return GLMesh;
	}

	GLuint GLMesh = 0;
};

GMesh::GMesh(const std::string& MeshName)
{
	ModelMatrix = std::shared_ptr<glm::mat4>(new glm::mat4(1.0f));

	if (MeshName.size() != 0)
	{
		// Load mesh from file

		BindBuffers(true);
	}
	else
	{
		bIsEditable = true;
	}
}

GMesh::~GMesh()
{
}

void GMesh::Tick(const float& DeltaTime)
{
	// TODO: move this to a timer class
	const double StartTime = glfwGetTime();

	const float Scale = std::sin(StartTime) + 1;
	//*ModelMatrix = glm::scale(glm::identity<glm::mat4>(), glm::vec3(Scale, Scale, Scale));
}

void GMesh::Rotate(const float RotateAngle, const glm::vec3& RotationAxis)
{
	*ModelMatrix = glm::rotate(*ModelMatrix, RotateAngle, RotationAxis);
}

void GMesh::Scale(const float Scale)
{
	*ModelMatrix = glm::scale(*ModelMatrix, glm::vec3(Scale));
}

void GMesh::Translate(const glm::vec3& TranslationVector)
{
	*ModelMatrix = glm::translate(*ModelMatrix, TranslationVector);
}

void GMesh::AddVertex(const float Vertex[3])
{
	ASSERT(bIsEditable);

	const std::array<const float, 3> VertexArray = {
		Vertex[0],
		Vertex[1],
		Vertex[2]
	};

	Vertices.push_back(VertexArray);
}

void GMesh::AddVertex(const float Vertex[3], const float Colour[4])
{
	ASSERT(bIsEditable);

	const std::array<const float, 3> VertexArray = {
		Vertex[0],
		Vertex[1],
		Vertex[2]
	};

	const std::array<const float, 4> ColourArray = {
		Colour[0],
		Colour[1],
		Colour[2],
		Colour[3]
	};

	Vertices.push_back(VertexArray);
	Colours.push_back(ColourArray);
}

void GMesh::AddColour(const float Colour[4])
{
	ASSERT(bIsEditable);

	const std::array<const float, 4> ColourArray = {
		Colour[0],
		Colour[1],
		Colour[2],
		Colour[4]
	};

	Colours.push_back(ColourArray);
}

void GMesh::AddTexCoord(const float TexCoord[2])
{
	ASSERT(bIsEditable);

	const std::array<const float, 2> TexCoordArray = {
		TexCoord[0],
		TexCoord[1]
	};

	TexCoords.push_back(TexCoordArray);
}

void GMesh::SetTexture(const std::string& TexturePath)
{
	ASSERT(Shader, "Shader must be set so texture can buffer itself to the shader");
	Shader->UseProgram();
	Texture = std::shared_ptr<GTexture>(new GTexture(TexturePath));
}

void GMesh::GenerateVertexArray()
{
	VertexArrayObject = std::unique_ptr<GraphicsMesh>(new GraphicsMesh());
	glGenVertexArrays(1, (GLuint*)VertexArrayObject.get());
}

void GMesh::BufferModelMatrixToShader(const std::shared_ptr<glm::mat4> ModelMatrix)
{
	ASSERT(Shader);
	Shader->UseProgram();
	Shader->BufferModelMatrix(ModelMatrix);
}

void GMesh::BindVertexArray()
{
	glBindVertexArray(*VertexArrayObject);
}

void GMesh::GenerateBuffers()
{
	ASSERT(Vertices.size());

	GraphicsMesh* VertexBuffer = new GraphicsMesh();
	VertexBufferObjects[EVertexBuffer::VERTEX_BUFFER] = std::unique_ptr<GraphicsMesh>(VertexBuffer);
	glGenBuffers(1, (GLuint*)VertexBuffer);

	if (Colours.size() != 0)
	{
		GraphicsMesh* ColourBuffer = new GraphicsMesh();
		VertexBufferObjects[EVertexBuffer::COLOUR_BUFFER] = std::unique_ptr<GraphicsMesh>(ColourBuffer);
		glGenBuffers(1, (GLuint*)ColourBuffer);
	}

	if (TexCoords.size() != 0)
	{
		GraphicsMesh* TexCoordBuffer = new GraphicsMesh();
		VertexBufferObjects[EVertexBuffer::TEXCOORD_BUFFER] = std::unique_ptr<GraphicsMesh>(TexCoordBuffer);
		glGenBuffers(1, (GLuint*)TexCoordBuffer);
	}

	if (Indices.size() != 0)
	{
		GraphicsMesh* IndexBuffer = new GraphicsMesh();
		VertexBufferObjects[EVertexBuffer::ELEMENT_BUFFER] = std::unique_ptr<GraphicsMesh>(IndexBuffer);
		glGenBuffers(1, (GLuint*)IndexBuffer);
	}
}

void GMesh::BindBuffer(void* Array, const unsigned int ArraySize, GraphicsMesh& BufferObject, unsigned int BufferType)
{
	glBindBuffer(BufferType, BufferObject);
	glBufferData(BufferType, ArraySize, Array, GL_STATIC_DRAW);
}

void GMesh::BindBuffers(const bool& bCanEdit)
{
	ASSERT(bIsEditable);

	GenerateVertexArray();
	BindVertexArray();

	GenerateBuffers();

	BindBuffer(const_cast<float*>(&Vertices[0][0]), Vertices.size() * Vertices[0].size() * sizeof(float), *VertexBufferObjects[EVertexBuffer::VERTEX_BUFFER], GL_ARRAY_BUFFER);
	SetVertexAttributePointer(EVertexBuffer::VERTEX_BUFFER, Vertices[0].size());

	if (Colours.size() != 0)
	{
		BindBuffer(const_cast<float*>(&Colours[0][0]), Colours.size() * Colours[0].size() * sizeof(float), *VertexBufferObjects[EVertexBuffer::COLOUR_BUFFER], GL_ARRAY_BUFFER);
		SetVertexAttributePointer(EVertexBuffer::COLOUR_BUFFER, Colours[0].size());
	}

	if (TexCoords.size() != 0)
	{
		BindBuffer(const_cast<float*>(&TexCoords[0][0]), TexCoords.size() * TexCoords[0].size() * sizeof(float), *VertexBufferObjects[EVertexBuffer::TEXCOORD_BUFFER], GL_ARRAY_BUFFER);
		SetVertexAttributePointer(EVertexBuffer::TEXCOORD_BUFFER, TexCoords[0].size());
	}

	if (Indices.size() != 0)
	{
		BindBuffer(const_cast<unsigned int*>(&Indices[0]), Indices.size() * sizeof(unsigned int), *VertexBufferObjects[EVertexBuffer::ELEMENT_BUFFER], GL_ELEMENT_ARRAY_BUFFER);
	}
}

void GMesh::SetVertexAttributePointer(const GMesh::EVertexBuffer& VertexBufferType, const unsigned int& VectorSize)
{
	glVertexAttribPointer(VertexBufferType, VectorSize, GL_FLOAT, GL_FALSE, VectorSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(VertexBufferType);
}
