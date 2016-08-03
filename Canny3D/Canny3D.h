#ifndef CANNY3D_NEW_H
#define CANNY3D_NEW_H
#include "Canny3Dabstract.h"
#include "Matrix3D.h"
#include "Threshold.h"
#include "Vector.h"
#include "WriteFile.h"
#include "Point.h"
#include <utility>
using namespace std;

struct edgeNeighbors_t{
	int original_seeds[3];
	int seeds[3];
	int neighbors[3][3];
};

class Canny3D : public Canny3Dabstract {
	
public: Canny3D(bool _t, int _dim3D[], Matrix3D<float>* _m3D_Data, Matrix3D<Vector>* _scg,
				Matrix3D<float>* _esX, Matrix3D<float>* _esY,  Matrix3D<float>* _esZ, Threshold* _ts){
	
	no_density_test = _t;
	pass_edge_mark_test=0;
	not_marked_test = 0;
	isMarked = 0;
	edgeMarkCount=0;
	cubeMarkCount=0;
	count_cubes=0;
	cube_count0=0; 
	cube_count1=0; 
	cube_count2=0;
	cube_count3=0;
	pass=1;
	
	width = _dim3D[_X_];  
	height= _dim3D[_Y_];  
	slice = _dim3D[_Z_];  
	
	m3D_Data = _m3D_Data;
	edge_strengthX = _esX;
	edge_strengthY = _esY;  
	edge_strengthZ = _esZ;
	surfaceCrossing_gradient = _scg;
    
    ts = _ts;
	
	//printf("x=%d, y=%d, z=%d", width, height, slice);
    ts->print_curr_edgeStrength();
    ts->print_density();
    ts->print_relax_density();
}
	virtual ~Canny3D(){};
	void print(Matrix3D<char>& markers, string text);
	void print();
	void print_relax();
	void writeToCubesFile(string _fname, string file_extension, char _w);
	void writeToCubesStatisticFile(string _fname, string file_extension, float upperThreshold, char _w);
	void filterCubes(Matrix3D<char>& data);
	
void clampBorderIdx(int& idx, int maxVal){
	
	int min = 0;
	int max = maxVal;
	
	if(idx<min) idx=min;
	else if(idx>=max) idx = maxVal-1;
	
}
void Find_Initial_Seed_EdgeCubes(){
	
	Point p;
	float sX=0.0, sY=0.0, sZ=0.0;
	cubes.resize(width, height, slice);
	edge_mark.resize(width, height, slice);
	
	for(int z=0; z<slice; z++)
		for(int y=0; y<height; y++)
			for(int x=0; x<width; x++){
				
				p = Point(x,y,z);
				
				sX = edge_strengthX->get(x,y,z);
				sY = edge_strengthY->get(x,y,z);
				sZ = edge_strengthZ->get(x,y,z);
				
				//process x
				if(sX != -9999){  markEdgePoints_lowThresholdFilter(p, sX, 1);
								  markSeeds_highThresholdFilter_MARINANEW(p, Point(x+1,y,z), sX, _X_); }
				 //process y
				if(sY != -9999){  markEdgePoints_lowThresholdFilter(p, sY, 2);
							      markSeeds_highThresholdFilter_MARINANEW(p, Point(x,y+1,z), sY, _Y_); }
				//process z
				if(sZ != -9999){  markEdgePoints_lowThresholdFilter(p, sZ, 4);
								  markSeeds_highThresholdFilter_MARINANEW(p, Point(x,y,z+1), sZ, _Z_); }
			}
}
void load_oddQueue(vector<Point>& odd_queue){
	
	Point p;
	for (int z = 0; z < slice; ++z)	
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x) 
				if ( cubes.get(x,y,z) ) {    
					
					p = Point(x,y,z);
					odd_queue.push_back(p);
	}
}
void Hysteresis(int& isRelax){
	
	Point p;
	
	vector<Point> odd_queue;
	vector<Point> even_queue;
	
	load_oddQueue(odd_queue);
	
	vector<Point>* current_queue = &odd_queue;
	vector<Point>* new_queue = &even_queue;
	
	int relax_flag = 2;
	int orig_flag = 1;
	int adj4_flag = 1;

	while (current_queue->size()>0) {
		
		for (int i = 0; i < current_queue->size(); ++i){  
			
				if(isRelax){
					switch(relax_flag){
						// 24 conn
						case 1: surfaceExpansion_Relax(new_queue,(*current_queue)[i],adj4_flag); break;
						case 2: surfaceExpansion_Relax2(new_queue,(*current_queue)[i],adj4_flag); break;
						case 3: surfaceExpansion_Relax3(new_queue,(*current_queue)[i], 20, adj4_flag); break;

						// MED 160706 - 6 way expansion
						case 4: surfaceExpansion_6way(new_queue, (*current_queue)[i], adj4_flag); break;
						default: break;
					}

				}else{
					switch(orig_flag){
						case 1: surfaceExpansion(new_queue, (*current_queue)[i], adj4_flag);  break;
						case 2: surfaceExpansion_6way(new_queue, (*current_queue)[i], adj4_flag); break;
						default: break;
					}
				}
		}
		if (pass%2 == 1) {						
			current_queue = &even_queue;
			new_queue = &odd_queue;
		}
		else {
			current_queue = &odd_queue;
			new_queue = &even_queue;
		}
		new_queue->clear();
		cout<<"pass: "<<pass<<endl;
		++pass;
	}
	cout<<"[Hysteresis] total number of passes="<<pass<<endl;
}

