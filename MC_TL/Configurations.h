#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

struct Configurations{
	
	//set up by the FileManager
	int cf_numFiles ;			 // num of input files
	
	char cf_fn1[600] ;			//input filename 1
	char cf_fn2[600] ;			//input filename 2
	char cf_fn3[600] ;			//input filename 3
	
	int cf_bytes_f1 ;			//num of bytes for file 1
	int cf_bytes_f2 ;			//num of bytes for file 2
	int cf_bytes_f3 ;			//num of bytes for file 3
	
	int cf_sb_f1 ;				//swap bytes for file 1
	int cf_sb_f2 ;				//swap bytes for file 2
	
	int cf_edrOperator ;		 //edge detection operator flag
	int cf_densityFilter;		 //density Filter
	int cf_markCube;			 //markers lookup
	float cf_isoVal;			//isovalue
	
	int cf_xdim ;				//data points dimensions
	int cf_ydim ;
	int cf_zdim ;

	int cf_cube_xdim ;			//cube dimension 
	int cf_cube_ydim ;
	int cf_cube_zdim ;  	
	
	float cf_deltaX ;			 //voxel size
	float cf_deltaY ;
	float cf_deltaZ ;
	
	float cf_two_deltaX ;		//2 voxel size
	float cf_two_deltaY ; 
	float cf_two_deltaZ ; 
	
	float cf_offsetX ;			//rendering offset
	float cf_offsetY ;	
	float cf_offsetZ ;
	
};
extern Configurations CONF;
#endif