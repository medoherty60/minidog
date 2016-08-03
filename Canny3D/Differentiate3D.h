#ifndef	DIFFERENTIATE3D_H
#define DIFFERENTIATE3D_H

#include "Vector.h"
#include "WriteFile.h"

using namespace std;

class Differentiate3D{

struct Dev_3rd_t{
	float Lxxx, Lyyy, Lzzz, Lxyy, Lxyz, Lxzz, Lxxy, Lyzz, Lxxz, Lyyz;
};
	 
struct Dev_2nd_t{
float Lxx, Lyy, Lzz, Lxy, Lyz, Lxz;
};
	
public: Differentiate3D(){

	grand_total_edges=0;
	unused_edges=0;
	countZC=0;
}

void computeDerivatives(Matrix3D<float>* m3D_rData, Matrix3D<Vector>* gDir, Matrix3D<float>* Lvv, Matrix3D<float>* Lvvv){
		
		width = m3D_rData->getWidth();  
		height= m3D_rData->getHeight(); 
		slice = m3D_rData->getSlice();
		
		Vector g;
		Dev_2nd_t Dev_2nd; 
		Matrix3D<Dev_2nd_t> g2Dev;			//2nd derivatives (Lxx,Lyy,Lzz,Lxy,Lyz,Lxz)
		
		gDir->resize(width, height, slice);
		g2Dev.resize(width, height, slice);
		
		float cVal;
		int tx_p=0, ty_p=0, tz_p=0;
		int tx_n=0, ty_n=0, tz_n=0;
		
		for(int z=0; z<slice; z++)
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++){
					
					//::check coefficient 1ndex within data domain
					// use border values for coefficient indices beyond the data border
					
					tx_p=x+1; ty_p=y+1; tz_p=z+1;
					tx_n=x-1; ty_n=y-1; tz_n=z-1; 
					
					if(tx_n<0 || ty_n<0 || tz_n<0 || tx_p>width-1 || ty_p>height-1 || tz_p>slice-1){
						clampBorderIdx(tx_p, width);
                        clampBorderIdx(ty_p, height);
                        clampBorderIdx(tz_p, slice);
                        
						clampBorderIdx(tx_n, width);
                        clampBorderIdx(ty_n, height);
                        clampBorderIdx(tz_n, slice);
					}
					
					//1st derivatives
					g = Vector((m3D_rData->get(tx_p,y,z) - m3D_rData->get(tx_n,y,z))/2, 
							   (m3D_rData->get(x,ty_p,z) - m3D_rData->get(x,ty_n,z))/2,
							   (m3D_rData->get(x,y,tz_p) - m3D_rData->get(x,y,tz_n))/2);
					g.normalize();
					
					//2nd derivatives
					Dev_2nd.Lxx = m3D_rData->get(tx_p,y,z) + m3D_rData->get(tx_n,y,z) - 2.* m3D_rData->get(x,y,z);
					Dev_2nd.Lyy = m3D_rData->get(x,ty_p,z) + m3D_rData->get(x,ty_n,z) - 2.* m3D_rData->get(x,y,z),
					Dev_2nd.Lzz = m3D_rData->get(x,y,tz_p) + m3D_rData->get(x,y,tz_n) - 2.* m3D_rData->get(x,y,z);
					
					Dev_2nd.Lxy = (m3D_rData->get(tx_p,ty_p,z) - m3D_rData->get(tx_p,ty_n,z) - 
								   m3D_rData->get(tx_n,ty_p,z) + m3D_rData->get(tx_n,ty_n,z))/4;
					Dev_2nd.Lyz = (m3D_rData->get(x,ty_p,tz_p) - m3D_rData->get(x,ty_p,tz_n) - 
								   m3D_rData->get(x,ty_n,tz_p) + m3D_rData->get(x,ty_n,tz_n))/4;
					Dev_2nd.Lxz = (m3D_rData->get(tx_p,y,tz_p) - m3D_rData->get(tx_p,y,tz_n) -
								   m3D_rData->get(tx_n,y,tz_p) + m3D_rData->get(tx_n,y,tz_n))/4;
					
					//Lxy = ((++*)-(+-*)-(-+*)+(--*))/4
					//Lyz = ((*++)-(*+-)-(*-+)+(*--))/4
					//Lxz = ((+*+)-(+*-)-(-*+)+(-*-))/4
					
					gDir->set(x,y,z,g);
					g2Dev.set(x,y,z,Dev_2nd);
				}	
		compute_HighOrder_DirDev(gDir, g2Dev, Lvv, Lvvv);
}

