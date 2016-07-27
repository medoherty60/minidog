#include "cube.h"
#include <vector>
using namespace std;

Point Cube::getVertex_indexPos(const int cpos){
	switch (cpos) {
		case 0: return (pt); break;                  //current pos (front)
		case 1: return (pt+Vector(1,0,0)); break;    //right lower
		case 2: return (pt+Vector(1,1,0)); break;    //right upper
		case 3: return (pt+Vector(0,1,0)); break;    //left upper
		case 4: return (pt+Vector(0,0,1)); break;    //current pos (back)
		case 5: return (pt+Vector(1,0,1)); break;    //right lower
		case 6: return (pt+Vector(1,1,1)); break;    //right uppper
		case 7: return (pt+Vector(0,1,1)); break;    //left lower
		}
		if(cpos >7)
		cout<<"********** Error (getVertex) in CubeManager********"<<endl;
}
void Cube::setIndex_Matrix3D(int _x, int _y, int _z){
	
	pt = Point(_x, _y, _z);	
}
void Cube::setPoint(int _largeNum){
	
	pt = Point(_largeNum, _largeNum, _largeNum);	
}
static bool close(const Point& p1, const Point&p2)
{
	float dx = p1.x()-p2.x();
	float dy = p1.y()-p2.y();
	float dz = p1.z()-p2.z();
	float d = sqrt(dx*dx+dy*dy+dz*dz);
	return d<0.001;
}

 

	
