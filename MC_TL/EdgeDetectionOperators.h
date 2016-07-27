#ifndef EDGE_DETECT_OPERATORS_H
#define EDGE_DETECT_OPERATORS_H

#include "Matrix3D.h"
#include "global.h"
#include <iostream>
using namespace std;

class EdgeDetectionOperators{
	
public: EdgeDetectionOperators(){}

		//void LaplacianOperator();
void LaplacianOperator(Matrix3D<float>& m3D_dataPoints);
		//Matrix3D<short>& getMartix3D(){ return m3D_dataPoints; }
Matrix3D<float>& getMartix3D(){ return m3D_edgePoints; }
	
private: 
	//Matrix3D<short> m3D_edgePoints;
	Matrix3D<float> m3D_edgePoints;
};
#endif