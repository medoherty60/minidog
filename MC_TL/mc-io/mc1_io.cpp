#include "mc1_io.h"

MarchingCube_header HEADER;

void display_meta_mcHeader(){

	printf("********************************************************");
	printf("\n");
	printf("[MarchingCubes] Number of files:\t%i\n\n", HEADER.numFiles);
	printf("[MarchingCubes] isovalue:\t%f\n\n",HEADER.isoVal);
	printf("[MarchingCubes] markCubes:\t%i\n\n",HEADER.markCube);
	printf("[MarchingCubes] voxel size:\t(%f,%f,%f)\n\n",HEADER.dX,HEADER.dY,HEADER.dZ);
	printf("[MarchingCubes] two voxel size:\t(%f,%f,%f)\n\n",HEADER.two_dX,HEADER.two_dY,HEADER.two_dZ);
	printf("[MarchingCubes] rendering offset:\t(%f,%f,%f)\n\n",HEADER.offsetX,HEADER.offsetY,HEADER.offsetZ);
	printf("********************************************************");
	printf("\n");
}

void display_mcHeader(const int& whichFile){

	string fname; int bytes=1; bool isSwapBytes=0;
	float density_high=0, density_low=0;

	switch(whichFile){
		case 1: fname = HEADER.referenceName;
				bytes= HEADER.bytes_ref;					// default: 4 bytes
				isSwapBytes = HEADER.isSwapByte_ref;
				density_high = HEADER.density_high_ref;
				density_low = HEADER.density_low_ref;
				break;
		case 2: fname = HEADER.shadingName;
			    bytes= HEADER.bytes_shad;					// default: 4 bytes
				isSwapBytes = HEADER.isSwapByte_shad;
				density_high = HEADER.density_high_shad;
				density_low = HEADER.density_low_shad;
				break;
		case 3: fname = HEADER.cubeMarkName;
				bytes= HEADER.bytes_cmk;					// default: 1 byte
				density_high = HEADER.density_high_cmk;
				density_low = HEADER.density_low_cmk;
				break;
		default: fprintf(stderr,"Err:\t Invalid file numbers.\n"); break;
	}

	printf("********************************************************");
	printf("\n");
	printf("[MarchingCubes] File Name:\t%s\n\n", fname.c_str());
	printf("[MarchingCubes] Bytes:\t%i\n\n",bytes);
	printf("[MarchingCubes] isSwapBytes:\t%i\n\n",isSwapBytes);
	printf("[MarchingCubes] Grid dimension:\t(%i,%i,%i)\n\n",HEADER.nX,HEADER.nY,HEADER.nZ);
	printf("[MarchingCubes] voxel values:\t[%f,%f]\n\n",density_low,density_high);
}


