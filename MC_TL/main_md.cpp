/*
 * main_md.cpp
 *
 *  Created on: Jul 29, 2016
 *      Author: msdoherty
 */
#include "mc-io/_mc_ReadWriteImages.h"
#include "MarchingCubes.h"
#include "UI/RenderManager.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

void PetitUsage(char *exec)
{
    fprintf(stderr,"Marching Cubes algorithm for surface extraction and display.\n");
    fprintf(stderr,"Usage:\t%s -ref <refImageName> -shad <shadingImageName> -cmk<cubeMarkImageName>"
    		"-dimX -dimY -dimZ [OPTIONS].\n",exec);
    fprintf(stderr,"\tSee the help for more details (-h).\n");
    return;
}
extern int pcbuckets[256];

int main(int argc, char** argv) {

	clock_t start = clock() ;

    if(argc==1){
        PetitUsage(argv[0]);
        return 1;
    }

	FileManager fm;
    MarchingCube_header & mcHeader = HEADER;
	Matrix3D<float> referenceObj;
	Matrix3D<float> shadingObj;
	Matrix3D<float> cubeMarkObj;

    /* read the input parameter */

    for(int i=1;i<argc;i++){

        if((strcmp(argv[i],"-ref")==0) || (strcmp(argv[i],"-target")==0) || (strcmp(argv[i],"--ref")==0)){
        	mcHeader.referenceName =argv[++i];
        }
        else if((strcmp(argv[i],"-shad")==0) || (strcmp(argv[i],"-shading")==0) || (strcmp(argv[i],"--shad")==0)){
        	mcHeader.shadingName=argv[++i];
        }
         else if((strcmp(argv[i],"-cmk")==0) || (strcmp(argv[i],"-cubeMark")==0) || (strcmp(argv[i],"--cmk")==0)){
        	 mcHeader.cubeMarkName=argv[++i];
        }
         else if(strcmp(argv[i], "-dimX")==0 || (strcmp(argv[i],"--dimX")==0)){
        	 mcHeader.nX = atoi(argv[++i]);
         }
         else if(strcmp(argv[i], "-dimY")==0 || (strcmp(argv[i],"--dimY")==0)){
        	 mcHeader.nY = atoi(argv[++i]);
         }
         else if(strcmp(argv[i], "-dimZ")==0 || (strcmp(argv[i],"--dimZ")==0)){
        	 mcHeader.nZ = atoi(argv[++i]);
         }
         else if(strcmp(argv[i], "-bytes_ref")==0 || (strcmp(argv[i],"--bytes_ref")==0)){
        	 mcHeader.bytes_ref = atoi(argv[++i]);
         }
         else if(strcmp(argv[i], "-bytes_shad")==0 || (strcmp(argv[i],"--bytes_shad")==0)){
        	 mcHeader.bytes_shad = atoi(argv[++i]);
         }
         else if(strcmp(argv[i], "-bytes_cmk")==0 || (strcmp(argv[i],"--bytes_cmk")==0)){
        	 mcHeader.bytes_cmk = atoi(argv[++i]);
         }
         else if(strcmp(argv[i], "-noCubeMark")==0 || (strcmp(argv[i],"--noCubeMark")==0)){
        	 mcHeader.markCube=false;
         }
         else if(strcmp(argv[i], "-isoVal")==0 || (strcmp(argv[i],"--isoVal")==0)){
        	 mcHeader.isoVal = atof(argv[++i]);
          }
         else if(strcmp(argv[i], "-densityFilter")==0 || (strcmp(argv[i],"--densityFilter")==0)){
        	 mcHeader.densityFilter=true;
          }
         else if(strcmp(argv[i], "-swapByte_ref")==0 || (strcmp(argv[i],"--swapByte_ref")==0)){
        	 mcHeader.isSwapByte_ref=true;
          }
         else if(strcmp(argv[i], "-swapByte_shad")==0 || (strcmp(argv[i],"--swapByte_shad")==0)){
        	 mcHeader.isSwapByte_shad=true;
          }
    }

    if(mcHeader.referenceName != NULL){
    	mcHeader.numFiles = 1;
    	if(mcHeader.referenceName == NULL){
    		 fprintf(stderr, "Error when reading the reference image header %s\n",mcHeader.referenceName);
    		 return 1;
    	}
    }

    if(mcHeader.markCube){
    	mcHeader.numFiles = 3;
    	if((mcHeader.shadingName == NULL || mcHeader.cubeMarkName == NULL)){
    		fprintf(stderr,"Err:\tThe cubeMark and the shading images have to be defined.\n");
    		PetitUsage(argv[0]);
    		return 1;
    	}
    }

	//Read the reference image
    if(!fm.readVolumeData(referenceObj, mcHeader, 1)){
        fprintf(stderr, "Error when reading the reference image %s\n",mcHeader.referenceName);
      	return 1;
    }

    if(mcHeader.markCube){
    	//Read the shading image
    	if(!fm.readVolumeData(shadingObj, mcHeader, 2)){
    		fprintf(stderr, "Error when reading the shading image %s\n",mcHeader.shadingName);
    		return 1;
    	}

    	//Read the cubeMark image
    	if(!fm.readVolumeData(cubeMarkObj, mcHeader, 3)){
    		fprintf(stderr, "Error when reading the cubeMark image %s\n",mcHeader.cubeMarkName);
    		return 1;
    	}
    }

    int cubes_x = mcHeader.nX-1;
    int cubes_y = mcHeader.nY-1;
    int cubes_z = mcHeader.nZ-1;
    int dim_L = 0;

	if( (cubes_x>cubes_y) && (cubes_x>cubes_z) )
		dim_L = cubes_x;
	else if( (cubes_y>cubes_x) && (cubes_y>cubes_z) )
		dim_L = cubes_y;
	else if( (cubes_z>cubes_x) && (cubes_z>cubes_y) )
		dim_L = cubes_z;
	else
		dim_L = cubes_y;


	if(dim_L < 0){
		fprintf(stderr, "Error: grid dimension must be positive value, %i,%i,%i\n",mcHeader.nX, mcHeader.nY,mcHeader.nZ);
		return 1;
	}
	else{											// ** MUST USE THE LARGEST DIMENSION (dim_L)
		//preserve aspect ratio						// this code does not preserve aspect ratio,
		mcHeader.dX = 1.0/dim_L;					// it stretches the data to fit into a unit size cube
		mcHeader.dY = 1.0/dim_L;				    // mcHeader.dX  = 1.0/nx;  ** deltaX, deltaY and deltaZ must not be zero
		mcHeader.dZ = 1.0/dim_L;				    // mcHeader.dY  = 1.0/ny;
													// mcHeader.dZ  = 1.0/nz;
		mcHeader.two_dX = mcHeader.dX*2;
		mcHeader.two_dY = mcHeader.dY*2;
		mcHeader.two_dZ = mcHeader.dZ*2;

		mcHeader.offsetX = -1*mcHeader.dX*(cubes_x/2);
		mcHeader.offsetY = -1*mcHeader.dY*(cubes_y/2);
		mcHeader.offsetZ = -1*mcHeader.dZ*(cubes_z/2);
	}

    display_meta_mcHeader();

    //-- create Marching cubes object --

	MarchingCubes* mcs = new MarchingCubes;
	mcs->set_resolution(mcHeader.nX, mcHeader.nY, mcHeader.nZ);
	mcs->init_all();
	mcs->set_ext_data(referenceObj.getPointer());

	switch(mcHeader.numFiles){
	case 1: { mcs->run( mcHeader, mcHeader.isoVal ); break; }
	case 3: { mcs->run( mcHeader,  mcHeader.isoVal, &shadingObj, &cubeMarkObj );
			  /*shadingObj.resize(1, 1, 1);
			  cubeMarkObj.resize(1, 1, 1);*/ break;
			}
	default: fprintf(stderr, "Error: invalid numFiles value."); break;
	}

    //wrapping up
	clock_t end = clock() ;
	double time = double (end - start) / CLOCKS_PER_SEC;

    int minutes = (int)floorf(time/60.0f);
    double seconds = time - 60.0*minutes;
    printf("[MarchingCubes] Performed in %i min %lf sec\n", minutes, seconds);
    printf("[MarchingCubes] Number of triangles and vertices are %i and %i", mcs->ntrigs(), mcs->nverts());
    printf("[MarchingCubes] Have a good day !\n");

    for (int i=0; i<256; i++) {
    	cout << "pcbuckets[" << setw(3) << i << "]=" << pcbuckets[i] << endl;
    }
	//-- start user interface
	RenderManager::initialize(&argc, argv);
	glutMainLoop();

}




