#ifndef PARSER_H
#define PARSER_H
#include "Matrix3D.h"
#include <string.h>
#include <stdlib.h>

enum { X, Y, Z };

class ReadFile{

public: ReadFile(const char* _fn, int _dim3D[]){
	
	w = _dim3D[X];
	h = _dim3D[Y];  
	d = _dim3D[Z];
	 
	strcpy(fname, _fn);
	smallest=10000.0;
	largest=-10000.0;
}
virtual ~ReadFile(){}

void loadData(Matrix3D<float>& m3D_Data, int bytes, char isSwapBytes){
	
	int n1=0, n2=0;
	float density=0.0;
	unsigned int slice=w*h;
	//float buffer1[slice], buffer2[slice];
	
	float* buffer1 = new float[slice];
	float* buffer2 = new float[slice];
	
    //*fp is a FILE pointer to the next bit to be read
	FILE *fp = fopen(fname, "rb");
    if (fp == NULL)
        cout<<"Open File Error:"<<fname<<endl;
	
	//n1 & n2 are number of words (oneSlice)
	n1 = readBuffer(buffer1, bytes, slice, fp, isSwapBytes);
	n2 = readBuffer(buffer2, bytes, slice, fp, isSwapBytes);
	if (n1<slice || n2<slice) cout<<"Reading Error 1"<<endl;
	
	int j=0;
	m3D_Data.resize(w, h, d);
	
	for (int z=0; z<d; z++){
		for (int y=0; y<h; y++){
			for (int x=0; x<w; x++) {		
				
				density = buffer1[j]; 	
				j++;
				m3D_Data.set(x, y, z, density);		
				if(density<smallest) smallest=density;
				if(largest<density) largest=density; 
			}
		}
		memcpy(buffer1, buffer2, slice*sizeof(float));	//destination, source, num bytes
		
		if(z<d-2){
			n2 = readBuffer(buffer2, bytes, slice, fp, isSwapBytes);
			if (n2<slice) { cout<<"[z, Reading Error 2] = ["<<z<<", "<<n2<<"]"<<endl; }
		}
		j=0;
	}//end for loop
	
	delete [] buffer1;
	delete [] buffer2;
}
void image_small(float& img_small){ img_small = smallest; }
void image_large(float& img_large) { img_large = largest; }
	
// format: 1=float, 2=short
int readBuffer(float* buffer, char bytes, int num_words, FILE* fp, char isSwapBytes);
union swapUnion2{
	short x;
	unsigned char c[2];
};
union swapUnion3{
		unsigned short x;
		unsigned char c[2];
	};
union swapUnion4{
	float x;
	unsigned char c[4];
};
	unsigned short swapBytes(unsigned short val);
short swapBytes(short val);
float swapBytes(float val);
	
void print(){
	cout<<"file name = "<<fname<<endl;
    cout<<"[w,h,d] = ["<<w<<","<<h<<","<<d<<"]"<<endl;
	cout<<"Image Range: [high, low] (from file) = ["<<largest<<", "<<smallest<<"]"<<endl;
}
	
private: 
	string dir;
	char fname[120];
	unsigned int w, h, d;
	float smallest, largest;
	
};
#endif
