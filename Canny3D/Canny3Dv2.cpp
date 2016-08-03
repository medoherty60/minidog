#include "Canny3Dv2.h"

#ifdef _MSC_VER // using Visual Studio
long round(float x)
{
	long tmp = int(x);
	if ((x-tmp)>=0.5f) tmp=tmp+1;
	return tmp;
}
#endif
/*
int Canny3Dv2::adjacent_increment[3][3][3]
= { { {0, -1, 0}, {0, -1, -1}, {0, 0, -1} },    //process x     (anti-clockwise)
	{ {-1, 0, 0}, {-1, 0, -1}, {0, 0, -1} },    //process y
	{ {-1, 0, 0}, {-1, -1, 0}, {0, -1, 0} } };  //process z
*/
//Canny3D_MARINA 
int Canny3Dv2::four_adjacent_increment[3][4][3]
= { { {0, 0, 0}, {0, -1, 0}, {0, -1, -1}, {0, 0, -1} },    //process x     (anti-clockwise)
	{ {0, 0, 0}, {-1, 0, 0}, {-1, 0, -1}, {0, 0, -1} },    //process y
	{ {0, 0, 0}, {-1, 0, 0}, {-1, -1, 0}, {0, -1, 0} } };  //process z 
/*
//Canny3D_MARINA 
int Canny3Dv2::eight_edge_neighbor_increment[3][8][3]
= { {{0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1},   //0	  //constant x-plane
	{0, -1, -1},{0, 1, 1}, {0, 1, -1}, {0, -1,1}}, 
	{{-1, 0, 0}, {1, 0, 0}, {0, 0, -1}, {0, 0, 1},   //1      //constant y-plane		
	{-1, 0, -1},{1, 0, 1}, {1, 0, -1}, {-1, 0,1}},  
	{{-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0},   //2	  //constant z-plane
	{-1, -1, 0},{1, 1, 0}, {1, -1, 0}, {-1, 1, 0}} }; 
*/
/*Canny3D_MARINA (Upper_threshold_filter)
 int Canny3Dv2::three_adjacent_increment[3][3][3]
 = { { {0, -1, 0}, {0, -1, -1}, {0, 0, -1} },    //process x     (anti-clockwise)
 { {-1, 0, 0}, {-1, 0, -1}, {0, 0, -1} },    //process y
 { {-1, 0, 0}, {-1, -1, 0}, {0, -1, 0} } };  //process z 
 */

void Canny3Dv2::filterCubes(Matrix3D<char>& data){
	
	int white = 255;
	int grey = 160;
	int light_grey =80; 
	int black = 0;
	
	float min_value = 10000, max_value =-10000;
	int count_higher =0, count_lower=0, count_remaining=0;
	
	float min_value2 = 10000, max_value2 =-10000;
	int count_higher2 =0, count_lower2=0, count_remaining2=0;
	
	float min_value3 = 10000, max_value3 =-10000;
	int count_higher3 =0, count_lower3=0, count_remaining3=0;
	
	for(int z=0; z<slice; z++)
		for(int y=0; y<height; y++)
			for(int x=0; x<width; x++){
				
				if(cubes.get(x,y,z)==1){	
					data.set(x,y,z, white);
					
					if(m3D_Data->get(x,y,z) < min_value) min_value = m3D_Data->get(x,y,z);
					if(m3D_Data->get(x,y,z) > max_value) max_value = m3D_Data->get(x,y,z);
					
					if(m3D_Data->get(x,y,z) >= ts->get_density(HIGH)) count_higher++;
					if(m3D_Data->get(x,y,z) < ts->get_density(LOW)) count_lower++;
					if(m3D_Data->get(x,y,z) > ts->get_density(LOW) && m3D_Data->get(x,y,z) <= ts->get_density(HIGH)) count_remaining++;
					
				}
				else if(cubes.get(x,y,z)==2){
					data.set(x,y,z, grey);
					if(m3D_Data->get(x,y,z) < min_value2) min_value2 = m3D_Data->get(x,y,z);
					if(m3D_Data->get(x,y,z) > max_value2) max_value2 = m3D_Data->get(x,y,z);
					
					if(m3D_Data->get(x,y,z) >= ts->get_density(HIGH)) count_higher2++;
					if(m3D_Data->get(x,y,z) < ts->get_density(LOW)) count_lower2++;
					if(m3D_Data->get(x,y,z) > ts->get_density(LOW) && m3D_Data->get(x,y,z) < ts->get_density(HIGH)) count_remaining2++;
				}
				else if(cubes.get(x,y,z)>=3){
					data.set(x,y,z, light_grey);
					if(m3D_Data->get(x,y,z) < min_value3) min_value3 = m3D_Data->get(x,y,z);
					if(m3D_Data->get(x,y,z) > max_value3) max_value3 = m3D_Data->get(x,y,z);
					
					if(m3D_Data->get(x,y,z) >= ts->get_density(HIGH)) count_higher3++;
					if(m3D_Data->get(x,y,z) < ts->get_density(LOW)) count_lower3++;
					if(m3D_Data->get(x,y,z) > ts->get_density(LOW) && m3D_Data->get(x,y,z) < ts->get_density(HIGH)) count_remaining3++;
				}
			}

	cout<<"Cubes (mark as 1) [>=image_high,<image_low, remaining] = ["<<count_higher<<", "<<count_lower<<", "<<count_remaining<<"]"<<endl;
	cout<<"Cubes (mark as 1) [min,max] density values = ["<<min_value<<", "<<max_value<<"]"<<endl;
	cout<<"Cubes (mark as 2) [>=image_high,<image_low, remaining] = ["<<count_higher2<<", "<<count_lower2<<", "<<count_remaining2<<"]"<<endl;
	cout<<"Cubes (mark as 2) [min,max] density values = ["<<min_value2<<", "<<max_value2<<"]"<<endl;
	cout<<"Cubes (mark as 3) [>=image_high,<image_low, remaining] = ["<<count_higher3<<", "<<count_lower3<<", "<<count_remaining3<<"]"<<endl;
	cout<<"Cubes (mark as 3) [min,max] density values = ["<<min_value3<<", "<<max_value3<<"]"<<endl;
}
/*
void Canny3Dv2::writeToStrengthFiles(){
	
	WriteFile wx("strengthX");
	wx.writeFloat(edge_strengthX, width, height, slice);
	
	WriteFile wy("strengthY");
	wy.writeFloat(edge_strengthY, width, height, slice);
	
	WriteFile wz("strengthZ");
	wz.writeFloat(edge_strengthZ, width, height, slice);
	
}*/

