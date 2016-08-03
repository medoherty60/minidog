#ifndef WRITEFILE_H
#define WRITEFILE_H

#include "Matrix3D.h"
#include <sstream>
using namespace std;

class WriteFile{
	
public: 
	
//Used by Gaussian Smoothing
WriteFile(string _fname, const char _fs, const float _sig){
	sprintf(name, "%s_[%0.3f,%d].raw", _fname.c_str(), _sig, short(_fs));
	 print_fname();
}
//Used by Canny
WriteFile(string _fname, const char* file_ext){
	
	string fname = _fname+".%s";
	sprintf(name, fname.c_str(), file_ext);
	 print_fname();
}
void print_fname(){
	
	cout<<"output filename: "<<name<<endl;
}

void writeHistogram(vector<int>& index, vector<int>& countX, vector<int>& countY, vector<int>& countZ){

 
	FILE *wp = fopen(name, "a");
	for(int i=0; i<index.size(); i++)
		fprintf(wp, "%d,%d,%d,%d,%d,%d\n", index[i], countX[i], index[i], countY[i], index[i], countZ[i]);
	
	fprintf(wp, "\n");
	fclose(wp);
}
void writeCubesStats(float upperThreshold, int count1, int count2){

		FILE *wp = fopen(name, "a");
		fprintf(wp, "%f,%d,%f,%d,\n", upperThreshold, count1, upperThreshold, count2);
		fclose(wp);
}
void writeFloat(Matrix3D<float>& m3D_rData, unsigned int _x, unsigned int _y, unsigned int _z){
	
	int n=0;
	FILE *wp = fopen(name, "wb");	
	
	for (int i=0; i<_z; i++) {
		float* slice_ptr = m3D_rData.getSlicePtr(i);
		n = fwrite(slice_ptr, 4, _x*_y, wp);
	}
	fclose(wp);
}

void writeCubes(Matrix3D<char>& data, unsigned int _x, unsigned int _y, unsigned int _z){
		
		int n=0;
		FILE *wp = fopen(name, "wb");	
		
		for (int i=0; i<_z; i++) {
			char* slice_ptr = data.getSlicePtr(i);
			n = fwrite(slice_ptr, 1, _x*_y, wp);
		}
		fclose(wp);
}

private: 
	char name[200];
	
};
#endif