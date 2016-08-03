#ifndef ACCEPTANCE_TEST_H
#define ACCEPTANCE_TEST_H

#include "Canny3Dv2.h"
#include "Point.h"
#include "Matrix3D.h"
#include "WriteFile.h"

class AcceptanceTest{
	
public: AcceptanceTest(Point _p, Matrix3D<float>* _edge_strengthX, Matrix3D<float>* _edge_strengthY, Matrix3D<float>* _edge_strengthZ, 
					   Matrix3D<float>* _m3D_Data, float _image_range[], Matrix3D<char>* _edge_mark, Matrix3D<char>* _cubes){

	p = _p;
	esX = _edge_strengthX;
	esY = _edge_strengthY;
	esZ = _edge_strengthZ;
	data = _m3D_Data;
	edge_mark = _edge_mark;
	cubes = _cubes;
	
	memcpy(image_range,_image_range, sizeof(float)*2);
	run();
}

void run(){
	cout<<"-------------------  Acceptance Test (Check: ZeroCrossing and edge strength) ---------------------------"<<endl;
	bool fail_the_test = false;
	
	if(esX->get(p.x(), p.y(), p.z()) ==-9999)
		printf("[%d,%d,%d], zero-crossing X test failed\n", int(p.x()), int(p.y()), int(p.z()));
	else{
	   printf("[%d,%d,%d], strengthX=%f\n", int(p.x()), int(p.y()), int(p.z()), esX->get(p.x(), p.y(), p.z()));
	   densityFilterTest(p, edge_mark, cubes);
	}
	if(esY->get(p.x(), p.y(), p.z()) ==-9999)
		printf("[%d,%d,%d], zero-crossing Y test failed\n", int(p.x()), int(p.y()), int(p.z()));
	else{
		printf("[%d,%d,%d], strengthY=%f\n", int(p.x()), int(p.y()), int(p.z()), esY->get(p.x(), p.y(), p.z()));
		densityFilterTest(p, edge_mark, cubes);
	}
	if(esZ->get(p.x(), p.y(), p.z()) ==-9999)
		printf("[%d,%d,%d], zero-crossing Z test failed\n", int(p.x()), int(p.y()), int(p.z()));
	else{
		printf("[%d,%d,%d], strengthZ=%f\n", int(p.x()), int(p.y()), int(p.z()), esZ->get(p.x(), p.y(), p.z()));
		densityFilterTest(p, edge_mark, cubes);
	}
	
}
void densityFilterTest(Point p, Matrix3D<char>* edge_mark, Matrix3D<char>* cubes){
		
		int corner[8][3] = {{0,0,0},{1,0,0},{1,1,0},{0,1,0},{0,0,1},{1,0,1},{1,1,1},{0,1,1}};
		
			//Based on Max's new code, check if cube corner is in density range
		
		for(int c=0; c<8; c++)
		{
			int kx = p.x()+corner[c][0];   //p is the bottom-left-front corner
			int ky = p.y()+corner[c][1];
			int kz = p.z()+corner[c][2];
			
			if((kx<data->getWidth()) && (ky<data->getHeight()) && (kz<data->getSlice()))
			{
				printf("[%d]=%f", c, data->get(kx,ky,kz));
				
				if((data->get(kx,ky,kz) > image_range[LOW]) && (data->get(kx,ky,kz) < image_range[HIGH]))
				{
					//cout<<"m3D_Data: [kx,ky,kz] = ["<<m3D_Data->get(kx,ky,kz)<<"]"<<endl;
					printf(", in density range [%f]", data->get(kx,ky,kz));
				}
				printf("\n");
			}
		}
		printf("edge_mark=%d\n", int(edge_mark->get(p.x(), p.y(), p.z())));
		printf("cubes=%d\n", int(cubes->get(p.x(), p.y(), p.z())));
}
private: 
	Point p;
	Matrix3D<float>* esX;
	Matrix3D<float>* esY;
	Matrix3D<float>* esZ;
	Matrix3D<float>* data;
	Matrix3D<char>* edge_mark;
	Matrix3D<char>* cubes;
	float image_range[2];
};
#endif
