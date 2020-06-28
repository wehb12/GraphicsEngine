#pragma once

#include "Graphics/Mesh/Mesh.h"

namespace MaterialExamples
{
	GMesh::GMaterial Gold
	{
		{ 0.24725, 0.1995, 0.0745 },
		{ 0.75164, 0.60648, 0.22648 },
		{ 0.628281, 0.555802, 0.366065 },
		0.4 * 128
	};

	GMesh::GMaterial Bronze
	{
		{ 0.2125, 0.1275, 0.054 },
		{ 0.714, 0.4284, 0.18144 },
		{ 0.393548, 0.271906, 0.166721 },
		0.2 * 128
	};

	GMesh::GMaterial Emerald
	{
		{ 0.0215, 0.1745, 0.0215 },
		{ 0.07568, 0.61424, 0.07568 },
		{ 0.633, 0.727811, 0.633 },
		0.6 * 128
	};
}