bool validSeed(Point p1, float curr_image_range[], int dir){
		
	bool valid = false;
	int p2x=p1.x(), p2y=p1.y(), p2z=p1.z();
	
	switch (dir) {  case _X_: { p2x++; break;}
					case _Y_: { p2y++; break;}
					case _Z_: { p2z++; break;} }
	
	Point p2(p2x,p2y,p2z);

	if(!no_density_test){
		if(((edge_mark.get(p1.x(),p1.y(),p1.z()) >> dir) & 1) &&
			(inDensityRange(p1, curr_image_range) || inDensityRange(p2, curr_image_range))){
			valid = true;
		}
	
	}else{
		if((edge_mark.get(p1.x(),p1.y(),p1.z()) >> dir) & 1){
			valid = true;
		}
	}
	return valid;
}
void surfaceExpansion_Relax(vector<Point>* new_queue, Point& p, int adj4_flag=0){
	
	int kx=0, ky=0, kz=0;
	int jx=0, jy=0, jz=0, j=0; 
	float curr_image_range[2]={0.0,0.0};
	
	curr_image_range[0] = ts->get_density(HIGH);
	curr_image_range[1] = ts->get_density(LOW);
	
	//New Code: Relaxation
	//p is the seed (isMarked); 
	if (cubes.get(p.x(), p.y(),p.z()) == 3)
    {
		curr_image_range[0] = ts->get_relax_density(HIGH);
		curr_image_range[1] = ts->get_relax_density(LOW);
	}
	
	for (int l = 0; l < 3; ++l) 
		for (int m = 0; m < 8; ++m) {  //extract eight neighbor edges one direction (l) at a time 
			
			//k is the 24 neighbors from p
			kx = p.x() + eight_edge_neighbor_increment[l][m][0];    
			ky = p.y() + eight_edge_neighbor_increment[l][m][1];
			kz = p.z() + eight_edge_neighbor_increment[l][m][2];
			
			// outside of data volume
			if (kx < 0 || kx >= width || ky < 0 || ky >= height || kz < 0 || kz >= slice ) continue; 
		
			//a valid seed: must satisfy the weak inclusion test and density range test
			for (int dir = 0; dir < 3; ++dir)    
					if(validSeed(Point(kx,ky,kz), curr_image_range, dir)){

					if(adj4_flag){
						for(int col=0; col<4; col++){//4 adjacency
							
							jx = kx + four_adjacent_increment[dir][col][0];
							jy = ky + four_adjacent_increment[dir][col][1];
							jz = kz + four_adjacent_increment[dir][col][2];

							if (jx >= 0 && jy >= 0 && jz >= 0 ){
								if ( cubes.get(jx, jy, jz) ) continue;
							
								//New Code:
								//1st condition: did not pass the tight inclusion test
								//2nd condition: (mark 2) they are 4-adjacency, mark no matter what
								if(!validSeed(Point(jx,jy,jz), ts->get_density_range(), dir))
									cubes.set(jx, jy, jz, 3); 
								else
									cubes.set(jx, jy, jz, 2);
									
								new_queue->push_back(Point(jx, jy, jz));
							}
						}
					}else{

						jx = kx; jy=ky; jz=kz;
						if (jx >= 0 && jy >= 0 && jz >= 0 ){
							if ( cubes.get(jx, jy, jz) ) continue;

							//New Code:
							//1st condition: did not pass the tight inclusion test
							//2nd condition: (mark 2) they are 4-adjacency, mark no matter what
							if(!validSeed(Point(jx,jy,jz), ts->get_density_range(), dir))
								cubes.set(jx, jy, jz, 3);
							else
								cubes.set(jx, jy, jz, 2);

							new_queue->push_back(Point(jx, jy, jz));
						}
					}
				}
		}
}
//use relaxed density range for surface expansion (use tight density range for initial seedings)
void surfaceExpansion_Relax2(vector<Point>* new_queue, Point& p, int adj4_flag=0){
	
	int kx=0, ky=0, kz=0;
	int jx=0, jy=0, jz=0, j=0; 
	
	for (int l = 0; l < 3; ++l) 
		for (int m = 0; m < 8; ++m) {  //extract eight neighbors one dimension(l) at a time 
			
			kx = p.x() + eight_edge_neighbor_increment[l][m][0];    
			ky = p.y() + eight_edge_neighbor_increment[l][m][1];
			kz = p.z() + eight_edge_neighbor_increment[l][m][2];
			
			// outside of data volume
			if (kx < 0 || kx >= width || ky < 0 || ky >= height || kz < 0 || kz >= slice ) continue; 
			
			//a valid seed: must satisfy the weak inclusion test and density range test
			for (int dir = 0; dir < 3; ++dir)    
				if(validSeed(Point(kx,ky,kz), ts->get_relax_density_range(), dir)){

					if(adj4_flag){
						for(int col=0; col<4; col++){
						
							jx = kx + four_adjacent_increment[dir][col][0];
							jy = ky + four_adjacent_increment[dir][col][1];
							jz = kz + four_adjacent_increment[dir][col][2];
						
							if (jx >= 0 && jy >= 0 && jz >= 0 ){
								if ( cubes.get(jx, jy, jz) ) continue;
								cubes.set(jx, jy, jz, 2);
								new_queue->push_back(Point(jx, jy, jz));
							}
						}
					}else{

						jx = kx; jy=ky; jz=kz;
						if (jx >= 0 && jy >= 0 && jz >= 0 ){
							if ( cubes.get(jx, jy, jz) ) continue;
							cubes.set(jx, jy, jz, 2);
							new_queue->push_back(Point(jx, jy, jz));
						}
					}
				}
		}
}

