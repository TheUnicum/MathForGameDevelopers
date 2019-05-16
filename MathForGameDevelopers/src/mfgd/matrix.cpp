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

Matrix4x4 Matrix4x4::Transposed() const
{
	Matrix4x4 r;
	r.m[0][0] = m[0][0]; r.m[1][0] = m[0][1]; r.m[2][0] = m[0][2]; r.m[3][0] = m[0][3];
	r.m[0][1] = m[1][0]; r.m[1][1] = m[1][1]; r.m[2][1] = m[1][2]; r.m[3][1] = m[1][3];
	r.m[0][2] = m[2][0]; r.m[1][2] = m[2][1]; r.m[2][2] = m[2][2]; r.m[3][2] = m[2][3];
	r.m[0][3] = m[3][0]; r.m[1][3] = m[3][1]; r.m[2][3] = m[3][2]; r.m[3][3] = m[3][3];
	return r;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & t) const
{
	Matrix4x4 r;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			r.m[i][j] = m[i][0] * t.m[0][j] + m[i][1] * t.m[1][j] + m[i][2] * t.m[2][j] + m[i][3] * t.m[3][j];
			//r.m[i][j] = m[0][j] * t.m[i][0] + m[1][j] * t.m[i][1] + m[2][j] * t.m[i][2] + m[3][j] * t.m[i][3];
	}

	return r;
}

bool Matrix4x4::operator==(const glm::mat4 & t) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//if (m[i][j] != t[j][i])
			//	return false;

			if (std::fabs(m[i][j] - t[j][i]) > 0.00001f)
				return false;

		}
	}
	return true;
}

Vector Matrix4x4::GetTranslation() const
{
	return Vector(m[0][3], m[1][3], m[2][3]);
}


void Matrix4x4::SetTranslation(const Vector & vecPos)
{
	// Non mi torna
	//m[3][0] = vecPos.x;
	//m[3][1] = vecPos.y;
	//m[3][2] = vecPos.z;

	m[0][3] = vecPos.x;
	m[1][3] = vecPos.y;
	m[2][3] = vecPos.z;

}

void Matrix4x4::SetRotation(float flAngle, const Vector & vecAxis)
{
	Vector _vecAxis = vecAxis.Normalized();


	// Rotaion Ax Z
	float x = _vecAxis.x;
	float y = _vecAxis.y;
	float z = _vecAxis.z;

	float c = std::cos(flAngle*(float)M_PI / 180);
	float s = std::sin(flAngle*(float)M_PI / 180);
	float t = 1 - c;

	/*
	https://learnopengl.com/Getting-started/Transformations
	https://en.wikipedia.org/wiki/Rotation_matrix
	https://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
	*/

	//1 + (1-cos(angle))*(x*x-1)	-z*sin(angle)+(1-cos(angle))*x*y	y*sin(angle)+(1-cos(angle))*x*z
	m[0][0] = +c + x*x*t;		//
	m[0][1] = x*y*t - z*s;		//
	m[0][2] = x*z*t + y*s;		//

	//z*sin(angle)+(1-cos(angle))*x*y	1 + (1-cos(angle))*(y*y-1)	-x*sin(angle)+(1-cos(angle))*y*z
	m[1][0] = y*x*t + z*s;		//
	m[1][1] = c + y*y *t;		//
	m[1][2] = y*z*t - x*s;		//

	//-y*sin(angle)+(1-cos(angle))*x*z	x*sin(angle)+(1-cos(angle))*y*z	1 + (1-cos(angle))*(z*z-1)
	m[2][0] = z*x*t - y * s;		//
	m[2][1] = z*y*t + x*s;		//
	m[2][2] = c + z*z *t;		//


}

void Matrix4x4::SetScale(const Vector & vecScale)
{
	m[0][0] = vecScale.x;
	m[1][1] = vecScale.y;
	m[2][2] = vecScale.z;

}

Vector Matrix4x4::operator*(const Vector & v) const
{
	/*
	[a b c d][X]
	[d e f y][Y]	= [aX+bY+cZ+dW, dX+eY+fZ+yW, gX+hY+iZ+zW]
	[g h i z][Z]
			 [1]

	*/
	Vector vecResult;
	vecResult.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
	vecResult.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
	vecResult.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
	return vecResult;
}

Vector4d Matrix4x4::operator*(const Vector4d & v) const
{
	/*
	[a b c d][X]
	[d e f y][Y]	= [aX+bY+cZ+dW, dX+eY+fZ+yW, gX+hY+iZ+zW, jX+kY+lZ+mW] 
	[g h i z][Z]
	[j k l m][W]
	
	*/
	Vector4d vecResult;
	vecResult.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	vecResult.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	vecResult.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	vecResult.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return vecResult;
}

// Change of basis

// FRU
/*		   R  U  F
		 +-          -+
		 | Rx Uy Fz 0 |
R U F  = | Rx Uy Fz 0 |
		 | Rx Uy Fz 0 |
		 | 0  0  0  1 |
		 +-          -+
*/

glm::mat4 Matrix4x4::ToGlm() const
{
	glm::mat4 r(1.0f);
	// Vector 0
	r[0] = glm::vec4(v[0].x, v[0].y, v[0].z, v[0].w );
	r[1] = glm::vec4(v[1].x, v[1].y, v[1].z, v[1].w );
	r[2] = glm::vec4(v[2].x, v[2].y, v[2].z, v[2].w );
	r[3] = glm::vec4(v[3].x, v[3].y, v[3].z, v[3].w );

	r = glm::transpose(r);
	return r;
}

