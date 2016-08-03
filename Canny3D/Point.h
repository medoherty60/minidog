#ifndef Point_h
#define Point_h

#include "Vector.h"
#include <iosfwd>

/*
 *  Point.h
 *  Created by Marina Doherty on 10/11/11.
 */

class Point
{
public: 
	Point(){
		data[0]=0., data[1]=0.; data[2]=0.;
	}
	Point(float x, float y, float z){
		data[0]=x, data[1]=y; data[2]=z;
	}
	
	Point(const Point& copy) {
		for(int i=0;i<3;i++)
			data[i] = copy.data[i];
	}
	Point(const Vector& copy) {
	
		data[0] = copy.x();
		data[1] = copy.y();
		data[2] = copy.z();
	}
	Point& operator=(const Point& copy) {
		for(int i=0;i<3;i++)
			data[i] = copy.data[i];
		return *this;
	}
	
	~Point() {
	}
	float x() const {
		return data[0];
	}
	float y() const {
		return data[1];
	}
	float z() const {
		return data[2];
	}
	Vector operator-(const Point& p) const {
		return Vector(data[0]-p.data[0], data[1]-p.data[1], data[2]-p.data[2]);
	}
	Point operator+(const Vector& v) const {
		return Point(data[0]+v.x(), data[1]+v.y(), data[2]+v.z());
	}
	Point operator+(const Point& p) const {
		return Point(data[0]+p.x(), data[1]+p.y(), data[2]+p.z());
	}
	Point operator-(const Vector& v) const {
		return Point(data[0]-v.x(), data[1]-v.y(), data[2]-v.z());
	}
	Point operator*(const float& s) const {
		return Point(data[0]*s, data[1]*s, data[2]*s);
	}
	Point operator/(const float& s) const {
		return Point(data[0]/s, data[1]/s, data[2]/s);
	}
	friend std::ostream& operator<<(std::ostream&, const Point& v1);
private:
	float data[3];
};
#endif




