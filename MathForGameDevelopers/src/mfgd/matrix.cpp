#include "matrix.h"

#include <string>	// for std::memset

Matrix4x4::Matrix4x4(const Vector & v0, const Vector & v1, const Vector & v2, const Vector & v3)
{
	v[0] = { v0.x, v0.y, v0.z, 0.0f };
	v[1] = { v1.x, v1.y, v1.z, 0.0f };
	v[2] = { v2.x, v2.y, v2.z, 0.0f };
	v[3] = { v3.x, v3.y, v3.z, 1.0f };
}

Matrix4x4::Matrix4x4(const Vector4d & v0, const Vector4d & v1, const Vector4d & v2, const Vector4d & v3)
{
	v[0] = { v0.x, v0.y, v0.z, v0.w };
	v[1] = { v1.x, v1.y, v1.z, v1.w };
	v[2] = { v2.x, v2.y, v2.z, v2.w };
	v[3] = { v3.x, v3.y, v3.z, v3.w };
}


void Matrix4x4::Identity()
{
	std::memset(this, 0, sizeof(Matrix4x4));

	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & t) const
{
	Matrix4x4 r;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			r.m[i][j] = m[i][0] * t.m[0][j] + m[i][1] * t.m[1][j] + m[i][2] * t.m[2][j] + m[i][3] * t.m[3][j];
	}

	return r;
}


glm::mat4 Matrix4x4::ToGlm() const
{
	// FRU
	/*		   F  U  R
			 +-          -+
			 | Fx Uy Rz 0 |
	LookAt = | Fx Uy Rz 0 |
			 | Fx Uy Rz 0 |
			 | 0  0  0  1 |
			 +-          -+
	*/

	// In glm we access elements as mat[col][row] due to clumn-major layout
	glm::mat4 r(1.0f);
	// Vector 0
	r[0] = glm::vec4(v[0].x, v[0].y, v[0].z, v[0].w );
	r[1] = glm::vec4(v[1].x, v[1].y, v[1].z, v[1].w );
	r[2] = glm::vec4(v[2].x, v[2].y, v[2].z, v[2].w );
	r[3] = glm::vec4(v[3].x, v[3].y, v[3].z, v[3].w );

	// if is not correc is possible to transpose the Matrix
	//r = glm::transpose(r);

	// for more info check camera.cpp
	//	<OpenGL-Beginner-to-Advance.sln>

	return r;
}


// Static funcitons
Matrix4x4 GetView(Point & position, Vector & target, Vector & worldUp)
{
	/*
			 +-          -+  +-           -+
			 | Rx Ry Rz 0 |  | 1  0  0 -Px |
	LookAt = | Ux Uy Uz 0 |  | 0  1  0 -Py |
			 | Dx Dy Dz 0 |  | 0  0  1 -Pz |
			 | 0  0  0  1 |  | 0  0  0  1  |
			 +-          -+  +-           -+
	*/
	Vector zaxis_D = position - target;
	zaxis_D = zaxis_D.Normalized();

	Vector xaxis_R = worldUp.Cross(zaxis_D);
	xaxis_R = xaxis_R.Normalized();

	Vector yaxis_U = zaxis_D.Cross(xaxis_R);
	yaxis_U.Normalized();

	Matrix4x4 r(xaxis_R, yaxis_U, zaxis_D);
	r.v[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

	std::cout << r << std::endl;

	Matrix4x4 t;
	t.m[0][3] = -position.x;
	t.m[1][3] = -position.y;
	t.m[2][3] = -position.z;


	Matrix4x4 ret;
	ret = r * t;
	return ret;

}



// utility overload for iostream Vertex Data
std::ostream & operator<<(std::ostream & stream, const Matrix4x4 & matrix)
{
	// TODO: inserire l'istruzione return qui
	stream << "------------------------------\n";
	//stream << "M4x4 mm: (" << matrix.m[0][0] << ", " << matrix.m[0][1] << ", " << matrix.m[0][2] << ", " << matrix.m[0][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix.m[1][0] << ", " << matrix.m[1][1] << ", " << matrix.m[1][2] << ", " << matrix.m[1][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix.m[2][0] << ", " << matrix.m[2][1] << ", " << matrix.m[2][2] << ", " << matrix.m[2][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix.m[3][0] << ", " << matrix.m[3][1] << ", " << matrix.m[3][2] << ", " << matrix.m[3][3] << ")\n";

	stream << "M4d4 v0: (" << matrix.v[0].x << ", " << matrix.v[0].y << ", " << matrix.v[0].z << ", " << matrix.v[0].w << ")\n";
	stream << "M4d4 v0: (" << matrix.v[1].x << ", " << matrix.v[1].y << ", " << matrix.v[1].z << ", " << matrix.v[1].w << ")\n";
	stream << "M4d4 v0: (" << matrix.v[2].x << ", " << matrix.v[2].y << ", " << matrix.v[2].z << ", " << matrix.v[2].w << ")\n";
	stream << "M4d4 v0: (" << matrix.v[3].x << ", " << matrix.v[3].y << ", " << matrix.v[3].z << ", " << matrix.v[3].w << ")";
	return stream;
}

