#pragma once

#include "mfgd/mfgd_Classes.h"

#include <iostream>

class Matrix4x4
{
public:
	Matrix4x4() { Identity(); };
	Matrix4x4(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& v3 = Vector(0.0f, 0.0f, 0.0f));
public:
	void Identity();



	struct MVector4D
	{
		float x, y, z, w;
	};

	union {
		float m[4][4];
		MVector4D v[4];
	};


	//MVector4D& operator[](unsigned int idx)
	//{
	//	return v[idx];
	//}

};

// utility overload std::cout
std::ostream& operator<<(std::ostream& stream, const Matrix4x4& matrix);
