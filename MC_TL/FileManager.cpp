#include "FileManager.h"
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
bool FileManager::readDataFormat(char* file_name, Configurations& conf){
	
	cout<<"filename: "<<file_name<<endl;
	
	char data[200], path1[1000], path2[1000], path3[1000];
	char fn1[600], fn2[600], fn3[600];
	int tx, ty, tz, isoVal, bytes_f1=0, bytes_f2=0, bytes_f3=0;
	int sb_f1=0, sb_f2=0, edr=0, hf=0, nf=0, df=0, cube_marks=0;
	float ts_1=0.0, tl_1=0.0, ts_2=0.0, tl_2=0.0;
	int largest_dim=0;
   
    //char* fdir = (char*) "/Users/msdoherty/Desktop/MyDocuments/Davis-Research/Research-Max/3D_Canny/build/Debug/data/MERISTEM_213x170x168/";
    char* fdir = (char*) "/home/mdoherty/workspace/MC_TL/dataFile/MERISTEM_213x170x168/";
    char* xdir = (char*) "";
    
    // char* fdir = (char*) "/Users/msdoherty/Desktop/MyDocuments/Davis-Research/Research-Max/3D_Canny/build/Debug/";
   // char* fdir = (char*) "/Users/msdoherty/Desktop/MyDocuments/Davis-Research/Research-Max/3D_Canny/build/Debug/otherSurfaces/";
    //  char* xdir = (char*) "experiment[2014-06-22-13-24][CThead2_128x256x256]/fsize7/";
    /*
	
    strcpy(path1, "/Users/msdoherty/Desktop/MyDocuments/Davis-Research/Research-Max/3D_Canny/build/Debug/");
    strcpy(path2, "/Users/msdoherty/");
    strcpy(path3, "/Users/msdoherty/Desktop/MyDocuments/Davis-Research/Research-Max/3D_Canny/build/Debug/");
    */
  
   // char* xdir = (char*) "experiment[2014-11-28-16-57][MERISTEM80_2_213x170x168_XZ]/fsize9/";
    
    //char* xdir = (char*) "experiment[2014-12-05-12-02][MERISTEM_170x170x200_M23]/fsize9/";
    //char* fdir = (char*) "/Users/msdoherty/Desktop/MyDocuments/Davis-Research/Research-Max/3D_Canny/dataFiles/";


	strcpy(path1, fdir);
	strcpy(path2, fdir);
	strcpy(path3, fdir);
    
    strcat(path1, xdir);
    strcat(path3, xdir);
	
	FILE *fp = fopen(file_name, "r");
	
	if (fp == NULL) {
		cout<<"Error: read NULL pointer found"<<endl; return false; 
	}
	
	while (fgets(data, 300, fp) != NULL) {
		
		if (data[0] == '#' || data[0] == '\0') continue;
		
		switch (data[0]) {
			case 'n': { nf = atoi(data+9); break; }
			case 'F': { strcpy(fn1, data+5); trimTrailingWhitespace(fn1); 
				        strcat(path1, fn1); strcpy(fn1, path1); break; }
			case 'b': { bytes_f1= atoi(data+6); break; }
			case 'i': { sb_f1 = atoi(data+12); break; }
			case 'G': { strcpy(fn2, data+6); trimTrailingWhitespace(fn2); 
				strcat(path2, fn2); strcpy(fn2, path2); break; }
			case 'B': { bytes_f2= atoi(data+6); break; }
			case 'I': { sb_f2 = atoi(data+12); break; }
			case 'Y': { strcpy(fn3, data+6); trimTrailingWhitespace(fn3); 
				strcat(path3, fn3); strcpy(fn3, path3); break; }
			case 'Q': { bytes_f3= atoi(data+7); break; }
			case 'x': { tx = atoi(data+12); break; }
			case 'y': { ty = atoi(data+12); break; }
			case 'z': { tz = atoi(data+12); break; }
			case 'c': { isoVal = atoi(data+13); break; }	
			case 'e': { edr = atoi(data+19); break; }	
			case 'd': { df = atoi(data+15); break; }
			case 'm': { cube_marks = atoi(data+11); break; }
		}
	}
		//file 1
	memcpy(conf.cf_fn1, fn1, 600*sizeof(char));
	conf.cf_numFiles = nf;
	conf.cf_bytes_f1 = bytes_f1;
    conf.cf_sb_f1 = sb_f1;
	
	cout<<"Num Files ="<<nf<<endl;
	cout<<"volume filename (file 1) ="<<fn1<<endl;
	cout<<"bytes size (file 1) = "<<bytes_f1<<endl;
	cout<<"isSwapBytes (file 1) ="<<sb_f1<<endl;
	
		//file 2
	if(nf==2){
		memcpy(conf.cf_fn2, fn2, 600*sizeof(char));
		conf.cf_bytes_f2 = bytes_f2;
		conf.cf_sb_f2 = sb_f2;
		cout<<"volume filename (file 2) ="<<fn2<<endl;
		cout<<"[small, large] = ["<<ts_2<<", "<<tl_2<<"]"<<endl;
		cout<<"bytes size (file 2)= "<<bytes_f2<<endl;
		cout<<"isSwapBytes (file 2)="<<sb_f2<<endl;
	}
		//file 3
	if(nf==3){
		memcpy(conf.cf_fn2, fn2, 600*sizeof(char));
		conf.cf_bytes_f2 = bytes_f2;
		conf.cf_sb_f2 = sb_f2;
		cout<<"volume filename (file 2) ="<<fn2<<endl;
		cout<<"[small, large] = ["<<ts_2<<", "<<tl_2<<"]"<<endl;
		cout<<"bytes size (file 2)= "<<bytes_f2<<endl;
		cout<<"isSwapBytes (file 2)="<<sb_f2<<endl;
		
		memcpy(conf.cf_fn3, fn3, 600*sizeof(char));
		conf.cf_bytes_f3 = bytes_f3;
		cout<<"volume filename (file 3) ="<<fn3<<endl;
		cout<<"bytes size (file 3)= "<<bytes_f3<<endl;
	}
	
	conf.cf_xdim = tx;
	conf.cf_ydim = ty;
	conf.cf_zdim = tz;
	conf.cf_isoVal = isoVal;
	conf.cf_edrOperator = edr;
	conf.cf_densityFilter = df;
	conf.cf_markCube = cube_marks;
	
	conf.cf_cube_xdim = tx-1;
	conf.cf_cube_ydim = ty-1;
	conf.cf_cube_zdim = tz-1;
	
	if(conf.cf_cube_xdim>conf.cf_cube_ydim && conf.cf_cube_xdim>conf.cf_cube_zdim) 
		largest_dim = conf.cf_cube_xdim;
	else if(conf.cf_cube_ydim>conf.cf_cube_xdim && conf.cf_cube_ydim>conf.cf_cube_zdim) 
		largest_dim = conf.cf_cube_ydim;
	else if(conf.cf_cube_zdim>conf.cf_cube_xdim && conf.cf_cube_zdim>conf.cf_cube_ydim)
		largest_dim = conf.cf_cube_zdim;
	else 
		largest_dim = conf.cf_cube_ydim;
		
													// ** MUST USE THE LARGEST DIMENSION
	//preserve aspect ratio							// Not preserve aspect ratio (stretch the data to fit into a unit size cube)
	conf.cf_deltaX = 1.0/largest_dim;				// DELTA_X = 1.0/xdim;  ** deltaX, deltaY and deltaZ must not be zero
	conf.cf_deltaY = 1.0/largest_dim;				// DELTA_Y = 1.0/ydim;	
	conf.cf_deltaZ = 1.0/largest_dim;				// DELTA_Z = 1.0/zdim;
	
	conf.cf_two_deltaX = conf.cf_deltaX*2;
	conf.cf_two_deltaY = conf.cf_deltaY*2;
	conf.cf_two_deltaZ = conf.cf_deltaZ*2;
	
	conf.cf_offsetX = -1*conf.cf_deltaX*(conf.cf_cube_xdim/2);
	conf.cf_offsetY = -1*conf.cf_deltaY*(conf.cf_cube_ydim/2);
	conf.cf_offsetZ = -1*conf.cf_deltaZ*(conf.cf_cube_zdim/2);
	 /*
	conf.cf_offsetX = -1*conf.cf_deltaX*(largest_dim/2);
	conf.cf_offsetY = -1*conf.cf_deltaY*(largest_dim/2);
	conf.cf_offsetZ = -1*conf.cf_deltaZ*(largest_dim/2);
	*/
	cout<<"dimension [x,y,z] =["<<tx<<", "<<ty<<", "<<tz<<"]"<<endl;
	cout<<"isoValue = "<<isoVal<<endl;
	
	cout<<"Density Filter = "<<df<<endl;
	cout<<"CubeMarks = "<<cube_marks<<endl;
	cout<<"-----------------"<<endl;
	
	return true; 
	
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
bool FileManager::readVolumeData(Matrix3D<float>& m3D_Data, Configurations& conf, int whichFile)
{	
	char fname[600]; int bytes=0, isSwapBytes=0;
	switch(whichFile){
		case 1: memcpy(fname, conf.cf_fn1, 600*sizeof(char)); 
				bytes= conf.cf_bytes_f1; 
				//data_size = sizeof(float); 
				isSwapBytes = conf.cf_sb_f1; 
				break;
		case 2: memcpy(fname, conf.cf_fn2, 600*sizeof(char)); 
			    bytes= conf.cf_bytes_f2; 
				//data_size = sizeof(short); 
				isSwapBytes = conf.cf_sb_f2; 
				break;
		case 3: memcpy(fname, conf.cf_fn3, 600*sizeof(char)); 
				bytes= conf.cf_bytes_f3; 
				//data_size = sizeof(char); 
				isSwapBytes = conf.cf_sb_f1; 
				break;
	}

	int n1, n2;
	
	int oneSlice = conf.cf_xdim * conf.cf_ydim;
	float smallest=10000.0, largest=-10000.0, small_hu=10000.0, big_hu=-10000.0;
	float buffer1[oneSlice], buffer2[oneSlice], density=0.0; 
	
	FILE *fp = fopen(fname, "rb");			//*fp is a FILE pointer to the next bit to be read
	if (fp == NULL) return false;

	cout<<endl<<"Read Volume data (file "<<whichFile<<" and byte size:"<<bytes<<")..."<<endl;
	n1 = readBuffer(buffer1, bytes, oneSlice, fp, isSwapBytes);  //n1 & n2 are number of words (oneSlice)
	n2 = readBuffer(buffer2, bytes, oneSlice, fp, isSwapBytes);
	if (n1<oneSlice || n2<oneSlice) return false;  

	int j=0;
	m3D_Data.resize(conf.cf_xdim, conf.cf_ydim, conf.cf_zdim);
	
	for (int z=0; z<conf.cf_zdim; z++){
		for (int y=0; y<conf.cf_ydim; y++){
			for (int x=0; x<conf.cf_xdim; x++) {
				getDensity_fromFile(buffer1[j], density, smallest, largest);
				j++;
				m3D_Data.set(x, y, z, density);					
			}
		}
		memcpy(buffer1, buffer2, oneSlice*sizeof(float));	//destination, source, num bytes
		
		if(z<conf.cf_zdim-2){
			n2 = readBuffer(buffer2, bytes, oneSlice, fp, isSwapBytes);
			if (n2<oneSlice) return false;
		}
		j=0;
	}//end for loop
	
		//m3D_dataPointsData.print(55, 60, 55, 60, 54, 55);
	cout<<"From File: [smallest, largest]= ["<<smallest<<","<<largest<<"]"<<endl;	
	cout<<"Time for Read: "<<time(NULL)-start_time<<" seconds"<<endl;
	cout<<"Volume Size:"<<conf.cf_xdim*conf.cf_ydim*conf.cf_zdim<<endl;
	cout<<"Matrix m3D_dataPoint Size: ["<< m3D_Data.getWidth()<<", ";
	cout<<m3D_Data.getHeight()<<", "<<m3D_Data.getSlice()<<"]"<<endl;
	cout<<"....................."<<endl;
	return true;
}
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
}
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
