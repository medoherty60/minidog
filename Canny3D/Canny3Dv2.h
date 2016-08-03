#ifndef CANNY3DV2_NEW_H
#define CANNY3DV2_NEW_H

#include "Canny3Dabstract.h"
#include "Matrix3D.h"
#include "Threshold.h"
#include "Vector.h"
#include "WriteFile.h"
#include "Point.h"
#include <utility>
#include <list>
using namespace std;

// an edge extends from the grid point p to the next
// grid point along the axis specified by d (direction).
struct Edge {
	Point p;
	CoordAxis d;
	unsigned char mark;
	Edge(Point& _p, CoordAxis _d, unsigned char _mark=0)
	{ p = _p; d = _d; mark = _mark; }
};

class Canny3Dv2 : public Canny3Dabstract{
	
public:

Canny3Dv2(bool _t, int _dim3D[], Matrix3D<float>* _m3D_Data, Matrix3D<Vector>* _scg,
		Matrix3D<float>* _esX, Matrix3D<float>* _esY,  Matrix3D<float>* _esZ,
		Threshold* _ts){

	// Store data received as constructor parameters.
	no_density_test = _t;
	width = _dim3D[_X_];  
	height= _dim3D[_Y_];  
	slice = _dim3D[_Z_];
	m3D_Data = _m3D_Data;
	edge_strengthX = _esX;
	edge_strengthY = _esY;  
	edge_strengthZ = _esZ;
	surfaceCrossing_gradient = _scg;
    ts = _ts;
    
    // Initialize bookkeeping values for processing.
	pass=1;
	xEdgeDone.resize(width,height,slice); xEdgeDone.setAll(false);
	yEdgeDone.resize(width,height,slice); yEdgeDone.setAll(false);
	zEdgeDone.resize(width,height,slice); zEdgeDone.setAll(false);

	// Initialize attributes that will be used for reporting once
	// processing is finished.
	edgeMarkCount=0;
	cubeMarkCount=0;
	count_cubes=0;
	cube_count0=0;
	cube_count1=0;
	cube_count2=0;
	cube_count3=0;
	
	// Print the basic process control parameters.
	//printf("x=%d, y=%d, z=%d", width, height, slice);
    ts->print_curr_edgeStrength();
    ts->print_density();
    ts->print_relax_density();

    analyzeInputCubes();
}
	
virtual ~Canny3Dv2() {};

void clampBorderIdx(int& idx, int maxVal){
	int min = 0;
	int max = maxVal;
	
	if(idx<min) idx=min;
	else if(idx>=max) idx = maxVal-1;
}

// This is an analysis function, not required for processing.
void analyzeInputCubes()
{
	unsigned long counts[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	for (int z=0; z<slice; z++) {
		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				int c = 0;
				float s;
				s = edge_strengthX->get(x,y,z); if (testLowEdgeStrength(s)){ c++; }
				s = edge_strengthY->get(x,y,z); if (testLowEdgeStrength(s)){ c++; }
				s = edge_strengthZ->get(x,y,z); if (testLowEdgeStrength(s)){ c++; }

				s = edge_strengthX->get(x,y+1,z); if (testLowEdgeStrength(s)){ c++; }
				s = edge_strengthZ->get(x,y+1,z); if (testLowEdgeStrength(s)){ c++; }

				s = edge_strengthX->get(x,y,z+1); if (testLowEdgeStrength(s)){ c++; }
				s = edge_strengthY->get(x,y,z+1); if (testLowEdgeStrength(s)){ c++; }

				s = edge_strengthY->get(x+1,y,z); if (testLowEdgeStrength(s)){ c++; }
				s = edge_strengthZ->get(x+1,y,z); if (testLowEdgeStrength(s)){ c++; }

				s = edge_strengthX->get(x,y+1,z+1); if (testLowEdgeStrength(s)){ c++; }
				s = edge_strengthY->get(x+1,y,z+1); if (testLowEdgeStrength(s)){ c++; }
				s = edge_strengthZ->get(x+1,y+1,z); if (testLowEdgeStrength(s)){ c++; }
				counts[c]++;
			}
		}
	}
	cout << "Initial cube analysis, based on lower edge strength test:" << endl;
	unsigned long total = 0;
	for (int i=0; i<13; i++) {
		cout << setw(10) << counts[i] << " cubes with " << setw(2) << i << " marked edges" << endl;
		total += counts[i];
	}
	cout << setw(10) << total << " total cubes" << endl;
}