void compute_HighOrder_DirDev(Matrix3D<Vector>* gDir, Matrix3D<Dev_2nd_t>& g2Dev, Matrix3D<float>* Lvv, Matrix3D<float>* Lvvv){
		
		Dev_3rd_t Dev_3rd; 
	//	Dev_5th Dev_3rd;
		float gX=0., gY=0., gZ=0.;
		float Lvv_val=0., Lvvv_val=0.;
		int tx_p=0, ty_p=0, tz_p=0;
		int tx_n=0, ty_n=0, tz_n=0;
		float vvmax = -1000000., vvmin = 100000.;
		float vvvmax = -1000000., vvvmin = 100000.;
	
		Lvv->resize(width, height, slice);
		Lvvv->resize(width, height, slice);
		
		for(int z=0; z<slice; z++)
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++){
					
						//::check coefficient index within data domain
						// use border values for coefficient indices beyond the data border
					tx_p=x+1; ty_p=y+1; tz_p=z+1;
					tx_n=x-1; ty_n=y-1; tz_n=z-1;   
					
					if(tx_n<0 || ty_n<0 || tz_n<0 || tx_p>width-1 || ty_p>height-1 || tz_p>slice-1){
						clampBorderIdx(tx_p, width); clampBorderIdx(ty_p, height); clampBorderIdx(tz_p, slice);
						clampBorderIdx(tx_n, width); clampBorderIdx(ty_n, height); clampBorderIdx(tz_n, slice);
					}
						//process x
					Dev_3rd.Lxxx = (g2Dev.get(tx_p,y,z).Lxx - g2Dev.get(tx_n,y,z).Lxx)/2;
					Dev_3rd.Lxyy = (g2Dev.get(tx_p,y,z).Lyy - g2Dev.get(tx_n,y,z).Lyy)/2;
					Dev_3rd.Lxyz = (g2Dev.get(tx_p,y,z).Lyz - g2Dev.get(tx_n,y,z).Lyz)/2;
					Dev_3rd.Lxzz = (g2Dev.get(tx_p,y,z).Lzz - g2Dev.get(tx_n,y,z).Lzz)/2;
					
						//process y
					Dev_3rd.Lyyy = (g2Dev.get(x,ty_p,z).Lyy - g2Dev.get(x,ty_n,z).Lyy)/2;
					Dev_3rd.Lxxy = (g2Dev.get(x,ty_p,z).Lxx - g2Dev.get(x,ty_n,z).Lxx)/2;
					Dev_3rd.Lyzz = (g2Dev.get(x,ty_p,z).Lzz - g2Dev.get(x,ty_n,z).Lzz)/2;
					
						//process z
					Dev_3rd.Lzzz = (g2Dev.get(x,y,tz_p).Lzz - g2Dev.get(x,y,tz_n).Lzz)/2;
					Dev_3rd.Lxxz = (g2Dev.get(x,y,tz_p).Lxx - g2Dev.get(x,y,tz_n).Lxx)/2;
					Dev_3rd.Lyyz = (g2Dev.get(x,y,tz_p).Lyy - g2Dev.get(x,y,tz_n).Lyy)/2;
					


					gX = gDir->get(x,y,z).x();
					gY = gDir->get(x,y,z).y();
					gZ = gDir->get(x,y,z).z();
					
					//2nd directional derivatives
					Lvv_val = gX * gX * g2Dev.get(x,y,z).Lxx +
							  gY * gY * g2Dev.get(x,y,z).Lyy + 
							  gZ * gZ * g2Dev.get(x,y,z).Lzz + 
						      2.* gX * gY * g2Dev.get(x,y,z).Lxy + 
					          2.* gX * gZ * g2Dev.get(x,y,z).Lxz + 
							  2.* gY * gZ * g2Dev.get(x,y,z).Lyz;
					
					//3rd directional derivatives
					Lvvv_val = gX * gX * gX * Dev_3rd.Lxxx + 
							   gY * gY * gY * Dev_3rd.Lyyy + 
							   gZ * gZ * gZ * Dev_3rd.Lzzz + 
							   6.* gX * gY * gZ * Dev_3rd.Lxyz + 
							   3.* gX * gX * gY * Dev_3rd.Lxxy + 
							   3.* gX * gY * gY * Dev_3rd.Lxyy + 
							   3.* gX * gX * gZ * Dev_3rd.Lxxz + 
							   3.* gX * gZ * gZ * Dev_3rd.Lxzz + 
						       3.* gY * gY * gZ * Dev_3rd.Lyyz + 
							   3.* gY * gZ * gZ * Dev_3rd.Lyzz; 
					
					if (Lvvv_val > vvvmax) vvvmax = Lvvv_val;
					if (Lvvv_val < vvvmin) vvvmin = Lvvv_val;
					
					if (Lvv_val > vvmax) vvmax = Lvv_val;
					if (Lvv_val < vvmin) vvmin = Lvv_val;
					
					Lvv->set(x,y,z, Lvv_val);
					Lvvv->set(x,y,z, Lvvv_val);
				}
		cout<<endl;
		cout<<"Lvv  [min, max] = ["<<vvmin<<", "<<vvmax<<"]"<<endl;
		cout<<"Lvvv [min, max] = ["<<vvvmin<<", "<<vvvmax<<"]"<<endl;
	
}
void clampBorderIdx(int& idx, int maxVal){
		
		int min = 0;
		int max = maxVal;
		
		if(idx<min) idx=min;
		else if(idx>=max) idx = maxVal-1;
		
}
void writeToDevFile(string _fname, Matrix3D<float>* m3D_rData,  Matrix3D<float>* Lvv, string file_extension, char _w){
		
		float initalVal = 0;
		//float initalVal = -1424;
		
		if(_w){
			Matrix3D<float> Lvv_filtered_Data;
			Lvv_filtered_Data.resize(width, height, slice);
			Lvv_filtered_Data.setAll(initalVal);
			filterLvv(Lvv_filtered_Data, m3D_rData, Lvv);
			
			WriteFile w(_fname, file_extension.c_str());
			w.writeFloat(Lvv_filtered_Data, width, height, slice);
		}
}
//all zero-crossing edges
void writeLvv_File(string _fname, Matrix3D<float>& Lvv, string file_extension, char _w){
		
		if(_w){			
			WriteFile w(_fname, file_extension.c_str());
			w.writeFloat(Lvv, width, height, slice);
		}
}
void filterLvv(Matrix3D<float>& data, Matrix3D<float>* m3D_rData, Matrix3D<float>* Lvv){
		
		float v = 0., vx =0., vy=0., vz=0.;
		int total_edges=0, count_nonZC=0;
 	    int totalCount=0;
	
		float minsd = 10000.0;
		float maxsd = -10000.0;
		int count_sd=0;
		
		int _x=76;
		int _y=28;
		int _z=4;
	
		for(int z=0; z<slice-1; z++)
			for(int y=0; y<height-1; y++)
				for(int x=0; x<width-1; x++){
					
					//zero-crossing test
					vx = Lvv->get(x,y,z) * Lvv->get(x+1,y,z);
					vy = Lvv->get(x,y,z) * Lvv->get(x,y+1,z);
					vz = Lvv->get(x,y,z) * Lvv->get(x,y,z+1);
					
					//zero-crossing (display with density values, initialVal = -1280)
					/*
					if(vx<0 || vy<0 || vz<0){
						if(x==_x && y==_y && z==_z){
							cout<<"------------------- isZeroCrossing (write Lvv) ---------------------------"<<endl;
							printf("[%d,%d,%d], 2nd_Dev=%f, density=%f\n", x,y,z, Lvv->get(x,y,z), m3D_rData->get(x,y,z));
							printf("[%d,%d,%d], 2nd_Dev=%f, density=%f\n", x+1,y,z, Lvv->get(x+1,y,z), m3D_rData->get(x+1,y,z));
						}
						data.set(x,y,z, m3D_rData->get(x,y,z));
						countZC++;
					}else count_nonZC++;
					*/
					//zero-crossing (initialVal=0 [zero-crossing set to white])
					if(vx<0 || vy<0 || vz<0){
						if(vx<0) { data.set(x,y,z, 255); countZC++;}
						if(vy<0) { data.set(x,y,z, 255); countZC++;}
						if(vz<0) { data.set(x,y,z, 255); countZC++;}
					}else
						count_nonZC++;
					
					/*
					 //zero-crossing (initialVal=0 [not zero-crossing set to white])
					 if(vx<0 || vy<0 || vz<0){
						
						 data.set(x,y,z, 1);
					 }*/
					/*
					if(vx<0 || vy<0 || vz<0){
						
						data.set(x,y,z, 1);
					}*/
					 /*
					//zero-crossing with density filter
					 if(vx<0 || vy<0 || vz<0){
						 if(m3D_rData->get(x,y,z)>-250 && m3D_rData->get(x,y,z)<200)
						 data.set(x,y,z, m3D_rData->get(x,y,z));
					 }*/
					
					if(Lvv->get(x,y,z)>-.5 && Lvv->get(x,y,z)<.5){
						if(Lvv->get(x,y,z)<minsd) minsd = Lvv->get(x,y,z);
						if(Lvv->get(x,y,z)>maxsd) maxsd = Lvv->get(x,y,z);
						count_sd++;
					}
					
				}
		grand_total_edges = (width*(height+1)*(slice+1))+
				            ((width+1)*height*(slice+1))+
							((width+1)*(height+1)*slice);

		unused_edges = ((width*(slice+1)+slice*(width+1)) +
					   (height*(slice+1)+slice*(height+1)) +
					   (width*(height+1)+height*(width+1)))*2 -
					   ((width+height+slice)*4);

		total_edges = grand_total_edges-unused_edges;
	
		//original volume size: (213*170*168)
		//grand total edges: (213*171*169)+(214*170*169)+(214*171*168)
		cout<<"Total edges= "<<total_edges<<endl;
		cout<<"zero-crossing edges = "<<countZC<<endl;
		cout<<"Non zero-crossing edges="<<count_nonZC<<endl;
		cout<<"total edges = "<<countZC + count_nonZC<<endl;
		cout<<"percentage of zero-crossing ="<<(float)(countZC/(float)(total_edges))*100<<"%"<<endl;
		//cout<<"second_derivatives: [min, max] = ["<<minsd<<", "<<maxsd<<"] "<<endl;
		//cout<<"second_derivatives count (between -0.5 and 0.5)="<<count_sd<<endl;
}
int getGrandTotalEdges() { return grand_total_edges; }
int getUnusedEdges() { return unused_edges; }
int getZeroCrossingCount() {return countZC; }
	
private:
	unsigned int width, height, slice;
	int grand_total_edges, unused_edges, countZC;
	
};
#endif 

