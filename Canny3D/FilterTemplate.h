/*
 *  Date:     May 9, 2013
 *  Author:   Marina Doherty
 *  Desc:     Pre-compute a 3D Gaussian filter template 
 */
#ifndef	FILTERTEMPLATE_H
#define FILTERTEMPLATE_H

#include "Matrix3D.h"
#include "math.h"
#include <iostream>
using namespace std;

class FilterTemplate{

public: FilterTemplate(const char _fs, const float _sd){

	fsize = _fs;
	variance = _sd * _sd;
	Dfilter=new float[_fs];
	fsheet=new float[_fs*_fs];
	FilterMatrix.resize(_fs, _fs, _fs);
	compute();
}
	
virtual ~FilterTemplate(){
	
	delete[] Dfilter;
	delete[] fsheet;
}
	
void compute(){
	
	int j=0, m=0, n=0;
	int hsize = (fsize - 1)/2; 
	double sum = 0, v=0;
	
	for (int i = 0; i < fsize; i++) {
		j = i - hsize;
		v = exp( -j*j/(2.*variance));
		Dfilter[i] = v;
		sum += v;
	}
	for (int i = 0; i < fsize; i++) {
	    Dfilter[i] /= sum;
	}
	for (int l = -hsize; l <= hsize; l++)  		
	   for (int k = -hsize; k <= hsize; k++) {
		   fsheet[m] = Dfilter[k + hsize]*Dfilter[l + hsize]; m++;		
	}
	int x=0, y=0;
	for(int z=0; z<fsize; z++)
		for(int i=0; i<(fsize*fsize); i++){
			y=i/fsize;
			FilterMatrix.set(x,y,z,fsheet[i]*Dfilter[z]);
			x++; if(x==fsize) x=0;
	}
}
Matrix3D<float>& getFilterMatrix(){
	return FilterMatrix;
}
void print(){
	print_Dfilter();
	print_fsheet();
	FilterMatrix.print();
}
void print_Dfilter(){
	cout << "Dfilter=[\t\t";
	for (int i = 0; i < fsize; i++) {
		cout<<Dfilter[i]<<"\t\t";
	}cout <<"]"<<endl<<endl;
}
void print_fsheet(){
	cout << "fsheet=[";
	for (int i=0; i<(fsize*fsize); i++){	
		if(i%fsize==0) cout<<endl;
		cout<<fsheet[i]<<"\t";	
		
	}cout <<"]"<<endl<<endl;
}

private:
	char fsize;
	float variance;
	float *Dfilter, *fsheet;
	Matrix3D<float> FilterMatrix;
};
#endif