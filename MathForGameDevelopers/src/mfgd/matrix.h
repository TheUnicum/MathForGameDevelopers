#pragma once

#include "mfgd/mfgd_Classes.h"
#include "mfgd/Vector4d.h"

#include <iostream>

#include "glm/glm.hpp"

class Matrix4x4
{
public:
	Matrix4x4() { Identity(); };
	Matrix4x4(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& v3 = Vector(0.0f, 0.0f, 0.0f));
	Matrix4x4(const Vector4d& v0, const Vector4d& v1, const Vector4d& v2, const Vector4d& v3);
public:
	void Identity();

	Matrix4x4 operator*(const Matrix4x4& t) const;

	glm::mat4 ToGlm() const;

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
