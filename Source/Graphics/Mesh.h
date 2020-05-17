#pragma once

#include <glm/fwd.hpp>

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

class GTexture;
struct GraphicsMesh;

class GMesh
{
	enum EVertexBuffer : unsigned int
	{
		VERTEX_BUFFER = 0,
		COLOUR_BUFFER,
		TEXCOORD_BUFFER,
		ELEMENT_BUFFER,
		MAX
	};

public:
	GMesh(const std::string& MeshName = "");

	~GMesh();

	// Manually add mesh data
	inline void AddVertex(std::initializer_list<float> VertexList)
	{
		float VertexArray[3];
		std::copy(VertexList.begin(), VertexList.end(), VertexArray);

		AddVertex(VertexArray);
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

	void SetTexture(const std::string& TexturePath);

	inline bool IsEditable()
	{
		return bIsEditable;
	}

	inline void BindBuffers()
	{
		BindBuffers(bIsEditable);
	}

	inline const std::shared_ptr<glm::mat4> GetModelMatrix()
	{
		return ModelMatrix;
	}

private:
	void GenerateVertexArray();

	void BindVertexArray(GraphicsMesh& VAO);

	void GenerateBuffers();

	void BindBuffer(void* Array, const unsigned int ArraySize, GraphicsMesh& BufferObject, unsigned int BufferType);

	void SetVertexAttributePointer(const GMesh::EVertexBuffer& VertexBufferType, const const unsigned int& VectorSize);
		
	void BindBuffers(const bool& bCanEdit);

private:
	// False if mesh loaded from file
	bool bIsEditable = false;

	std::vector<std::array<const float, 3>> Vertices;
	std::vector<std::array<const float, 4>> Colours;
	std::vector<std::array<const float, 2>> TexCoords;
	std::vector<unsigned int> Indices;

	std::unique_ptr<GraphicsMesh> VertexArrayObject;
	std::unique_ptr<GraphicsMesh> VertexBufferObjects[EVertexBuffer::MAX];

	std::shared_ptr<GTexture> Texture;

	std::shared_ptr<glm::mat4> ModelMatrix;
};
