#include "mfgd/mfgd_Classes.h"


// Vector-----------------------------------------------
//------------------------------------------------------

Vector::Vector()
	: x(0), y(0), z(0) {}

Vector::Vector(float x, float y, float z)
	: x(x), y(y), z(z) {}

Vector::Vector(const Point & p)
	: x(p.x), y(p.y), z(p.z) {}

float Vector::Length() const
{
	float length;
	length = std::sqrt(x*x + y*y + z*z);
	return length;
}

float Vector::LengthSqr() const
{
	float length;
	length = (x*x + y*y + z*z);
	return length;
}

Vector Vector::operator+() const
{
	return Vector(x, y, z);
}

Vector Vector::operator-() const
{
	return Vector(-x, -y, -z);
}

Vector Vector::operator+(const Vector& v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator-(const Vector& v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator*(float s) const
{
	Vector scaled;
	scaled.x = x * s;
	scaled.y = y * s;
	scaled.z = z * s;
	return scaled;
}

Vector Vector::operator/(float s) const
{
	Vector scaled;
	scaled.x = x / s;
	scaled.y = y / s;
	scaled.z = z / s;
	return scaled;
}

Vector Vector::Normalized() const
{
	Vector Normalized;
	Normalized = (*this) / Length();
	return Normalized;
}

float Vector::Dot(const Vector & v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector Vector::Cross(const Vector & v) const
{
	Vector c;
	c.x = y * v.z - z * v.y;
	c.y = z * v.x - x * v.z;
	c.z = x * v.y - y * v.x;
	return c;
}

// Point-----------------------------------------------
//------------------------------------------------------
Vector operator-(Point a, Point b)
{
	Vector v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	return v;
}

Point::Point()
	: x(0), y(0), z(0) {}

Point::Point(float x, float y, float z)
	: x(x), y(y), z(z) {}

float Point::Length() const
{
	return std::sqrt(x*x + y * y + z * z);
}

float Point::LengthSqr() const
{
	return (x*x + y * y + z * z);
}

Point Point::AddVector(Vector v)
{
	Point p2;
	p2.x = x + v.x;
	p2.y = y + v.y;
	p2.z = z + v.z;
	return p2;
}

Point Point::operator-() const
{
	return Point(-x, -y, -z);
}

Point Point::operator+(const Vector & v) const
{
	return Point(x + v.x, y + v.y, z + v.z);
}

Point Point::operator-(const Vector & v) const
{
	return Point(x - v.x, y - v.y, z - v.z);
}

float DotProduct(const Vector& a, const Vector& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


// Test MFGD-------------------------------------------
//------------------------------------------------------
void Test_MFGD()
{
	Point p{ 1, 0, 0 };
	Vector v{ 2, 3, 0 };

	Point p2 = p.AddVector(v);
	std::cout << "Result [AddVector]: (" << p2.x << ", " << p2.y << ", " << p2.z << ")\n";

	p = { 0,-1, 0 };
	Point i{ 1, 1, 0 };
	Point c{ 2, -1, 0 };
	v = p - i;
	std::cout << "Result  [ p - i ] : (" << v.x << ", " << v.y << ", " << v.z << ")\n";

	float lenght = v.Length();
	std::cout << "Result  [Length]  : " << lenght << "\n";

	// 4 MFGD - Distance Comparison
	Vector ip = p - i;
	Vector cp = p - c;

	std::cout << "Length square of CP: " << cp.LengthSqr() << "\n";
	std::cout << "Length square of IP: " << ip.LengthSqr() << "\n";

	// 5 MFGD - Vector scaling
	v = Vector(3, 4, 0);
	std::cout << "Pac-man's initial speed: " << v.Length() << "\n";
	Vector doubled = v * 2;
	std::cout << "Pac-man's doubeld speed: " << doubled.Length() << "\n";
	Vector halved = v / 2;
	std::cout << "Pac-man's halved speed: " << halved.Length() << "\n";

	// 6 MFGD - Normalized
	i = { 3, 4, 0 };
	p = { 1, 2, 0 };
	Vector pi = i - p;
	Vector normalized = pi.Normalized();

	std::cout << "Pac-man's view vector : (" << normalized.x << ", " << normalized.y << ", " << normalized.z << ")\n";
	std::cout << "Pac-man's view vector length: (" << normalized.Length() << ")\n";

	// 7-8 MFGD - Adding Vectors
	Vector r(4, 0, 0);
	Vector d(0,-5, 0);

	v = r + d;
	std::cout << "Pac-man's new velocity: (" << v.x << ", " << v.y << ", " << v.z << ")\n";

	// 9-10 MFGD - Dot Product 
	std::cout << "DotProduct of r * r: (" << DotProduct(r, r) << ")\n";
	std::cout << "DotProduct of r * d: (" << DotProduct(r, d) << ")\n";

	//std::cout << "Pause..." << std::endl;
	//std::cin.get();
}


// MFGD 12 - Smooth Move(ment) (Linear Interpolation)
//------------------------------------------------------
float Approch(float flGoal, float flCurrent, float dVelocity)
{
	float flDifference = flGoal - flCurrent;

	if (flDifference > dVelocity)			// Goal is UP
		return flCurrent + dVelocity;
	else if (flDifference < -dVelocity)	// Goal is DOWN
		return flCurrent - dVelocity;
	else return flGoal;

}


// MFGD 17 - Explosions! (The Remap Function)
// ------------------------------------------------------
float Remap(float x, float t1, float t2, float s1, float s2)
{
	float yellow = (x - t1) / (t2 - t1);		// Map [t1, t2] -> [ 0,  1]
	float green = yellow * (s2 - s1) + s1;		// Map [ 0,  1] -> [s1, s2]
	return green;
}


// MFGD 37 (The RemapClamp Function)
// ------------------------------------------------------
float RemapClamp(float x, float t1, float t2, float s1, float s2)
{
	if (x < t1)
		return s1;
	if (x > t2)
		return s2;

	return Remap(x, t1, t2, s1, s2);
}


// MFGD 18 - Bullet Whizzes (Projections)
// https://youtu.be/VTV1GTrrtBQ?list=PLW3Zl3wyJwWOpdhYedlD-yCB7WQoHf-My
// -------------------------------------------------------
float BulletWhizzes(Point player, Point enemy, Vector direction)
{
	/*
	b : vector from enemy to player
	a : vector of bullet direction
	b1: projection of <b> on <a>
	e : min distance from bullet to player	-> 
	e = b - b1
	a dot e = 0 : perpendicular vectors
	b1 = (float)x * a
	*/

	Vector b = player - enemy;
	Vector a = direction;

	float x = a.Dot(b) / a.Dot(a);

	// Vector to enemy to min distance:
	// b1 = x * a
	return 0;
}
