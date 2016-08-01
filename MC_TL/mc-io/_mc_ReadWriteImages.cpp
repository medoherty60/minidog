#include "_mc_ReadWriteImages.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

bool FileManager::isWhitespace(char c)
{
	return ((c==' ') || (c=='\t') || (c=='\n') || (c=='\r'));
}

void FileManager::trimTrailingWhitespace(char* s)
{
	int i = strlen(s)-1;
	while ((i>=0) && isWhitespace(s[i]))
	{
		s[i] = '\0';
		i--;
	}
}
// format: 1=float, 2=short
int FileManager::readBuffer(float* buffer, int data_format, int num_words, FILE* fp, int isSwapBytes)
{
	// for float format, read directly into the passed buffer
	if (data_format == 4) {
		int n = fread(buffer, data_format, num_words, fp);
		if(isSwapBytes) 
			for (int i=0; i<n; i++)
				buffer[i] = swapBytes(buffer[i]);
		return n;
	}
	// for short format, read first into a temporary buffer
	else if (data_format == 2) {
		short* sbuffer = new short[num_words];
		int n = fread(sbuffer, data_format, num_words, fp);

		if(isSwapBytes) 
			for (int i=0; i<n; i++)
				// REPLACE THIS WITH A TWO BYTE SWAP
				sbuffer[i] = swapBytes(sbuffer[i]);
		
		// copy to actual buffer and convert to floats
		for (int i=0; i<n; i++) 
			buffer[i] = float(sbuffer[i]);

		delete [] sbuffer;
		return n;
	}// for char format, read first into a temporary buffer
	else if (data_format == 1) {
        // MEDTEST - assume bytes are unsigned
		//char* sbuffer = new char[num_words];
        unsigned char* sbuffer = new unsigned char[num_words];
        int n = fread(sbuffer, data_format, num_words, fp);
		
			// copy to actual buffer and convert to floats
		for (int i=0; i<n; i++) 
			buffer[i] = float(sbuffer[i]);
		
		delete [] sbuffer;
		return n;
	}else cout<<"Error: Data format not supported"<<endl;

	return 0;
}
void FileManager::getDensity_fromFile(float& buffer, float& density, float& small, float& big){
	
	density = buffer;

	if(density<small) small=density; //largest
	if(big<density) big=density;     //smallest
}
// infile or file_name is a pointer to the 1st char in the array
// frac is the color percentage of the data range
// *****************************************
// * Color Interpolation (255/1444)=0.17
// *****************************************
// float SF = dataRange*0.3;
// float frac = 255.0/(SF+dataRange);
// float dataRange = mcs.smallestVal()+mcs.largestVal();
//
// Data Description:
//   • Data Domain:  [dimX, dimY, dimZ]; 
//   • Cube Domain:  [dimX-1, dimY-1, dimZ-1]
// Buffer Types:
//   • buffer type: float (4 bytes) OR short (2 bytes)
//

