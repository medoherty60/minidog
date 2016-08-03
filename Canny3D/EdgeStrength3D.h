#ifndef EDGE_STRENGTH_H
#define EDGE_STRENGTH_H

#include "Canny3Dv2.h"
#include "Matrix3D.h"
#include "WriteFile.h"
#include "Vector.h"

class EdgeStrength3D{

public: EdgeStrength3D(){
	
	strengthX_min = 1000000.; strengthX_max = -100000.;
	strengthY_min = 1000000.; strengthY_max = -100000.;
	strengthZ_min = 1000000.; strengthZ_max = -100000.;
	
	minStrength=0; maxStrength=0;
	strengthCount=0;
}
bool isZeroCrossing(double& f, float p1, float p2){
	
	if(p2*p1<0) {
		f = p2/( p2-p1 ); 
		return true;
	}else {
		f=0.; 
		return false; 
	}
}
/*
void compute_SurfaceCrossingGradient(Vector& surfaceCrossing_gradient, double f, Vector g1, Vector g2){

	Vector scg(f*g1.x() + (1-f)*g2.x(),
			   f*g1.y() + (1-f)*g2.y(),
			   f*g1.z() + (1-f)*g2.z());

	surfaceCrossing_gradient = scg.normalize();
}*/
void compute_EdgeStrength_Dev3(float& edge_strength, double f, float p1, float p2){
	
	edge_strength = -1*( (f*p1) + ((1.-f)*p2) );
}
void estimate_density_range(float up_threshold, float image_range[], Matrix3D<float>* m3D_Data, 
							Matrix3D<float>* edge_strengthX, Matrix3D<float>* edge_strengthY, 
							Matrix3D<float>* edge_strengthZ){
	
	int corner[8][3] = {{0,0,0},{1,0,0},{1,1,0},{0,1,0},{0,0,1},{1,0,1},{1,1,1},{0,1,1}};
	float d = 0.0;
	float diff=0.;
	float small_diff, new_d_up, new_d_low;
	float global_new_d_low =10000, global_new_d_up=-10000;
	
	for(int z=0; z<slice; z++)
		for(int y=0; y<height; y++)
			for(int x=0; x<width; x++){
				
				if((edge_strengthX->get(x,y,z)!=9999 && (edge_strengthX->get(x,y,z)>up_threshold)) || 
				   (edge_strengthY->get(x,y,z)!=9999 && (edge_strengthY->get(x,y,z)>up_threshold)) || 
				   (edge_strengthZ->get(x,y,z)!=9999 && (edge_strengthZ->get(x,y,z)>up_threshold))){
					
					new_d_up = 10000.0;
					new_d_low = -10000.0;
					
					for(int c=0; c<8; c++){
						
						d = m3D_Data->get(x+corner[c][0],y+corner[c][1],z+corner[c][2]);
	
						//find the nearest density at both ends of the range
						if(d < image_range[LOW]){
							if(d>new_d_low){
								new_d_low = d;
							}
						}else if(d > image_range[HIGH]){
							if(d<new_d_up){
								new_d_up = d;
							}
						}
					}
					if((new_d_low > -10000.0) && (new_d_low<global_new_d_low))
						global_new_d_low = new_d_low;
					if((new_d_up < 10000.0) && (new_d_up>global_new_d_up))
						global_new_d_up = new_d_up;
				}
	}
	printf("Estimate Density Range (above upper Threshold): [%0.2f, %0.2f]\n", global_new_d_low, global_new_d_up);
}
int getMinStrength() { return minStrength; }
int getMaxStrength() { return maxStrength;}

void ComputeEdgeStrengths(Matrix3D<float>& edge_strengthX, Matrix3D<float>& edge_strengthY,
						  Matrix3D<float>& edge_strengthZ,  Matrix3D<Vector>& surfaceCrossing_gradient,
						  Matrix3D<Vector>* gradient, Matrix3D<float>* Lvv, Matrix3D<float>* Lvvv, int dim3D[]){
		
		width = dim3D[_X_];  
		height= dim3D[_Y_];
		slice = dim3D[_Z_];
	
		double f=0.0, strength=0.0;
		int tx_p=0, ty_p=0, tz_p=0;
		float strengthX=0.0, strengthY=0.0, strengthZ=0.0;
		Vector surfaceCrossing_gradientVector(0.,0.,0.);
		
		edge_strengthX.resize(width, height, slice);
		edge_strengthY.resize(width, height, slice);
		edge_strengthZ.resize(width, height, slice);
		
		edge_strengthX.setAll(-9999);
		edge_strengthY.setAll(-9999);
		edge_strengthZ.setAll(-9999);
		
		Vector maxXpos, maxYpos, maxZpos;
		float mX, mY, mZ;

		for(int z=0; z<slice; z++)
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++){
					
					tx_p=x+1; ty_p=y+1; tz_p=z+1;   
					
					clampBorderIdx(tx_p, width);
                    clampBorderIdx(ty_p, height);
                    clampBorderIdx(tz_p, slice);
					
					//process x
					if(isZeroCrossing(f, Lvv->get(x,y,z), Lvv->get(tx_p,y,z))) {

						compute_EdgeStrength_Dev3(strengthX, f, Lvvv->get(x,y,z), Lvvv->get(tx_p,y,z));

						if(strengthX>0) {
							update_MinMax_Strength(strengthX, _X_);
							edge_strengthX.set(x,y,z,strengthX);
							strengthCount++;

							//compute_SurfaceCrossingGradient(surfaceCrossing_gradientVector, f,
							//						        gradient->get(x,y,z), gradient->get(tx_p,y,z));
							//surfaceCrossing_gradient.set(x,y,z, surfaceCrossing_gradientVector);
						}
					}
					//process y
					if(isZeroCrossing(f, Lvv->get(x,y,z), Lvv->get(x,ty_p,z))){
						
						compute_EdgeStrength_Dev3(strengthY, f, Lvvv->get(x,y,z), Lvvv->get(x,ty_p,z));

						if(strengthY>0){
							update_MinMax_Strength(strengthY, _Y_);
							edge_strengthY.set(x,y,z,strengthY);
							strengthCount++;

							//compute_SurfaceCrossingGradient(surfaceCrossing_gradientVector, f,
							//						        gradient->get(x,y,z), gradient->get(x,ty_p,z));
							//surfaceCrossing_gradient.set(x,y,z, surfaceCrossing_gradientVector);
						}
					}
						//process z
					if(isZeroCrossing(f, Lvv->get(x,y,z), Lvv->get(x,y,tz_p))){
						
						compute_EdgeStrength_Dev3(strengthZ, f, Lvvv->get(x,y,z), Lvvv->get(x,y,tz_p));
						if(strengthZ>0){
							update_MinMax_Strength(strengthZ, _Z_);
						 	edge_strengthZ.set(x,y,z,strengthZ);
							strengthCount++;

							//compute_SurfaceCrossingGradient(surfaceCrossing_gradientVector, f,
							//								gradient->get(x,y,z), gradient->get(x,y,tz_p));
							//surfaceCrossing_gradient.set(x,y,z, surfaceCrossing_gradientVector);
						}
					}
				}
		float t1=0., t2=0.;
		float MaxVal=0., MinVal=0.;
		findLargest(t1, strengthX_max, strengthY_max);
		findLargest(MaxVal, t1, strengthZ_max);
	