//use relaxed density range for surface expansion (use tight density range for initial seedings)
void surfaceExpansion_6way(vector<Point>* new_queue, Point& p, int adj4_flag=0){

	int six_neighbors[6][3] = { {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1 }, {0,0,-1} };

	int kx=0, ky=0, kz=0;
	int jx=0, jy=0, jz=0, j=0;
	
	for (int n = 0; n < 6; ++n) {

		kx = p.x() + six_neighbors[n][0];
		ky = p.y() + six_neighbors[n][1];
		kz = p.z() + six_neighbors[n][2];

		// outside of data volume
		if (kx < 0 || kx >= width || ky < 0 || ky >= height || kz < 0 || kz >= slice ) continue;

		//a valid seed: must satisfy the weak inclusion test and density range test
		for (int dir = 0; dir < 3; ++dir)
			if(validSeed(Point(kx,ky,kz), ts->get_relax_density_range(), dir)){

				if(adj4_flag){
					for(int col=0; col<4; col++){

						jx = kx + four_adjacent_increment[dir][col][0];
						jy = ky + four_adjacent_increment[dir][col][1];
						jz = kz + four_adjacent_increment[dir][col][2];

						if (jx >= 0 && jy >= 0 && jz >= 0 ){
							if ( cubes.get(jx, jy, jz) ) continue;
							cubes.set(jx, jy, jz, 2);
							new_queue->push_back(Point(jx, jy, jz));
						}
					}
				}else{

					jx = kx; jy=ky; jz=kz;
					if (jx >= 0 && jy >= 0 && jz >= 0 ){
						if ( cubes.get(jx, jy, jz) ) continue;
						cubes.set(jx, jy, jz, 2);
						new_queue->push_back(Point(jx, jy, jz));
					}

				}
			}
	}
}

