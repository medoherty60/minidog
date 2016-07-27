#ifndef GLOBALS_H
#define GLOBALS_H

#include "Face.h"
#include <vector>
using namespace std;

#define AIR -1000
extern vector<Face> global_facesVector;

extern int getNumFaces();
Face getOneFace(int iv);

#endif