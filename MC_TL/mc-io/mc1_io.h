#ifndef MC1_IO_H
#define MC1_IO_H

#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

/*! \struct MarchingCube_header
    \brief High level data structure for storing header information of the
           input datasets in the mc1_io API.  This structure is used to
           implement one API for reading/writing ASCII data in the raw format.
 */

struct MarchingCube_header{		/*!< Image storage struct **/

	//constructor

	MarchingCube_header():markCube(true), isoVal(0), densityFilter(false), numFiles(0),
						  referenceName(NULL),shadingName(NULL),cubeMarkName(NULL),
						  isSwapByte_ref(false),isSwapByte_shad(false),
			              bytes_ref(4),bytes_shad(4),bytes_cmk(1),nX(0),nY(0),nZ(0),
						  dX(0),dY(0),dZ(0),two_dX(0),two_dY(0),two_dZ(0),
						  offsetX(0),offsetY(0),offsetZ(0),
						  density_high_ref(0.),density_low_ref(0.),
						  density_high_shad(0.),density_low_shad(0.),
						  density_high_cmk(0.),density_low_cmk(0.) {
	}

	/*! cube markers flag 																	*/
	/*! If true, cubeMark image will be initialized and to be used to guide the search 		*/
	/*! in the MarchingCubes algorithm 														*/

	bool markCube;				/** cube markers flag (default: true)	 			(used by MarchingCubes.cpp) */
	float isoVal;			    /** isovalue 		  (default: 0)					(used by RenderManager.cpp) */
	bool densityFilter;		    /** density Filter	  (default: false)				(used by MarchingCubes.cpp) */

	int numFiles ;			    /** num of input files     														*/
	
    char *referenceName;   		/** required image: any image where the surface is defined by iso.          	*/
    char *shadingName;     		/** optional image: an image where surface normals are defined for shading. 	*/
    char *cubeMarkName;    		/** optional image: an image where surface-crossing cubes are identified.   	*/

	bool isSwapByte_ref ;		/** swap bytes for reference image  (default: 0) 								*/
	bool isSwapByte_shad ;		/** swap bytes for shading image    (default: 0) 								*/
	
	int bytes_ref ;			    /** byte size for reference image   (default: 4 bytes)    						*/
	int bytes_shad ;			/** byte size for shading image     (default: 4 bytes)    						*/
	int bytes_cmk ;			    /** byte size for cubeMark image    (default: 1 byte)     						*/
	
	int nX ;					/** grid dimension of the images.        (default: (0,0,0)) 					*/
	int nY ;					/** all images must have the same dimension  									*/
	int nZ ;
	
	float dX ;			        /** size of a voxel                (used by MarchingCubes.cpp and RenderManager.cpp) */
	float dY ;
	float dZ ;
	
	float two_dX ;		    	/** 2 voxel size                                    (used by MarchingCubes.cpp) */
	float two_dY ;
	float two_dZ ;
	
	float offsetX ;			    /** rendering offset                                 (used by RenderManager.cpp) */
	float offsetY ;
	float offsetZ ;

    float density_high_ref;
    float density_low_ref;
    float density_high_shad;	/** density values for each image 												*/
    float density_low_shad;
    float density_high_cmk;
    float density_low_cmk;
} ;
void display_meta_mcHeader();
void display_mcHeader(const int& whichFile);
extern MarchingCube_header HEADER;
#endif
