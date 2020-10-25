#pragma once

#include <glm/glm.hpp>

#include <array>
#include <vector>

class GShader;
class GTexture;
struct GraphicsMesh;

class GMesh
{
private:
	enum EVertexBuffer : unsigned int
	{
		VERTEX_BUFFER = 0,
		COLOUR_BUFFER,
		NORMAL_BUFFER,
		TANGENT_BUFFER,
		TEXCOORD_BUFFER,
		ELEMENT_BUFFER,
		MAX
	};

public:
	struct GMaterial
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;
	};

public:
	GMesh(const std::string& MeshName = "");

	~GMesh();

	void Tick(const float& DeltaTime);

	void Draw();

	// ----------------------
    // Transform mesh methods
	// Method that say 'Set' decompose the transform first

	// Rotate by an angle in radians
	void Rotate(const float RotateAngle, const glm::vec3& RotationAxis);
	// TODO: at the minute it just rotates around origin
	void RotateAroundPoint(const float RotateAngle, const glm::vec3& RotationAxis, const glm::vec3& Point);

	void Scale(const float Scale);

	void Translate(const glm::vec3& TranslationVector);
	void SetTranslation(const glm::vec3& TranslationVector);

	glm::vec3 GetPosition() const;

	// ----------------------

    void SetTexture(const std::string& TexturePath);

	inline void SetTexture(const std::shared_ptr<GTexture>& InTexture)
	{
		Texture = InTexture;
	}

	inline void SetShader(const std::shared_ptr<GShader>& InShader)
	{
		Shader = InShader;
	}

	// ----------------------

    void BindVertexArray();

	inline unsigned int GetDrawCount() const
	{
		return Indices.size() ? Indices.size() : Vertices.size();
	}

	inline const std::shared_ptr<glm::mat4> GetModelMatrix() const
	{
		return ModelMatrix;
	}

	inline bool HasTexCoords() const
	{
		return TexCoords.size();
	}

	inline void SetMaterial(const GMaterial InMaterial)
	{
		Material = InMaterial;
	}

protected:
    void GenerateVertexArray();

	void GenerateBuffers();

	void BindBuffer(void* Array, const unsigned int ArraySize, GraphicsMesh& BufferObject, unsigned int BufferType);

	void SetVertexAttributePointer(const GMesh::EVertexBuffer& VertexBufferType, const unsigned int& VectorSize);
		
	void BindBuffers();

	void BufferModelMatrixToShader();

	void BufferMaterialToShader();

protected:
	std::vector<std::array<const float, 3>> Vertices;
	std::vector<std::array<const float, 4>> Colours;
	std::vector<std::array<const float, 2>> TexCoords;
	std::vector<unsigned int> Indices;
	std::vector<std::array<const float, 3>> Normals;
	std::vector<std::array<const float, 3>> Tangents;

	std::unique_ptr<GraphicsMesh> VertexArrayObject;
	std::unique_ptr<GraphicsMesh> VertexBufferObjects[EVertexBuffer::MAX];

	std::shared_ptr<GTexture> Texture;
	std::shared_ptr<GShader> Shader;

	std::shared_ptr<glm::mat4> ModelMatrix;

	GMaterial Material;
};