// This function checks the edge strength of every edge.
// Edges that pass the lower edge strength criteria are added to the low_strength_edges list.
// Edges that pass the validOriginalSeed test are added to the crossed_edges list.
// A better name for this function might be filterEdgesByStrength().
// This function also accumulates the edgeMarkCount and cubeMarkCount values, since
//   these can no longer be counted from the edge_marks matrix.
void Find_Initial_Seed_EdgeCubes(){
	
	Point p;
	float sX=0.0, sY=0.0, sZ=0.0;
	cubes.resize(width, height, slice);
	crossed_edges.reserve(20000);
	
	for (int z=0; z<slice; z++) {
		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				
				p = Point(x,y,z);
				sX = edge_strengthX->get(x,y,z);
				sY = edge_strengthY->get(x,y,z);
				sZ = edge_strengthZ->get(x,y,z);
				
				bool count_cube = false;

				//process x
				if(sX != -9999) {
					Edge ex(p,_X_,1);
					if (testLowEdgeStrength(sX)){ edgeMarkCount++; count_cube = true; }
					Point p2(x+1,y,z);
					if (valid_OriginalSeed(p, p2, sX)) crossed_edges.push_back(ex);
				}
				 //process y
				if(sY != -9999) {
					Edge ey(p,_Y_,1);
					if (testLowEdgeStrength(sY)) { edgeMarkCount++; count_cube = true; }
					Point p2(x,y+1,z);
					if (valid_OriginalSeed(p, p2, sY)) crossed_edges.push_back(ey);
				}
				//process z
				if(sZ != -9999) {
					if (testLowEdgeStrength(sZ)) { edgeMarkCount++; count_cube = true; }
					Edge ez(p,_Z_,1);
					Point p2(x,y,z+1);
					if (valid_OriginalSeed(p, p2, sZ)) crossed_edges.push_back(ez);
				}
				if (count_cube) cubeMarkCount++;
			}
		}
	}
}

// This function pushes all edges in the crossed_edges list into the edge_queue.
void loadInitialEdgeQueue() {
	for (unsigned int i=0; i<crossed_edges.size(); i++)
		enqueueEdge(crossed_edges[i]);
}

// This function adds an edge to the edge_queue, but only if it hasn't already been in the queue.
// Enqueued edges are also added to the crossed_edges list, on the assumption that an edge would
//   only be queued if it is crossed by the surface.
void enqueueEdge(Edge& e)
{
	int x = e.p.x(), y = e.p.y(), z = e.p.z();
	switch (e.d) {
	case _X_:
		if (xEdgeDone.get(x,y,z)) return;
		xEdgeDone.set(x,y,z,true);
		break;
	case _Y_:
		if (yEdgeDone.get(x,y,z)) return;
		yEdgeDone.set(x,y,z,true);
		break;
	case _Z_:
		if (zEdgeDone.get(x,y,z)) return;
		zEdgeDone.set(x,y,z,true);
		break;
	}
	edge_queue.push_back(e);
	crossed_edges.push_back(e);
}

// This function pulls edges from the edge_queue and passes
// them to be processed for expansion.
// This function needs a better name.
void Hysteresis(int& isRelax){
	int relax_flag = 0;
	int orig_flag = 2;
	int adj4_flag = 1;

	loadInitialEdgeQueue();

	while (edge_queue.size()>0) {
		Edge e = edge_queue.front();
		edge_queue.pop_front();
		expandFromEdge(e, isRelax, relax_flag, orig_flag, adj4_flag);
	}

	// Once all the crossed edges are found, mark the crossed cubes.
	markCubesContainingCrossedEdges();
}

// This function selects the expansion strategy based on the control flags.
void expandFromEdge(Edge& e, int isRelax, int relax_flag, int orig_flag, int adj4_flag)
{
	surfaceExpansion_test1(e, isRelax && relax_flag);
	/*
	if (isRelax) {
		switch(relax_flag){
		// 24 conn
		//case 1: surfaceExpansion_Relax(,(*current_queue)[i],adj4_flag); break;
		//case 2: surfaceExpansion_Relax2(new_queue,(*current_queue)[i],adj4_flag); break;
		//case 3: surfaceExpansion_Relax3(new_queue,(*current_queue)[i], 20, adj4_flag); break;
		// MED 160706 - 6 way expansion
		//case 4: surfaceExpansion_6way(new_queue, (*current_queue)[i], adj4_flag); break;
		default: break;
		}
	}else{
		switch(orig_flag){
		//case 1: surfaceExpansion(new_queue, (*current_queue)[i], adj4_flag);  break;
		//case 2: surfaceExpansion_6way(new_queue, (*current_queue)[i], adj4_flag); break;
		default: break;
		}
	}
	*/
}

