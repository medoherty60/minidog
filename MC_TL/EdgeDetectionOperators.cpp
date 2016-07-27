#include "EdgeDetectionOperators.h"

// Laplacian Operator
// • implement 2nd order differencing
//
//      z-1         z           z+1
// [ 0  0  0 ] [ 0  -1  0 ]  [ 0  0  0 ]
// [ 0 -1  0 ] [ -1  4 -1 ]  [ 0 -1  0 ]
// [ 0  0  0 ] [ 0  -1  0 ]  [ 0  0  0 ]
//
void EdgeDetectionOperators::LaplacianOperator(Matrix3D<float>& m3D_dataPoints){

	int c_weight = 6;
	int n_weight = -1;
	
	int w = m3D_dataPoints.getWidth();
	int h = m3D_dataPoints.getHeight();
	int s = m3D_dataPoints.getSlice();
	
	short curr_val =0, diff_val=0;
	short left=0, right=0, down=0, up=0, back=0, front=0;
	
	m3D_edgePoints.resize(w,h,s);
	m3D_edgePoints.setAll(AIR);

	//Edge borders are excluded
	for(int z=1; z<s-1; z++)
		for(int y=1; y<h-1; y++)
			for(int x=1; x<w-1; x++){
			
				curr_val = m3D_dataPoints.get(x,y,z);
				left  = m3D_dataPoints.get(x-1,y,z);
				right = m3D_dataPoints.get(x+1,y,z);
				down = m3D_dataPoints.get(x,y-1,z);
				up = m3D_dataPoints.get(x,y+1,z);
				back = m3D_dataPoints.get(x,y,z-1);
				front = m3D_dataPoints.get(x,y,z+1);
				
				diff_val = ((curr_val*c_weight) + 
							(left*n_weight) + (right*n_weight) + 
							(down*n_weight) + (up*n_weight) + 
							(back*n_weight) + (front*n_weight));
					//	if(diff_val==0) cout<<"diff_val="<<diff_val<<", x="<<x<<", y="<<y<<", z="<<z<<endl;
				m3D_edgePoints.set(x,y,z,diff_val);
			
	}
	cout<<"Matrix3D m3D_edgePoints Size: ["<<m3D_edgePoints.getWidth();
	cout<<", "<<m3D_edgePoints.getHeight()<<", "<<m3D_edgePoints.getSlice()<<"]"<<endl;
	/*
	cout<<"-----m3D_data (z-1)-------"<<endl;
	m3D_dataPoints.print(97, 117, 75, 95, 82, 83);
	cout<<"-----m3D_data (z)-------"<<endl;
	m3D_dataPoints.print(97, 117, 75, 95, 83, 84);
	cout<<"-----m3D_data (z+1)-------"<<endl;
	m3D_dataPoints.print(97, 117, 75, 95, 84, 85);
	cout<<"-----m3D_edge-------"<<endl;
	m3D_edgePoints.print(97, 117, 75, 95, 83, 84);
 
	*/
}