Matrix4x4 Matrix4x4::InvertedTR() const
{
	// This method can only be used if the matrix is a translation/rotation matrix.
	// The below asserts will trigger if this is not the case.
	//TAssert(fabs(GetForwardVector().LengthSqr() - 1) < 0.00001f);   // Each basis vector should be length 1.
	//TAssert(fabs(GetUpVector().LengthSqr() - 1) < 0.00001f);
	//TAssert(fabs(GetRightVector().LengthSqr() - 1) < 0.00001f);
	//TAssert(fabs(GetForwardVector().Dot(GetUpVector())) < 0.0001f); // All vectors should be orthogonal.
	//TAssert(fabs(GetForwardVector().Dot(GetRightVector())) < 0.0001f);
	//TAssert(fabs(GetRightVector().Dot(GetUpVector())) < 0.0001f);


	Matrix4x4 M;

	// M^-1 = R^-1 * T^-1
	/*
			 +-          -+  +-           -+   +-            -+  
			 |            |  |             |   |              |  
	LookAt = | R^T      0 |  | I       -t  | = | R^T  -(R^T)*t|  
			 |            |  |             |   |              |  
			 | 0        1 |  | 0        1  |   | 0          1 |  
			 +-          -+  +-           -+   +-            -+  
	*/

	// Create the transpose upper 3x3 matrix
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			M.m[i][j] = m[j][i];

	// The new matrix translatrion = -Rt
	M.SetTranslation(-(M*GetTranslation()));
	return M;
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

	// Matrix "r-Rotation"
	// -------------------
	Vector zaxis_D = position - target;
	zaxis_D = zaxis_D.Normalized();

	Vector xaxis_R = worldUp.Cross(zaxis_D);
	xaxis_R = xaxis_R.Normalized();

	Vector yaxis_U = zaxis_D.Cross(xaxis_R);
	yaxis_U.Normalized();

	Matrix4x4 r(xaxis_R, yaxis_U, zaxis_D);

	// Matrix "t-Translation"
	// -------------------
	Matrix4x4 t;
	t.m[0][3] = -position.x;
	t.m[1][3] = -position.y;
	t.m[2][3] = -position.z;

	return r * t;

}


// utility overload for iostream Vertex Data
std::ostream & operator<<(std::ostream & stream, const Matrix4x4 & matrix)
{
	stream << "-----------Matrix4x4-----[row][col]--------------\n";
	//stream << "M4x4 mm: (" << matrix.m[0][0] << ", " << matrix.m[0][1] << ", " << matrix.m[0][2] << ", " << matrix.m[0][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix.m[1][0] << ", " << matrix.m[1][1] << ", " << matrix.m[1][2] << ", " << matrix.m[1][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix.m[2][0] << ", " << matrix.m[2][1] << ", " << matrix.m[2][2] << ", " << matrix.m[2][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix.m[3][0] << ", " << matrix.m[3][1] << ", " << matrix.m[3][2] << ", " << matrix.m[3][3] << ")\n";

	stream << "my_:M4d4 v0: (" << matrix.v[0].x << ", " << matrix.v[0].y << ", " << matrix.v[0].z << ", " << matrix.v[0].w << ")\n";
	stream << "my_:M4d4 v0: (" << matrix.v[1].x << ", " << matrix.v[1].y << ", " << matrix.v[1].z << ", " << matrix.v[1].w << ")\n";
	stream << "my_:M4d4 v0: (" << matrix.v[2].x << ", " << matrix.v[2].y << ", " << matrix.v[2].z << ", " << matrix.v[2].w << ")\n";
	stream << "my_:M4d4 v0: (" << matrix.v[3].x << ", " << matrix.v[3].y << ", " << matrix.v[3].z << ", " << matrix.v[3].w << ")";
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const glm::mat4 & matrix)
{
	stream << "-----------glm::mat4-----[col][row]--------------\n";
	//stream << "M4x4 mm: (" << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << ", " << matrix[0][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << ", " << matrix[1][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << ", " << matrix[2][3] << ")\n";
	//stream << "M4x4 mm: (" << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2] << ", " << matrix[3][3] << ")\n";

	stream << "glm:M4x4 v0: (" << matrix[0][0] << ", " << matrix[1][0] << ", " << matrix[2][0] << ", " << matrix[3][0] << ")\n";
	stream << "glm:M4x4 v0: (" << matrix[0][1] << ", " << matrix[1][1] << ", " << matrix[2][1] << ", " << matrix[3][1] << ")\n";
	stream << "glm:M4x4 v0: (" << matrix[0][2] << ", " << matrix[1][2] << ", " << matrix[2][2] << ", " << matrix[3][2] << ")\n";
	stream << "glm:M4x4 v0: (" << matrix[0][3] << ", " << matrix[1][3] << ", " << matrix[2][3] << ", " << matrix[3][3] << ")";
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const glm::vec4 & vec4)
{
	stream << "-----------glm::Vec4----------------------------\n";
	stream << "glm::Vec4:   (" << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << ")";
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const Vector4d vec4)
{
	stream << "-----------Vector4d---------------------------\n";
	stream << "Vector4d :   (" << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << ")";
	return stream;
}