//** The various surfaceExpansion functions define various strategies for selecting
//** new edges for processing.
//** These functions receive a single edges and some control parameters.
//** The add edges to the edge_queue by calling enqueueEdge()
//** and they may also add validated edges to the crossed_edges list.
//** These functions do not mark cubes, that will be done from the crossed_edges list.

//** NOTE FOR FURTHER DEVELOPMENT - It seems there are two options in defining the
//** expansion strategy: (1) The "connectivity" rules for selecting potential edges,
//** and (2) the validity criteria for determining if a potential edge is a crossing edge.
//** It might make sense to separate these two things so that they can be independently
//** selected by expandFromEdge().

void surfaceExpansion_test1(Edge& e, bool use_relax=false){

	Point p = e.p;
	CoordAxis dir = e.d;
	unsigned char current_mark = e.mark;
	unsigned char next_mark = current_mark + 1;
	if (next_mark > pass) pass = next_mark;

	// Generate a list of neighboring edges.
	list<Edge> neighbors;
	getNeighbors(e, neighbors);

	// Test each neighbor for surface crossing.
	while (neighbors.size() > 0)
	{
		Edge enew = neighbors.front();
		neighbors.pop_front();
		bool edge_is_valid = false;
		if (use_relax)
			edge_is_valid = validSeed(enew, ts->get_relax_density_range());
		else
			edge_is_valid = validSeed(enew, ts->get_density_range());
		if (edge_is_valid)
		{
			enew.mark = next_mark;
			enqueueEdge(enew);
		}
	}

}

