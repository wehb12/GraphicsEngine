#pragma once

#include "Graphics/Mesh/Mesh.h"

#include <glm/glm.hpp>

#include <algorithm>
#include <array>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

class GEditableMesh : public GMesh
{
public:
	GEditableMesh()
		: GMesh()
	{
	} 

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

	inline void BindEditableBuffers()
	{
		BindBuffers();
	}
};