bool FileManager::readVolumeData(Matrix3D<float>& m3D_Data, MarchingCube_header& header, int whichFile)
{	
	clock_t time = clock() ;
	char* fname; int bytes=1; bool isSwapBytes=0;

	switch(whichFile){
		case 1: fname = header.referenceName;
				bytes= header.bytes_ref;					// default: 4 bytes
				isSwapBytes = header.isSwapByte_ref;
				break;
		case 2: fname = header.shadingName;
			    bytes= header.bytes_shad;					// default: 4 bytes
				isSwapBytes = header.isSwapByte_shad;
				break;
		case 3: fname = header.cubeMarkName;
				bytes= header.bytes_cmk;					// default: 1 byte
				break;
	}

	int s1, s2;
	int oneSlice = header.nX * header.nY;
	float smallest=10000.0, largest=-10000.0, density=0.0;
	float buffer1[oneSlice], buffer2[oneSlice];
	m3D_Data.resize(header.nX , header.nY , header.nZ );
	
	FILE *fp = fopen(fname, "rb");			//*fp is a FILE pointer to the next bit to be read

	if (fp == NULL){
		return false;
	}else{
		s1 = readBuffer(buffer1, bytes, oneSlice, fp, isSwapBytes);  //n1 & n2 are number of words (oneSlice)
		s2 = readBuffer(buffer2, bytes, oneSlice, fp, isSwapBytes);
		if (s1<oneSlice || s2<oneSlice) return false;

		int bi=0;
		for (int k=0; k<header.nZ; k++){

			for (int j=0; j<header.nY; j++){
				for (int i=0; i<header.nX; i++) {

					getDensity_fromFile(buffer1[bi], density, smallest, largest);
					bi++;
					m3D_Data.set(i, j, k, density);
				}
			}
			memcpy(buffer1, buffer2, oneSlice*sizeof(float));	//destination, source, num bytes

			if(k<header.nZ-2){
				s2 = readBuffer(buffer2, bytes, oneSlice, fp, isSwapBytes);
				if (s2<oneSlice) return false;
			}
			bi=0;
		}//end for loop
	}

	switch(whichFile){
		case 1: header.density_high_ref = largest;
				header.density_low_ref = smallest; break;
		case 2: header.density_high_shad = largest;
				header.density_low_shad= smallest; break;
		case 3: header.density_high_cmk = largest;
				header.density_low_cmk = smallest; break;
	}

	display_mcHeader(whichFile);
	//m3D_dataPointsData.print(55, 60, 55, 60, 54, 55);
	printf("[MarchingCubes] Reading data ran in %lf secs.\n", (double)(clock() - time)/CLOCKS_PER_SEC) ;
	return true;
}/*
void FileManager::writeFaceListToFile()
{
	ofstream out("facelist_nov.txt");
	for (int f=0; f<global_facesVector.size(); f++)
	{
		Face& face = global_facesVector[f];
		Cube* cube = face.getCube();	
		
		out << "faceindex: " << f << endl;
		out << "cubeindex: " << cube->getIndex_Matrix3D() << endl;
		out << "point1: " << face.point1() << endl;
		out << "point2: " << face.point2() << endl;
		out << "point3: " << face.point3() << endl;
		out << "normal1: "<< face.normal1() << endl;
		out << "normal2: "<< face.normal2() << endl;
		out << "normal3: "<< face.normal3() << endl;
		out << "isReversed: " << face.getIsReversed() << endl;
	}
	out.close();
}*/
/*
void FileManager::compare_Neighbor_Density(short* _b1, short* _b2, int curr_index, 
										   Cube& c, int j, int TargetCubeIndex){
	
	if(TargetCubeIndex == curr_index){
		
		int ix1 = 1;
		int ix2 = mcs.dimX()+1;
		int ix3 = mcs.dimX();
		bool testSuccess = true;
		
		if(_b1[curr_index] != c.cube[0]) testSuccess = false;	
		if(_b1[curr_index+ix1] != c.cube[1]) testSuccess = false;		
		if(_b1[curr_index+ix2] != c.cube[2]) testSuccess = false;		
		if(_b1[curr_index+ix3] != c.cube[3]) testSuccess = false;	
		
		if(_b2[curr_index] != c.cube[4]) testSuccess = false;		
		if(_b2[curr_index+ix1] != c.cube[5]) testSuccess = false;		 
		if(_b2[curr_index+ix2] != c.cube[6]) testSuccess = false;		
		if(_b2[curr_index+ix3] != c.cube[7]) testSuccess = false;		
			
		if(!testSuccess){
			cout<<"j="<<j<<endl;
			cout<<"readIndex="<<curr_index<<endl
			<<"  density 0 ="<<_b1[curr_index]<<endl
			<<"  density 1 ="<<_b1[curr_index+ix1]<<endl
			<<"  density 2 ="<<_b1[curr_index+ix2]<<endl
			<<"  density 3 ="<<_b1[curr_index+ix3]<<endl
			<<"  density 4 ="<<_b2[curr_index]<<endl
			<<"  density 5 ="<<_b2[curr_index+ix1]<<endl
			<<"  density 6 ="<<_b2[curr_index+ix2]<<endl
			<<"  density 7 ="<<_b2[curr_index+ix3]<<endl;
			exit(0);
		}
	}
}*/