void Canny3Dv2::writeToCubesFile(string _fname, string file_extension, char _w){
	
	if(_w){
		Matrix3D<char> Cubes_filtered_Data;
		Cubes_filtered_Data.resize(width, height, slice);
		filterCubes(Cubes_filtered_Data);
		
		WriteFile w(_fname, file_extension.c_str());
		w.writeCubes(Cubes_filtered_Data, width, height, slice);
	}
}
void Canny3Dv2::writeToCubesStatisticFile(string _fname, string file_extension, float upperThreshold, char _w){
	
	int count0=0, count1=0, count2=0, count3=0, countn=0;
	
	if(_w){
		for(int z=0; z<slice; z++)
			for(int y=0; y<height; y++)
				for(int x=0; x<width; x++){
					
					switch (cubes.get(x,y,z)){
						case 0: count0++; break;
						case 1: count1++; break;
						case 2: count2++; break;
						case 3: count3++; break;
						default: countn++;
					}
		}

		WriteFile w(_fname, file_extension.c_str());
		w.writeCubesStats(upperThreshold, count1, count2);
	}
}

void Canny3Dv2::print(Matrix3D<char>& markers, string text){
	
	int count = 0;
	for(int z=0; z<slice; z++)
		for(int y=0; y<height; y++)
			for(int x=0; x<width; x++){
				if( markers.get(x,y,z) ) 
					count++;
			}
	cout<<text<< " = "<<count<<endl;			   
}
void Canny3Dv2::print(){
	
	float mincubes = 10000.0;
	float maxcubes = -10000.0;
	
	cube_count0=0; cube_count1=0; cube_count2=0; 
	int cube_countn=0;
	count_cubes=0;

	for(int z=0; z<slice; z++)
		for(int y=0; y<height; y++)
			for(int x=0; x<width; x++){
				
				if(cubes.get(x,y,z) == 0) cube_count0++;
				if(cubes.get(x,y,z) == 1) cube_count1++;
				if(cubes.get(x,y,z) >= 2) cube_count2++;

				if(cubes.get(x,y,z)<mincubes) mincubes = cubes.get(x,y,z);
				if(cubes.get(x,y,z)>maxcubes) maxcubes = cubes.get(x,y,z);
			}

	count_cubes = cube_count1+cube_count2;
	cout<<"---------------------- "<<endl;
	cout<<"cubes: [min, max] = ["<<mincubes<<", "<<maxcubes<<"]"<<endl;
	cout<<"cubes: count [1,2,0]=["<<cube_count1<<", "<<cube_count2<<", "<<cube_count0<<"]"<<endl;
	cout<<"total edge_mark = "<<edgeMarkCount<<endl;
	cout<<"total cube_mark = "<<cubeMarkCount<<endl;
	cout<<"total cubes (final)="<<count_cubes<<endl;
	cout<<"selected cubes percentage="<<((float)count_cubes/(float)(width*height*slice))*100<<endl<<endl;
	
}
void Canny3Dv2::print_relax(){
	
	float mincubes = 10000.0;
	float maxcubes = -10000.0;
	
	cube_count0=0; cube_count1=0; cube_count2=0, cube_count3=0; 
	count_cubes=0;
	
	for(int z=0; z<slice; z++)
		for(int y=0; y<height; y++)
			for(int x=0; x<width; x++){
				
				if(cubes.get(x,y,z) == 0) cube_count0++;
				if(cubes.get(x,y,z) == 1) cube_count1++;
				if(cubes.get(x,y,z) == 2) cube_count2++;
				if(cubes.get(x,y,z) >= 3) cube_count3++;

				if(cubes.get(x,y,z)<mincubes) mincubes = cubes.get(x,y,z);
				if(cubes.get(x,y,z)>maxcubes) maxcubes = cubes.get(x,y,z);
			}
	count_cubes = cube_count1+cube_count2+cube_count3;

	cout<<"---------------------- "<<endl;
	cout<<"cubes: [min, max] = ["<<mincubes<<", "<<maxcubes<<"]"<<endl;
	cout<<"cubes: count [1,2,3,0]=["<<cube_count1<<", "<<cube_count2<<", "<<cube_count3<<", "<<cube_count0<<"]"<<endl;
	cout<<"total edge_mark = "<<edgeMarkCount<<endl;
	cout<<"total cube_mark = "<<cubeMarkCount<<endl;
	cout<<"total cubes (final)="<<count_cubes<<endl;
	cout<<"selected cubes percentage="<<((float)count_cubes/(float)(width*height*slice))*100<<endl<<endl;
	
}





