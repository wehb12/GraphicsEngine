#include "Graphics/Mesh/Mesh.h"
#include "Common/DebugMacros.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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

		BindBuffers();
	}
}

GMesh::~GMesh()
{
}

void GMesh::Tick(const float& DeltaTime)
{
}

void GMesh::Draw()
{
	BufferModelMatrixToShader();
	BufferMaterialToShader();
	BindVertexArray();
	Indices.size()
		? glDrawElements(GL_TRIANGLES, GetDrawCount(), GL_UNSIGNED_INT, 0)
		: glDrawArrays(GL_TRIANGLES, 0, GetDrawCount());
}

void GMesh::Rotate(const float RotateAngle, const glm::vec3& RotationAxis)
{
	*ModelMatrix = glm::rotate(*ModelMatrix, RotateAngle, RotationAxis);
}

void GMesh::RotateAroundPoint(const float RotateAngle, const glm::vec3& RotationAxis, const glm::vec3& Point)
{
	*ModelMatrix = glm::mat4_cast(glm::angleAxis(RotateAngle, RotationAxis)) * *ModelMatrix;
}

void GMesh::Scale(const float Scale)
{
	*ModelMatrix = glm::scale(*ModelMatrix, glm::vec3(Scale));
}

void GMesh::Translate(const glm::vec3& TranslationVector)
{
	*ModelMatrix = glm::translate(*ModelMatrix, TranslationVector);
}

void GMesh::SetTranslation(const glm::vec3& TranslationVector)
{
	const glm::vec3 InvScaleVec = { 1 / (*ModelMatrix)[0][0], 1 / (*ModelMatrix)[1][1], 1 / (*ModelMatrix)[2][2] };
	*ModelMatrix = glm::scale(*ModelMatrix, InvScaleVec);

	(*ModelMatrix)[3] = glm::vec4(TranslationVector, (*ModelMatrix)[3][3]);

	*ModelMatrix = glm::scale(*ModelMatrix, glm::vec3(1 / InvScaleVec[0], 1 / InvScaleVec[1], 1 / InvScaleVec[2]));
}

glm::vec3 GMesh::GetPosition() const
{
	glm::mat4 Modelcopy = *ModelMatrix;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(Modelcopy, scale, rotation, translation, skew, perspective);
	return translation;
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

void GMesh::BufferModelMatrixToShader()
{
	ASSERT(Shader);
	Shader->UseProgram();
	Shader->BufferModelMatrix(ModelMatrix);
}

void GMesh::BufferMaterialToShader()
{
	ASSERT(Shader);
	Shader->UseProgram();
	if (Texture)
	{
		Texture->BufferToShader(Shader);
	}
	else
	{
		Shader->BufferFloatUniformVector3("Material.Ambient", &Material.Ambient[0]);
		Shader->BufferFloatUniformVector3("Material.Diffuse", &Material.Diffuse[0]);
	}
	Shader->BufferFloatUniformVector3("Material.Specular", &Material.Specular[0]);
	Shader->BufferFloatUniform("Material.Shininess", Material.Shininess);
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

	if (Normals.size() != 0)
	{
		GraphicsMesh* NormalBuffer = new GraphicsMesh();
		VertexBufferObjects[EVertexBuffer::NORMAL_BUFFER] = std::unique_ptr<GraphicsMesh>(NormalBuffer);
		glGenBuffers(1, (GLuint*)NormalBuffer);
	}

	if (Tangents.size() != 0)
	{
		GraphicsMesh* TangentBuffer = new GraphicsMesh();
		VertexBufferObjects[EVertexBuffer::TANGENT_BUFFER] = std::unique_ptr<GraphicsMesh>(TangentBuffer);
		glGenBuffers(1, (GLuint*)TangentBuffer);
	}
}

void GMesh::BindBuffer(void* Array, const unsigned int ArraySize, GraphicsMesh& BufferObject, unsigned int BufferType)
{
	glBindBuffer(BufferType, BufferObject);
	glBufferData(BufferType, ArraySize, Array, GL_STATIC_DRAW);
}

void GMesh::BindBuffers()
{
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

	if (Normals.size() != 0)
	{
		BindBuffer(const_cast<float*>(&Normals[0][0]), Normals.size() * Normals[0].size() * sizeof(float), *VertexBufferObjects[EVertexBuffer::NORMAL_BUFFER], GL_ARRAY_BUFFER);
		SetVertexAttributePointer(EVertexBuffer::NORMAL_BUFFER, Normals[0].size());
	}

	if (Tangents.size() != 0)
	{
		BindBuffer(const_cast<float*>(&Tangents[0][0]), Tangents.size() * Tangents[0].size() * sizeof(float), *VertexBufferObjects[EVertexBuffer::TANGENT_BUFFER], GL_ARRAY_BUFFER);
		SetVertexAttributePointer(EVertexBuffer::TANGENT_BUFFER, Tangents[0].size());
	}
}

void GMesh::SetVertexAttributePointer(const GMesh::EVertexBuffer& VertexBufferType, const unsigned int& VectorSize)
{
	glVertexAttribPointer(VertexBufferType, VectorSize, GL_FLOAT, GL_FALSE, VectorSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(VertexBufferType);
}