		findSmallest(t2, strengthX_min, strengthY_min);
		findSmallest(MinVal, t2, strengthZ_min);
	
		minStrength = round(MinVal);
		maxStrength = round(MaxVal);
	
		cout <<"------------------------------"<<endl; 
		cout<<"strengthX[min, max]=["<<strengthX_min<<", "<<strengthX_max<<"]"<<endl;
		cout<<"strengthY[min, max]=["<<strengthY_min<<", "<<strengthY_max<<"]"<<endl;
		cout<<"strengthZ[min, max]=["<<strengthZ_min<<", "<<strengthZ_max<<"]"<<endl<<endl;
		cout<<"strength range in histogram: [min, max] = ["<< minStrength<<", "<< maxStrength<<"]"<<endl;
}

void writeStrength_And_MagnitudeFiles(string _fname, string _txt, 
									  Matrix3D<float>* edge_strengthX, 
									  Matrix3D<float>* edge_strengthY, 
									  Matrix3D<float>* edge_strengthZ){
		
		
		int range=0;
		
		range = maxStrength-minStrength+1;
    
		vector<int> strength_index(range);
		vector<int> strengthX_count(range);
		vector<int> strengthY_count(range);
		vector<int> strengthZ_count(range);
		
		
		for(int i=0, j=minStrength; i<range; i++, j++){
			strength_index[i] = j;
		}
	
		int bucketX=0, bucketY=0, bucketZ=0;
		double mag=0.0;
		Matrix3D<float> m3D_magnitude;
		m3D_magnitude.resize(width, height, slice);
		
		for(int z=0; z<slice; z++)
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++){
					
					bucketX = round(edge_strengthX->get(x,y,z))-minStrength;
					bucketY = round(edge_strengthY->get(x,y,z))-minStrength;
					bucketZ = round(edge_strengthZ->get(x,y,z))-minStrength;
					
					if (bucketX < 0) bucketX = 0;
					if (bucketX > range) bucketX = range;
					
					if (bucketY < 0) bucketY = 0;
					if (bucketY > range) bucketY = range;
					
					if (bucketZ < 0) bucketZ = 0;
					if (bucketZ > range) bucketZ = range;
					
					strengthX_count[bucketX]++;
					strengthY_count[bucketY]++;
					strengthZ_count[bucketZ]++;
					
					Vector m(edge_strengthX->get(x,y,z), 
							 edge_strengthY->get(x,y,z), 
							 edge_strengthZ->get(x,y,z));
					
					m3D_magnitude.set(x,y,z, m.length());
				}
		
		string fname = _fname+_txt;
		WriteFile wx(fname.c_str(),"csv");
		wx.writeHistogram(strength_index, strengthX_count, strengthY_count, strengthZ_count);
		
		string _fname1 = _fname + "magnitude";
		WriteFile wm(_fname1,"raw");
		wm.writeFloat(m3D_magnitude, width, height, slice);
		
		string _fname2 = _fname + "strengthX";
		WriteFile wsX(_fname2,"raw");
	    wsX.writeFloat(*edge_strengthX, width, height, slice);
	
		string _fname3 = _fname + "strengthY";
		WriteFile wsY(_fname3,"raw");
		wsY.writeFloat(*edge_strengthY, width, height, slice);
	
		string _fname4 = _fname + "strengthZ";
		WriteFile wsZ(_fname4,"raw");
		wsZ.writeFloat(*edge_strengthZ, width, height, slice);
}
void update_MinMax_Strength(float s, int dim){
	
	switch(dim){
		case _X_:		
			if (s > strengthX_max) strengthX_max = s; 
			if (s < strengthX_min) strengthX_min = s; break;
		case _Y_:
			if (s > strengthY_max) strengthY_max = s; 
			if (s < strengthY_min) strengthY_min = s; break;
		case _Z_:
			if (s > strengthZ_max) strengthZ_max = s; 
			if (s < strengthZ_min) strengthZ_min = s; break;
	}
}
void clampBorderIdx(int& idx, int maxVal){
		
	int min = 0;
	int max = maxVal;
		
	if(idx<min) idx=min;
	else if(idx>=max) idx = maxVal-1;
		
}
void findLargest(float& MaxVal, float v1, float v2){
	if(v1 > v2) MaxVal = v1;
	else MaxVal = v2;
}

void findSmallest(float& MinVal, float v1, float v2){
	if(v1 > v2) MinVal = v2;
	else MinVal = v1;
}
int getStrengthCount(){ return strengthCount; }
	
private:
	float strengthX_max, strengthX_min;
	float strengthY_max, strengthY_min;
	float strengthZ_max, strengthZ_min;
	int minStrength, maxStrength;
	int strengthCount;
	
	unsigned int width, height, slice;
	
	
};
#endif

