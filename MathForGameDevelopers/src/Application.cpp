#include <iostream>

class Vector
{
public:
	Vector() {}
	Vector(float x, float y)
		: x(x), y(y) {}

	float Length() const;
	float LengthSqr() const;
	
	Vector operator*(float) const;
	Vector operator/(float) const;

	float x, y;
};


class Point
{
public:
	Point AddVector(Vector v);
	
	float x, y;
};


float Vector::Length() const
{
	float length;
	length = std::sqrt(x*x + y*y);
	return length;
}

float Vector::LengthSqr() const
{
	float length;
	length = (x*x + y*y);
	return length;
}

Vector Vector::operator*(float s) const
{
	Vector scaled;
	scaled.x = x * s;
	scaled.y = y * s;
	return scaled;
}

Vector Vector::operator/(float s) const
{
	Vector scaled;
	scaled.x = x / s;
	scaled.y = y / s;
	return scaled;
}


Vector operator-(Point a, Point b)
{
	Vector v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	
	return v;
}

Point Point::AddVector(Vector v)
{
	Point p2;
	p2.x = x + v.x;
	p2.y = y + v.y;

	return p2;
}

int main(int argc, char** args)
{
	Point p{ 1, 0 };
	Vector v{ 2, 3 };

	Point p2 = p.AddVector(v);
	std::cout << "Result [AddVector]: (" << p2.x << ", " << p2.y << ")\n";

	p = { 0,-1 };
	Point i{ 1, 1 };
	Point c{ 2, -1 };
	v = p - i;
	std::cout << "Result  [ p - i ] : (" << v.x << ", " << v.y << ")\n";

	float lenght = v.Length();
	std::cout << "Result  [Length]  : " << lenght << "\n";

	// 4 MFGD - Distance Comparison
	Vector ip = p - i;
	Vector cp = p - c;

	std::cout << "Length square of CP: " << cp.LengthSqr() << "\n";
	std::cout << "Length square of IP: " << ip.LengthSqr() << "\n";

	// 5 MFGD - Vector scaling
	v = Vector(3, 4);
	std::cout << "Pac-man's initial speed: " << v.Length() << "\n";
	Vector doubled = v * 2;
	std::cout << "Pac-man's doubeld speed: " << doubled.Length() << "\n";
	Vector halved = v / 2;
	std::cout << "Pac-man's halved speed: " << halved.Length() << "\n";

	std::cout << "Pause..." << std::endl;  
	std::cin.get();
}