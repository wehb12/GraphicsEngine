#pragma once

#include <glm/glm.hpp>

#include <algorithm>
#include <array>
#include <initializer_list>
#include <memory>
#include <string>
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

	// Transform mesh methods
	// Method that say 'Set' decompose the transform first
	// Rotate by an angle in radians
	void Rotate(const float RotateAngle, const glm::vec3& RotationAxis);
	// TODO: at the minute it just rotates around origin
	void RotateAroundPoint(const float RotateAngle, const glm::vec3& RotationAxis, const glm::vec3& Point);

	void Scale(const float Scale);

	void Translate(const glm::vec3& TranslationVector);
	void SetTranslation(const glm::vec3& TranslationVector);

	const glm::vec3& GetPosition() const;

	// ----------------------

	// Manually add mesh data
	inline void AddVertex(std::initializer_list<float> VertexList)
	{
		float VertexArray[3];
		std::copy(VertexList.begin(), VertexList.end(), VertexArray);

		AddVertex(VertexArray);
	}

	inline void AddNormal(std::initializer_list<float> NormalList)
	{
		float NormalArray[3];
		std::copy(NormalList.begin(), NormalList.end(), NormalArray);

		AddNormal(NormalArray);
	}

	inline void AddTangent(std::initializer_list<float> TangentList)
	{
		float TangentArray[3];
		std::copy(TangentList.begin(), TangentList.end(), TangentArray);

		AddTangent(TangentArray);
	}

	inline void AddColour(std::initializer_list<float> ColourList)
	{
		float ColourArray[4];
		std::copy(ColourList.begin(), ColourList.end(), ColourArray);

		if (ColourList.size() == 3)
		{
			ColourArray[3] = 1.0f;
		}

		AddColour(ColourArray);
	}

	void AddColour(const glm::vec4& ColourVector);

	inline void AddTexCoord(std::initializer_list<float> TexCoordList)
	{
		float TexCoordArray[2];
		std::copy(TexCoordList.begin(), TexCoordList.end(), TexCoordArray);

		AddTexCoord(TexCoordArray);
	}

	inline void AddVertex(std::initializer_list<float> VertexList, std::initializer_list<float> ColourList)
	{
		float VertexArray[3];
		float ColourArray[4];
		std::copy(VertexList.begin(), VertexList.end(), VertexArray);
		std::copy(ColourList.begin(), ColourList.end(), ColourArray);

		if (ColourList.size() == 3)
		{
			ColourArray[3] = 1.0f;
		}

		AddVertex(VertexArray, ColourArray);
	}

	inline void AddVertex(std::initializer_list<float> VertexList, std::initializer_list<float> ColourList, std::initializer_list<float> TexCoordList)
	{
		float VertexArray[3];
		float ColourArray[4];
		float TexCoordArray[2];
		std::copy(VertexList.begin(), VertexList.end(), VertexArray);
		std::copy(ColourList.begin(), ColourList.end(), ColourArray);
		std::copy(TexCoordList.begin(), TexCoordList.end(), TexCoordArray);

		if (ColourList.size() == 3)
		{
			ColourArray[3] = 1.0f;
		}

		AddVertex(VertexArray, ColourArray);
		AddTexCoord(TexCoordArray);
	}

	void AddVertex(const float Vertex[3]);

	void AddVertex(const float Vertex[3], const float Colour[4]);

	void AddColour(const float Colour[4]);

	void AddNormal(const float Normal[3]);

	void AddTangent(const float Tangent[3]);

	void AddTexCoord(const float TexCoord[2]);

	inline void AddIndices(const unsigned int& Index)
	{
		Indices.push_back(Index);
	}

	template<typename... T>
	inline void AddIndices(const unsigned int& Index, T... Args)
	{
		Indices.push_back(Index);
		AddIndices(Args...);
	}
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

	inline bool IsEditable()
	{
		return bIsEditable;
	}

	inline void BindBuffers()
	{
		BindBuffers(bIsEditable);
	}

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

private:
	void GenerateVertexArray();

	void GenerateBuffers();

	void BindBuffer(void* Array, const unsigned int ArraySize, GraphicsMesh& BufferObject, unsigned int BufferType);

	void SetVertexAttributePointer(const GMesh::EVertexBuffer& VertexBufferType, const unsigned int& VectorSize);
		
	void BindBuffers(const bool& bCanEdit);

	void BufferModelMatrixToShader();

	void BufferMaterialToShader();

private:
	// False if mesh loaded from file
	bool bIsEditable = false;

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
