#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

class GraphicsMesh;

class GMesh
{
public:
	GMesh(const std::string& MeshName = "");

	~GMesh();

	inline void AddVertex(std::initializer_list<float> VertexList)
	{
		float VertexArray[3];
		std::copy(VertexList.begin(), VertexList.end(), VertexArray);

		AddVertex(VertexArray);
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

	void AddVertex(const float Vertex[3]);

	void AddVertex(const float Vertex[3], const float Colour[4]);

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

	inline bool IsEditable()
	{
		return bIsEditable;
	}

	void BindBuffers()
	{
		BindBuffers(bIsEditable);
	}

private:
	enum EVertexBuffer : unsigned int
	{
		VERTEX_BUFFER = 0,
		COLOUR_BUFFER,
		ELEMENT_BUFFER,
		MAX
	};

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
	std::vector<unsigned int> Indices;

	std::unique_ptr<GraphicsMesh> VertexArrayObject;
	std::unique_ptr<GraphicsMesh> VertexBufferObjects[EVertexBuffer::MAX];
};
