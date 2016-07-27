/*
 *  Date:     June 3, 2011
 *  Author:   Marina Doherty
 *  File:     Matrix3D.h
 *
 */
#ifndef MATRIX3D_DOT_H
#define MATRIX3D_DOT_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
using namespace std;

template<class T> 
class Matrix3D{
	
private: 
	T* m;
	int width; 
	int height;
	int slice;
	
public:
	Matrix3D(int _width=1, int _height=1, int _slice=1)
	: m(NULL)
	{
		resize(_width,_height,_slice);
	}
	Matrix3D(const Matrix3D& other)
	{
		resize(other.width, other.height, other.slice);
		memcpy(m, other.m, width*height*slice*sizeof(T));
	}
	Matrix3D& operator=(const Matrix3D& other)
	{
		resize(other.width, other.height, other.slice);
		memcpy(m, other.m, width*height*slice*sizeof(T));
		return *this;
	}
	~Matrix3D()
	{
		if (m != NULL) delete [] m;
	}
	void resize(int _width, int _height, int _slice)
	{
		width=_width;
		height=_height;
		slice = _slice;
		if (m != NULL) delete [] m;
		m = new T[width*height*slice];
		memset(m, 0, width*height*slice*sizeof(T));
	}
	T& get(int c, int r, int d){
		
		return m[(width*height*d)+(width*r)+c];   //column-major order
	}
	
	T* getPointer() { return m; }
	
	void set(int c, int r, int d, T val){
		
		m[(width*height*d)+(width*r)+c]=val;		 //column-major order
	}
	void setAll(T val)
	{
		for (int i=0; i<width; i++)
			for (int j=0; j<height; j++)
				for (int k=0; k<slice; k++)
				set(i, j, k, val);
	}
	int getWidth(){
		return width;
	}
	int getHeight(){
		return height;
	}
	int getSlice(){
		return slice;
	}

/*
	T* getColumnPtr(int c){
		return &(m[height*c]);		//get address of one column;
	}*/
	//num of output rows, max output rows, num of output columns,, max output col, normalization
	void print(int c=0, int w=-1, int r=0, int h=-1, int d=0, int s=-1, float cpf=-1){
		if (h==-1) { h=getHeight(); }
		if (w==-1) { w=getWidth(); }
		if (s==-1) { s=getSlice(); }
		
		if (cpf == -1)
		{
			for (int k=d; k<s; k++) {			//slice (depth)
				for (int j=r; j<h; j++) {		//height (row)
					for (int i=c; i<w; i++) {	//width (column)
						float item = get(i,j,k);
							//cout<<"[x,y,z]=["<<i<<","<<j<<","<<k<<"]"<<"="<<item<<"\t";		
						cout<<setw(10)<<item;	
					}
					cout<<endl;
				}
			}
			cout << "------------------------"<<endl;
		}
		else {

		   for (int i=d; i<s; i++) {		 //slice (depth)
			  for (int j=r; j<h; j++) {      //height (row)
				for (int k=c; k<w; k++) {    //width (column)
					float item = get(i,j,k);
					cout<<item<<"\t";
					if (item!=0.0) {
						cout<<endl;
						cout<<"position for value: i="<<i<<",j="<<j<<",k="<<k<<endl;
					}

				}
				cout<<endl;
				if (cpf!=-1) {
					cout<<"Normalized cost per frame = "<<get(h-1,w-1,s-1)/cpf<<endl;
				}
				cout << "------------------------"<<endl;
			}
		}
	}
	}
	/*
	void MultiplyElements(Matrix3D<double>& x)
	{
		if ((x.getWidth() != getWidth()) || (x.getHeight() != getHeight())) 
		{
			cout << "!!! INCOMPATIBLE MATRIX TO Matrix3D.MultiplyElements ... aborting operation" << endl;
			return;
		}
		for (int r=0; r<getHeight(); r++)
			for (int c=3; c<getWidth(); c++)	// (c starts at 3 to skip root rotations)
				set(r,c,get(r,c)*x.get(r,c));
	}

	void AddElements(Matrix3D<double>& x)
	{
		if ((x.getWidth() != getWidth()) || (x.getHeight() != getHeight())) 
		{
			cout << "!!! INCOMPATIBLE MATRIX TO Matrix3D.AddElements ... aborting operation" << endl;
			return;
		}
		for (int r=0; r<getHeight(); r++)
			for (int c=3; c<getWidth(); c++)   // (c starts at 3 to skip root rotations)
				set(r,c,get(r,c)+x.get(r,c));
	}*/
};
#endif



