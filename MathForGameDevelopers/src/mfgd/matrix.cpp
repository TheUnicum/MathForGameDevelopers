#include "matrix.h"

#include <string>	// for std::memset

Matrix4x4::Matrix4x4(const Vector & v0, const Vector & v1, const Vector & v2, const Vector & v3)
{
	v[0] = { v0.x, v0.y, v0.z, 0.0f };
	v[1] = { v1.x, v1.y, v1.z, 0.0f };
	v[2] = { v2.x, v2.y, v2.z, 0.0f };
	v[3] = { v3.x, v3.y, v3.z, 0.0f };
}


void Matrix4x4::Identity()
{
	std::memset(this, 0, sizeof(Matrix4x4));

	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}


// utility overload for iostream Vertex Data
std::ostream & operator<<(std::ostream & stream, const Matrix4x4 & matrix)
{
	// TODO: inserire l'istruzione return qui
	stream << "------------------------------\n";
	stream << "M4x4 mm: (" << matrix.m[0][0] << ", " << matrix.m[0][1] << ", " << matrix.m[0][2] << ", " << matrix.m[0][3] << ")\n";
	stream << "M4x4 mm: (" << matrix.m[1][0] << ", " << matrix.m[1][1] << ", " << matrix.m[1][2] << ", " << matrix.m[1][3] << ")\n";
	stream << "M4x4 mm: (" << matrix.m[2][0] << ", " << matrix.m[2][1] << ", " << matrix.m[2][2] << ", " << matrix.m[2][3] << ")\n";
	stream << "M4x4 mm: (" << matrix.m[3][0] << ", " << matrix.m[3][1] << ", " << matrix.m[3][2] << ", " << matrix.m[3][3] << ")\n";

	stream << "M4d4 v0: (" << matrix.v[0].x << ", " << matrix.v[0].y << ", " << matrix.v[0].z << ", " << matrix.v[0].w << ")\n";
	stream << "M4d4 v0: (" << matrix.v[1].x << ", " << matrix.v[1].y << ", " << matrix.v[1].z << ", " << matrix.v[1].w << ")\n";
	stream << "M4d4 v0: (" << matrix.v[2].x << ", " << matrix.v[2].y << ", " << matrix.v[2].z << ", " << matrix.v[2].w << ")\n";
	stream << "M4d4 v0: (" << matrix.v[3].x << ", " << matrix.v[3].y << ", " << matrix.v[3].z << ", " << matrix.v[3].w;
	return stream;
}
