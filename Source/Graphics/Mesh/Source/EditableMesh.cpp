#include "Graphics/Mesh/EditableMesh.h"
#include "Common/Vector.h"

void GEditableMesh::AddColour(const glm::vec4& ColourVector)
{
	AddColour(&ColourVector[0]);
}

void GEditableMesh::AddVertex(const float Vertex[3])
{
	const GVector3 VertexArray = {
		Vertex[0],
		Vertex[1],
		Vertex[2]
	};

	Vertices.push_back(VertexArray);
}

void GEditableMesh::AddVertex(const float Vertex[3], const float Colour[4])
{
	const GVector3 VertexArray = {
		Vertex[0],
		Vertex[1],
		Vertex[2]
	};

	const GVector4 ColourArray = {
		Colour[0],
		Colour[1],
		Colour[2],
		Colour[3]
	};

	Vertices.push_back(VertexArray);
	Colours.push_back(ColourArray);
}

void GEditableMesh::AddColour(const float Colour[4])
{
	const GVector4 ColourArray = {
		Colour[0],
		Colour[1],
		Colour[2],
		Colour[4]
	};

	Colours.push_back(ColourArray);
}

void GEditableMesh::AddNormal(const float Normal[3])
{
	const GVector3 NormalArray = {
		Normal[0],
		Normal[1],
		Normal[2]
	};

	Normals.push_back(NormalArray);
}

void GEditableMesh::AddTangent(const float Tangent[3])
{
	const GVector3 TangentArray = {
		Tangent[0],
		Tangent[1],
		Tangent[2]
	};

	Tangents.push_back(TangentArray);
}

void GEditableMesh::AddTexCoord(const float TexCoord[2])
{
	const GVector2 TexCoordArray = {
		TexCoord[0],
		TexCoord[1]
	};

	TexCoords.push_back(TexCoordArray);
}
