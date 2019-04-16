#include <iostream>

class Vector
{
public:
	float Length() const;
	
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
	v = p - i;
	std::cout << "Result  [ p - i ] : (" << v.x << ", " << v.y << ")\n";

	float lenght = v.Length();
	std::cout << "Result  [Length]  : " << lenght << "\n";

	std::cout << "Pause..." << std::endl;  
	std::cin.get();
}