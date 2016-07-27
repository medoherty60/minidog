#include "global.h"

vector<Face> global_facesVector;
int getNumFaces(){ return global_facesVector.size(); }
Face getOneFace(int iv){ return global_facesVector[iv]; }