void getNeighbors(Edge& e, list<Edge>& n)
{
	int spread = 2;
	int i = e.p.x(), j = e.p.y(), k = e.p.z();
	int di, dj, dk;
	CoordAxis dir = e.d;
	if (dir == _X_) {
		// X edges parallel to e:
		for (dj=-spread; dj<=spread; dj++)
			for (dk=-spread; dk<=spread; dk++)
				if ((dj!=0)||(dk!=0)) {
					Point p(i,j+dj,k+dk);
					n.push_back(Edge(p, _X_, 0));
				}
		// nearby Y edges
		for (di=0; di<=spread; di++)
			for (dj=-spread; dj<=0; dj++)
				for (dk=-spread; dk<=spread; dk++) {
					Point p(i+di,j+dj,k+dk);
					n.push_back(Edge(p, _Y_, 0));
				}
		// nearby Z edges
		for (di=0; di<=spread; di++)
			for (dj=-spread; dj<=spread; dj++)
				for (dk=-spread; dk<=0; dk++) {
					Point p(i+di,j+dj,k+dk);
					n.push_back(Edge(p, _Z_, 0));
				}
	}
	if (dir == _Y_) {
		// Y edges parallel to e:
		for (di=-spread; di<=spread; di++)
			for (dk=-spread; dk<=spread; dk++)
				if ((di!=0)||(dk!=0)) {
					Point p(i+di,j,k+dk);
					n.push_back(Edge(p, _Y_, 0));
				}
		// nearby X edges
		for (di=-spread; di<=0; di++)
			for (dj=0; dj<=spread; dj++)
				for (dk=-spread; dk<=spread; dk++) {
					Point p(i+di,j+dj,k+dk);
					n.push_back(Edge(p, _X_, 0));
				}
		// nearby Z edges
		for (di=-spread; di<=spread; di++)
			for (dj=0; dj<=spread; dj++)
				for (dk=-spread; dk<=0; dk++) {
					Point p(i+di,j+dj,k+dk);
					n.push_back(Edge(p, _Z_, 0));
				}
	}
	if (dir == _Z_) {
		// Z edges parallel to e:
		for (di=-spread; di<=spread; di++)
			for (dj=-spread; dj<=spread; dj++)
				if ((di!=0)||(dj!=0)) {
					Point p(i+di,j+dj,k);
					n.push_back(Edge(p, _Z_, 0));
				}
		// nearby X edges
		for (di=-spread; di<=0; di++)
			for (dj=-spread; dj<=spread; dj++)
				for (dk=0; dk<=spread; dk++) {
					Point p(i+di,j+dj,k+dk);
					n.push_back(Edge(p, _X_, 0));
				}
		// nearby Y edges
		for (di=-spread; di<=spread; di++)
			for (dj=-spread; dj<=0; dj++)
				for (dk=0; dk<=spread; dk++) {
					Point p(i+di,j+dj,k+dk);
					n.push_back(Edge(p, _Y_, 0));
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

bool validSeed(Edge e, float curr_image_range[]){

	Point p1 = e.p;
	CoordAxis dir = e.d;
	int x=p1.x(),y=p1.y(),z=p1.z();

	bool valid_strength = false;
	if (dir == _X_) {
		float sX = edge_strengthX->get(x,y,z);
		valid_strength = testLowEdgeStrength(sX);
	}
	if (dir == _Y_) {
		float sY = edge_strengthY->get(x,y,z);
		valid_strength = testLowEdgeStrength(sY);
	}
	if (dir == _Z_) {
		float sZ = edge_strengthZ->get(x,y,z);
		valid_strength = testLowEdgeStrength(sZ);
	}

	bool valid_density = true;

	if(!no_density_test) {

		int p2x=p1.x(), p2y=p1.y(), p2z=p1.z();

		switch (dir) {  case _X_: { p2x++; break;}
						case _Y_: { p2y++; break;}
						case _Z_: { p2z++; break;} }
		Point p2(p2x,p2y,p2z);

		valid_density = (inDensityRange(p1, curr_image_range) || inDensityRange(p2, curr_image_range));
	}

	return valid_strength && valid_density;
}

bool inDensityRange(Point p, float image_range[]){
	
	int inRange = false;
	
	if( m3D_Data->get(p.x(), p.y(), p.z()) > ts->get_density(LOW) &&
	    m3D_Data->get(p.x(), p.y(), p.z()) < ts->get_density(HIGH)){
		inRange = true;
	}
	return inRange;
}

	void markCubesContainingCrossedEdges()
	{
		for (unsigned int i; i<crossed_edges.size(); i++) {
			Edge e = crossed_edges[i];
			Point p = e.p;
			CoordAxis dir = e.d;
			unsigned char mark = e.mark;

			// for each of the four cubes that share edge e
			for (int col=0; col<4; col++) {
				int jx = p.x() + four_adjacent_increment[dir][col][0];
				int jy = p.y() + four_adjacent_increment[dir][col][1];
				int jz = p.z() + four_adjacent_increment[dir][col][2];

				// keep the smallest non-zero mark for each cube
				unsigned char old_mark = cubes.get(jx,jy,jz);
				if ((old_mark == 0) || (old_mark > mark))
					cubes.set(jx,jy,jz,mark);
			}
		}
	}

	bool testLowEdgeStrength(float& strength){
		return (strength > ts->get_curr_edgeStrength(LOW));
	}

	Matrix3D<char>* get_cubes_Martrix(){ return &cubes; }
	int get_num_passes(){ return pass; }
	int get_num_edgeMarks() {return edgeMarkCount; }
	int get_num_cubeMarks() {return cubeMarkCount; }
	int get_num_markedCubes_Final() {return count_cubes; }
	int get_cubeMarks0_Final() {return cube_count0; }
	int get_cubeMarks1_Final() {return cube_count1; }
	int get_cubeMarks2_Final() {return cube_count2; }
	int get_cubeMarks3_Final() {return cube_count3; }

	// These methods are all used for reporting and storage after processing.
	void print(Matrix3D<char>& markers, string text);
	void print();
	void print_relax();
	void writeToCubesFile(string _fname, string file_extension, char _w);
	void writeToCubesStatisticFile(string _fname, string file_extension, float upperThreshold, char _w);
	void filterCubes(Matrix3D<char>& data);

private: 
	
// Attributes holding data received from outside the class.
// These attributes are read-only.

	bool no_density_test;
	unsigned int width, height, slice;
	Matrix3D<float>* m3D_Data;
	Matrix3D<float>* edge_strengthX;
	Matrix3D<float>* edge_strengthY;
	Matrix3D<float>* edge_strengthZ;
	Matrix3D<Vector>* surfaceCrossing_gradient;
    Threshold *ts;

 // Attributes that record the results of the edge and cube selection algorithm.
	// edges determined to be crossed by the surface.
	vector<Edge> crossed_edges;
	// cubes determined to be crossed by the surface.
	// a better name would be crossed_cubes or surface_cubes
	Matrix3D<char> cubes;

// Attributes used by the edge/cube selection algorithm.

	int pass;
	// edges waiting to be processed for expansion.
	list<Edge> edge_queue;
	// tables for remembering edges that have already been processed
	Matrix3D<bool> xEdgeDone, yEdgeDone, zEdgeDone;

// Static data defining neighborhoods and adjacencies.

	static int four_adjacent_increment[3][4][3];  
	static int eight_edge_neighbor_increment[3][8][3];
	static int adjacent_increment[3][3][3];

// Used for reporting after the selection algorithm is complete.
	
	int edgeMarkCount;
	int cubeMarkCount;
	int count_cubes;
	int cube_count0; 
	int cube_count1; 
	int cube_count2; 
	int cube_count3;
	
};
#endif
