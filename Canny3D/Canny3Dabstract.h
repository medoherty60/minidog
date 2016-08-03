#ifndef CANNY3DABSTRACT_NEW_H
#define CANNY3DABSTRACT_NEW_H

#include "Matrix3D.h"
#include "Threshold.h"
#include "Vector.h"
#include "WriteFile.h"
#include "Point.h"
#include <utility>
using namespace std;

enum RangeIndex { HIGH, LOW };
enum CoordAxis { _X_, _Y_, _Z_ };

class Canny3Dabstract {
	
public:
	virtual ~Canny3Dabstract() {};

	virtual void Find_Initial_Seed_EdgeCubes() = 0;
	virtual void Hysteresis(int& isRelax) = 0;

	virtual void print(Matrix3D<char>& markers, string text) = 0;
	virtual void print() = 0;
	virtual void print_relax() = 0;
	virtual void writeToCubesFile(string _fname, string file_extension, char _w) = 0;
	virtual void writeToCubesStatisticFile(string _fname, string file_extension, float upperThreshold, char _w) = 0;

	virtual Matrix3D<char>* get_cubes_Martrix() = 0;
	virtual int get_num_passes() = 0;
	virtual int get_num_edgeMarks() = 0;
	virtual int get_num_cubeMarks() = 0;
	virtual int get_num_markedCubes_Final() = 0;
	virtual int get_cubeMarks0_Final() = 0;
	virtual int get_cubeMarks1_Final() = 0;
	virtual int get_cubeMarks2_Final() = 0;
	virtual int get_cubeMarks3_Final() = 0;
};
#endif
