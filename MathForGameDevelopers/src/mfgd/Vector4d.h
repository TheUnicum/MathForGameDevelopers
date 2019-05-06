#pragma once

#include "mfgd/mfgd_Classes.h"

class Vector4d
{
public:
	Vector4d();
	Vector4d(float x, float y, float z, float w);
	Vector4d(const class Point& p);
	Vector4d(const class Vector& v3);
	Vector4d(const class Vector& v3, float w);

	inline float Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
	inline float LengthSqr() const { return (x * x + y * y + z * z + w * w); }

	inline Vector4d operator+() const { return Vector4d(x, y, z, w); }
	inline Vector4d operator-() const { return Vector4d(-x, -y, -z, -w); }

	inline Vector4d operator+(const Vector4d& v) const { return Vector4d(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline Vector4d operator-(const Vector4d& v) const { return Vector4d(x - v.x, y - v.y, z - v.z, w - v.w); }

	inline Vector4d operator*(float s) const { return Vector4d(x * s, y * s, z * s, w * s); }
	inline Vector4d operator/(float s) const { return Vector4d(x / s, y / s, z / s, w / s); }

	Vector4d Normalized() const;

	inline float Dot(const Vector4d& v) const { return (x * v.x + y * v.y + z * v.z + w * v.w); }

public:
	//float x, y, z, w;
	union {
		struct
		{
			float x, y, z, w;
		};
		float v[4];
	};
};