//define number of relaxed expansion steps that are allowed by locality criterion
//expansion will stop if there is a relaxed expansion cube more than MAX_RELAXED_EXPANSION cubes
//away from a cube uses tight range
void surfaceExpansion_Relax3(vector<Point>* new_queue, Point& p, int max_step, int adj4_flag=0){
	
	int kx=0, ky=0, kz=0;
	int jx=0, jy=0, jz=0, j=0; 
	float curr_image_range[2]={0.0,0.0};
	
		//int max_step = 3;
	int relax_mark =3;
	int MAX_RELAXED_EXPANSION = relax_mark + max_step;
	
	//cube mark is set to 2 (if it pass the tight range) 
	//cube mark is set to 3, and keep increment until it reaches the MAX_RELAEXD_EXPANSION.
	int cube_mark= 0;
	int new_mark=0;
	 
	cube_mark = cubes.get(p.x(), p.y(),p.z());
	
	//exceeded allowed local relaxation criterion - STOP
	if(cube_mark >MAX_RELAXED_EXPANSION) return;
	else{
      
		curr_image_range[HIGH] = ts->get_density(HIGH);
        curr_image_range[LOW] =  ts->get_density(LOW);
	
		if(cube_mark >= relax_mark){	
			curr_image_range[HIGH] =  ts->get_relax_density(HIGH);;
			curr_image_range[LOW] =  ts->get_relax_density(LOW);;
		}
	}
	
	for (int l = 0; l < 3; ++l) 
		for (int m = 0; m < 8; ++m) {  //extract eight neighbors one dimension(l) at a time 
			
			//neighbour points (not yet marked)
			kx = p.x() + eight_edge_neighbor_increment[l][m][0];    
			ky = p.y() + eight_edge_neighbor_increment[l][m][1];
			kz = p.z() + eight_edge_neighbor_increment[l][m][2];
			
			// outside of data volume
			if (kx < 0 || kx >= width || ky < 0 || ky >= height || kz < 0 || kz >= slice ) continue; 
			
				//a valid seed: must satisfy the weak inclusion test and density range test
			for (int dir = 0; dir < 3; ++dir)    
				if(validSeed(Point(kx,ky,kz), curr_image_range, dir)){
					
					if(adj4_flag){
						for(int col=0; col<4; col++){ //4 adjacency cubes
						
							jx = kx + four_adjacent_increment[dir][col][0];
							jy = ky + four_adjacent_increment[dir][col][1];
							jz = kz + four_adjacent_increment[dir][col][2];
						
							if (jx >= 0 && jy >= 0 && jz >= 0 ){
								if ( cubes.get(jx, jy, jz) ) continue;
                            
								//New Code:
								//1st condition: did not pass the tight inclusion test
								//2nd condition: (mark 2) they are 4-adjacency, mark no matter what
								if(!validSeed(Point(jx,jy,jz), ts->get_density_range(), dir))
								{
									if (cube_mark == 1) new_mark = relax_mark;
									else new_mark = cube_mark + 1;
								
									cubes.set(jx, jy, jz, new_mark);
								}else
									cubes.set(jx, jy, jz, 2);
								
								new_queue->push_back(Point(jx, jy, jz));
							}
						}
					}else{
						jx = kx; jy=ky; jz=kz;
						if (jx >= 0 && jy >= 0 && jz >= 0 ){
							if ( cubes.get(jx, jy, jz) ) continue;

							//New Code:
							//1st condition: did not pass the tight inclusion test
							//2nd condition: (mark 2) they are 4-adjacency, mark no matter what
							if(!validSeed(Point(jx,jy,jz), ts->get_density_range(), dir))
							{
								if (cube_mark == 1) new_mark = relax_mark;
								else new_mark = cube_mark + 1;

								cubes.set(jx, jy, jz, new_mark);
							}else
								cubes.set(jx, jy, jz, 2);

							new_queue->push_back(Point(jx, jy, jz));
						}


					}
				}
		}
}
//No relaxation
void surfaceExpansion(vector<Point>* new_queue, Point& p, int adj4_flag=0){
	
	int kx=0, ky=0, kz=0;
	int jx=0, jy=0, jz=0, j=0; 
	
	for (int l = 0; l < 3; ++l) 
		for (int m = 0; m < 8; ++m) { 
			
			// 18 ways connectivities
			kx = p.x() + eight_edge_neighbor_increment[l][m][0];    
			ky = p.y() + eight_edge_neighbor_increment[l][m][1];
			kz = p.z() + eight_edge_neighbor_increment[l][m][2];
			
			// outside of data volume
			if (kx < 0 || kx >= width || ky < 0 || ky >= height || kz < 0 || kz >= slice ) continue; 
            
				for (int dir = 0; dir < 3; ++dir)    
					if(validSeed(Point(kx,ky,kz), ts->get_density_range(), dir)){
						
						if(adj4_flag){
							for(int col=0; col<4; col++){//mark 4 seeds which shares the same edge of p(kx,ky,kz)
					
								jx = kx + four_adjacent_increment[dir][col][0];
								jy = ky + four_adjacent_increment[dir][col][1];
								jz = kz + four_adjacent_increment[dir][col][2];

								if (jx >= 0 && jy >= 0 && jz >= 0 ){
									if ( cubes.get(jx, jy, jz) ) continue;
										cubes.set(jx, jy, jz, 2);
										new_queue->push_back(Point(jx, jy, jz));
								}
							}
						}else{
							
							jx = kx; jy=ky; jz=kz;
							if (jx >= 0 && jy >= 0 && jz >= 0 ){
								if ( cubes.get(jx, jy, jz) ) continue;
									cubes.set(jx, jy, jz, 2);
									new_queue->push_back(Point(jx, jy, jz));
							}

						}
				}
	}
}

