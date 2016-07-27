#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <stdlib.h>
#include <time.h>
#include "cube.h"
#include "Matrix3D.h"
#include "Configurations.h"
#include "global.h"
/* 
	FileName: FileManager()
	Creation Date: Jun 11, 2012
	readDataFormat(): - read the data format and create an instance of MarchingCubes object
                        with attribues of (x,y,z) dimensions, smallest and largest density values.
    readVolumeData(): - read the volume into memory, and create Cube objects
 */
class FileManager{
	
public: 
	FileManager(){ start_time = time(NULL);}
	
	int readBuffer(float* buffer, int data_format, int num_words, FILE* fp, int isSwapBytes);
	bool readDataFormat(char* file_name, Configurations& conf);
	bool readVolumeData(Matrix3D<float>& m3D_Data, Configurations& conf, int whichFile);
	void getDensity_fromFile(float& buffer, float& density, float& small, float& big);
	
	/*void compare_Neighbor_Density(short* _b1, short* _b2, int curr_index, 
							     Cube& c, int j, int TargetCubeIndex);	*/

	void writeFaceListToFile();
	void trimTrailingWhitespace(char* s);
	bool isWhitespace(char c);
	
	union swapUnion2{
		short x;
		unsigned char c[2];
	};
	union swapUnion4{
		float x;
		unsigned char c[4];
	};
	short swapBytes(short val){
		
		swapUnion2 su;
		su.x = val;
		unsigned char tmp=su.c[0];
		su.c[0] = su.c[1];
		su.c[1] = tmp;
		return su.x;
	}
	float swapBytes(float val){
		
		swapUnion4 su;
		su.x = val;
		unsigned char tmp=su.c[0];
		su.c[0] = su.c[3];
		su.c[3] = tmp;
		tmp = su.c[1];
		su.c[1] = su.c[2];
		su.c[2] = tmp;
		
		return su.x;
	}
	
private:
	int start_time;
};
#endif

