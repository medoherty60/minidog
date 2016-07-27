#include "Vector.h"
#include <iostream>
#include "Point.h"

Vector::Vector(const Point& copy)
{
	data[0] = copy.x(); data[1] = copy.y(); data[2] = copy.z();
}

std::ostream& operator<<(std::ostream& out, const Vector& v)
{
	out << '[' << v.x() << ", " << v.y() << ", " << v.z() << ']';
	return out;
}