bool valid_OriginalSeed(Point& p1, Point& p2,  double strength){

	bool valid = false;
	
	if(!no_density_test){
		if((strength > ts->get_edgeStrength(HIGH)) &&
		   (inDensityRange(p1, ts->get_density_range()) || inDensityRange(p2, ts->get_density_range())))
		   valid = true;
	}else{
		if(strength > ts->get_edgeStrength(HIGH)) valid = true;
	}
	return valid;
}
	
bool inDensityRange(Point p, float image_range[]){
	
	int inRange = false;
	
	if( m3D_Data->get(p.x(), p.y(), p.z()) > ts->get_density(LOW) &&
	    m3D_Data->get(p.x(), p.y(), p.z()) < ts->get_density(HIGH)){
		inRange = true;
	}
	return inRange;
}
	
//Marching Cubes Version
void markSeeds_highThresholdFilter_MARINANEW(Point& p, Point p2, float& strength, int dir){
	edgeNeighbors_t ei;
	int jx=0, jy=0, jz=0;
	
	if(valid_OriginalSeed(p, p2, strength)){
		
		cubes.set(p.x(), p.y(), p.z(), 1); 
		
		//4-adjacency analysis
		for (int l = 0; l < 3; ++l){    
			
			ei = getOriginalSeeds(l, dir); 
			
			jx = p.x() + ei.original_seeds[0]; 
			jy = p.y() + ei.original_seeds[1]; 
			jz = p.z() + ei.original_seeds[2]; 
			
			if (jx >= 0 && jy >= 0 && jz >= 0 ) 
				cubes.set(jx, jy, jz, 1); 
		}
	}
}
void markEdgePoints_lowThresholdFilter(Point& p, float& strength, const char bitPos){
    
    if(strength > ts->get_curr_edgeStrength(LOW)){
		char accumulated_marks = edge_mark.get(p.x(),p.y(),p.z());
		char t = accumulated_marks | bitPos;
		edge_mark.set( p.x(),p.y(),p.z(),t );
	}
}
//this function is not working
bool validSeed_withRelaxation(Point p1, int dim){
	
	bool valid = false;
	int p2x=p1.x(), p2y=p1.y(), p2z=p1.z();
	float sX=0.0, sY=0.0, sZ=0.0;
	sX = edge_strengthX->get(p1.x(), p1.y(), p1.z());
	sY = edge_strengthY->get(p1.x(), p1.y(), p1.z());
	sZ = edge_strengthZ->get(p1.x(), p1.y(), p1.z()); 
	
	float curr_strength=0.0;
	
	switch (dim) {  case _X_: { p2x++; curr_strength = sX; break;}
		case _Y_: { p2y++; curr_strength = sY; break;}
		case _Z_: { p2z++; curr_strength = sZ; break;} }
	
	Point p2(p2x,p2y,p2z);	
	float relax_image_range[2]={-200.0, -760.0};   //2D looks good, 3D looks bad
												   //	float relax_image_range[2]={-200.0, -960.0};  **too many edges are detected
	
	if(!no_density_test){
		
		if((edge_mark.get(p1.x(),p1.y(),p1.z()) >> dim) & 1){
			
			if(curr_strength >= 30.0){
				
				if(inDensityRange(p1, ts->get_density_range()) || inDensityRange(p2, ts->get_density_range()))
					valid = true;
			}else{ 
				if(inDensityRange(p1, ts->get_relax_density_range()) || inDensityRange(p2, ts->get_relax_density_range()))
					valid = true;
			}
			
		}
	}else{
		valid = true; 
	}
	return valid;
}
Matrix3D<char>* get_edgeMarks_Martrix(){ return &edge_mark; }
Matrix3D<char>* get_cubes_Martrix(){ return &cubes; }
int get_num_passes(){ return pass; }
int get_num_edgeMarks() {return edgeMarkCount; }
int get_num_cubeMarks() {return cubeMarkCount; }
int get_num_markedCubes_Final() {return count_cubes; }
int get_cubeMarks0_Final() {return cube_count0; }
int get_cubeMarks1_Final() {return cube_count1; }
int get_cubeMarks2_Final() {return cube_count2; }
int get_cubeMarks3_Final() {return cube_count3; }
	
//there is a problem here, shouldn't it be the three elements?
edgeNeighbors_t getOriginalSeeds(int col, int dim){
	
	edgeNeighbors_t ei;
	memcpy(ei.original_seeds, &(adjacent_increment[dim][col][0]), 3*sizeof(int));	
	
	return ei;
}
	
private: 
	
	bool no_density_test;
	unsigned int width, height, slice;
	static int four_adjacent_increment[3][4][3];  
	static int eight_edge_neighbor_increment[3][8][3];
	static int adjacent_increment[3][3][3];
	
	Matrix3D<float>* m3D_Data;
	Matrix3D<float>* edge_strengthX;
	Matrix3D<float>* edge_strengthY;  
	Matrix3D<float>* edge_strengthZ;
	Matrix3D<Vector>* surfaceCrossing_gradient;
	Matrix3D<char> edge_mark;
	Matrix3D<char> cubes;
	
	int pass_edge_mark_test;
	int not_marked_test;
	int isMarked;
	int edgeMarkCount;
	int cubeMarkCount;
	int count_cubes;
	int cube_count0; 
	int cube_count1; 
	int cube_count2; 
	int cube_count3;
	int pass;
    
    Threshold *ts;
	
};
#endif
