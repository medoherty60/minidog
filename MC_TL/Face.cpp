#include "Face.h"
#include "global.h"
#include <iostream>

void Face::PrintFaceTable(){
	cout<<"FaceTable{"<<endl;
	for (int i=0; i<global_facesVector.size(); i++){
		cout<<i<<"= "<<global_facesVector[i] <<endl;
	}
	cout<<"}"<<endl;
}
std::ostream& operator<<(std::ostream& out, const Face& f)
{
	out << f.p1 << f.p2 << f.p3;
	return out;
}





