#pragma once

#include "mfgd/mfgd_Classes.h"
#include "mfgd/Vector4d.h"

#include <iostream>

#include "glm/glm.hpp"


#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

class Matrix4x4
{
public:
	Matrix4x4() { Identity(); };
	Matrix4x4(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& v3 = Vector(0.0f, 0.0f, 0.0f));
	Matrix4x4(const Vector4d& v0, const Vector4d& v1, const Vector4d& v2, const Vector4d& v3);
public:
	void Identity();

	Matrix4x4 Transposed() const;

	Matrix4x4 operator*(const Matrix4x4& t) const;

	bool operator==(const glm::mat4& t) const;


	// Set a transformation
	void SetTranslation(const Vector& vecPos);
	void SetRotation(float flAngle, const Vector& vecAxis);		// Assume te axis is a normalized vector
	void SetScale(const Vector& vecScale);


	// Trasform a position vector
	Vector operator*(const Vector& v) const;
	Vector4d operator*(const Vector4d& v) const;

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

// Static funcitons
Matrix4x4 GetView(Point& position, Vector& target, Vector& worldUp);

// utility overload std::cout
std::ostream& operator<<(std::ostream& stream, const Matrix4x4& matrix);
std::ostream& operator<<(std::ostream& stream, const glm::mat4& matrix);
std::ostream& operator<<(std::ostream& stream, const glm::vec4& vec4);
std::ostream& operator<<(std::ostream& stream, const Vector4d vec4);
