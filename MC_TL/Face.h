#ifndef Face_h
#define Face_h

#include <vector>
#include <iosfwd>
#include "Point.h"
#include "Vector.h"
#include "cube.h"
#include "Color.h"
using namespace std;

class Face
{
private:
	Point p1, p2, p3;
	Vector n1, n2, n3;
	Cube* shadowCube;
	Color face_color;
	int drawFaceFlag, faceID, numberFaces, isReversed;
	
public:
	
	Face():face_color(.5f, 0.f, 0.f, 1.0f){ 
		drawFaceFlag=0;
		numberFaces=0;
		faceID=0;
		isReversed=0;
		shadowCube = NULL;
	}
	//new function
	void addOneFace(vector<Point>& _triPoints, vector<Vector>& _normalVectors){
		
		p1=_triPoints[0]; p2=_triPoints[1]; p3=_triPoints[2];
		n1=_normalVectors[0]; n2=_normalVectors[1]; n3=_normalVectors[2];
	}
	
	//old function
	void addOneFace(Cube* _c, vector<Point>& _triPoints, vector<Vector>& _normalVectors, 
					int _numfaces, int _fid, int _drawFace, int _isReversed){
		
		p1=_triPoints[0]; p2=_triPoints[1]; p3=_triPoints[2];
		n1=_normalVectors[0]; n2=_normalVectors[1]; n3=_normalVectors[2];
		
		shadowCube = _c;
		face_color = _c->getColor();
		drawFaceFlag = _drawFace;    //0=good gradients, 1=bad gradients
		numberFaces = _numfaces;     //number of faces in a cube
		faceID = _fid;
		isReversed = _isReversed;
		
	}
	int isDrawFace(){ return drawFaceFlag; }
	Cube* getCube(){ return shadowCube; }
	int getNumFaces(){ return numberFaces; }
	int getFaceID(){ return faceID; }
	int getIsReversed() { return isReversed; }
	void PrintFaceTable();
	
	Point point1() const { return p1; }
	Point point2() const { return p2; }
	Point point3() const { return p3; }
	
	Vector normal1() const { return n1; }
	Vector normal2() const { return n2; }
	Vector normal3() const { return n3; }

	Color getColor() { return face_color; }
	
	float r() { return face_color.red(); }
	float g() { return face_color.green(); }
	float b() { return face_color.blue(); }

	friend std::ostream& operator<<(std::ostream&, const Face& f);
};
#endif
