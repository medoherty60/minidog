#ifndef CUBE_DOT_H
#define CUBE_DOT_H

#include "Configurations.h"
#include "Point.h"
#include "Color.h"
#include <iostream>
#include <vector>
using namespace std;

class Cube
{	
public:
	float cube[8];			      //the 8 corner values
	Vector gradient[8];            //the 8 corner gradients
							      //float e1, e2;			//2 new points containing density values (used to solve ambigous cases)
	Point pt;			          //Matrix3D index 
	short activeCorners;	      //number of corners that are smaller than isovalues (0-8)
	short cornerFlag;		      //bits
	vector<short> cornerPos;      //vector of corner positions

	vector<Color> colors;
	Color color, red, green, blue, yellow, magenta, white, gray, lightblue;

public: 
	Cube() : cornerFlag(0), activeCorners(0),
		     red(.5,0.,0.), green(0.0,0.5,0.), blue(0.0,0.,0.5), yellow(0.5,0.5,0.),
		     magenta(1.0,0.,1.), white(1.,1.,1.), gray(.5,.5,.5), lightblue(.2,0.8,1.){

				 colors.resize(8);
				 colors[0]=red;			colors[4]=magenta;
				 colors[1]=green;		colors[5]=white;
				 colors[2]=blue;		colors[6]=gray;
				 colors[3]=yellow;		colors[7]=lightblue;
				 
				 setColor(0);
	}
	
	void setPoint(int _largeNum);
	void setIndex_Matrix3D(int _x, int _y, int _z);
	void setOneCube(int n, float val) { cube[n] = val; }
	void setGradients(int n, Vector& g) { gradient[n] = g; }
	void setColor(int x) { color = colors[x]; }
	
	float getDensity(int const n){ return cube[n]; }
	Vector getGradient(int const n) { return gradient[n]; }
	Color getColor() { return color; }
	
	vector<short>getCornerPos(){ return cornerPos; }
	short getActiveCorner(){ return activeCorners; }
	Point getIndex_Matrix3D(){ return pt; }
	Point getVertex_indexPos(const int cpos);

};
#endif


