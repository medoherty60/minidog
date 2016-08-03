/*
 *  Convolution3D.h
 *  Created by Marina Doherty on 5/11/13.
 */
#ifndef CONVOLUTION3D_H
#define CONVOLUTION3D_H

#include "Matrix3D.h"
#include "FilterTemplate.h"
#include "WriteFile.h"

enum { _X, _Y, _Z };

class GaussianFilter3D{

public: GaussianFilter3D(const char _fs, const float _sig, unsigned int _x, unsigned int _y, unsigned int _z){
	
	m3D_rData.resize(_x, _y, _z);
	m3D_rData.setAll(-1000.);
	width = _x; height= _y; slice = _z;
	fsize = _fs; hsize = (_fs-1)/2;
	sigma = _sig;
	f_template = new FilterTemplate(_fs, _sig);
}
public: GaussianFilter3D(const char _fs, int _dim3D[]){
	
	m3D_rData.resize(_dim3D[_X], _dim3D[_Y], _dim3D[_Z]);
	m3D_rData.setAll(-1000.);
	width = _dim3D[_X]; height= _dim3D[_Y]; slice = _dim3D[_Z];
	fsize = _fs; hsize = (_fs-1)/2;
	sigma = hsize/3.0;
	f_template = new FilterTemplate(_fs, sigma);
}
virtual ~GaussianFilter3D(){
	delete f_template; 
}
void print(){
	cout<<"fsize="<<fsize<<", sigma="<<sigma<<endl;
	f_template->print();
}
void clampBorderIdx(int& val, int borderVal){
	
	if(val<0) val=0;
	else if(val>=borderVal) val = borderVal-1;
	
}
void convolute(Matrix3D<float>& m3D_Data, const char write, string fname){
					 
		Matrix3D<float>& filterMatrix = f_template->getFilterMatrix(); 
					 
		double rVal=0.0;
		int tx=0, ty=0, tz=0;

		 for (int z=0; z<slice; z++){	
			 for (int y=0; y<height; y++){
				 for(int x=0; x<width; x++){
					 
				  rVal = 0.0;
				  for(int k=-hsize; k<=hsize; k++)
					  for(int j=-hsize; j<=hsize; j++)
						  for(int i=-hsize; i<=hsize; i++){
							
							  //::check kernal index within data domain
							  // use border values for filter indices beyond the data border
							  tx=x+i; ty=y+j; tz=z+k;    
							  
							  clampBorderIdx(tx, width);
							  clampBorderIdx(ty, height);
							  clampBorderIdx(tz, slice);
							  
							 rVal += m3D_Data.get(tx,ty,tz) * filterMatrix.get(i+hsize,j+hsize,k+hsize);
				   }
				   m3D_rData.set(x,y,z,rVal);
				 }
			   }
		   }
	if(write){
		WriteFile w(fname, fsize, sigma);
		w.writeFloat(m3D_rData, width, height, slice);
	}
}
	
Matrix3D<float>* getFilteredMatrix(){ return &m3D_rData; }
	
int get_fsize(){ return fsize; }
int get_hsize(){ return hsize; }

private: 
	FilterTemplate *f_template;  
	Matrix3D<float> m3D_rData;
	unsigned int width, height, slice;
	unsigned int fsize;
	int hsize;
	float sigma;

};
#endif