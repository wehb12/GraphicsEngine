#pragma once

#include <array>

struct GVector2 : public std::array<float, 2>
{
	GVector2() : std::array<float, 2>({ 0 })
	{
	}

	GVector2(const float InX, const float InY)
		: std::array<float, 2>({ InX, InY })
	{
	}

	float& X = (*this)[0];
	float& Y = (*this)[1];
};

struct GVector3 : public std::array<float, 3>
{
	GVector3() : std::array<float, 3>({ 0 })
	{
	}

	GVector3(const float InX, const float InY, const float InZ)
		: std::array<float, 3>({ InX, InY, InZ })
	{
	}

	float& X = (*this)[0];
	float& Y = (*this)[1];
	float& Z = (*this)[2];
};

struct GVector4 : public std::array<float, 4>
{
	GVector4() : std::array<float, 4>({ 0 })
	{
	}

	GVector4(const float InR, const float InG, const float InB, const float InA)
		: std::array<float, 4>({ InR, InG, InB, InA })
	{
	}

	float& X = (*this)[0];
	float& Y = (*this)[1];
	float& Z = (*this)[2];
	float& W = (*this)[3];

    float& R = (*this)[0];
	float& G = (*this)[1];
	float& B = (*this)[2];
	float& A = (*this)[3];
};
