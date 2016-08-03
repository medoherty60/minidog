#include "ReadFile.h"


// format: 1=float, 2=short
int ReadFile::readBuffer(float* buffer, char bytes, int num_words, FILE* fp, char isSwapBytes)
{
	int n=0;
	switch(bytes){
		// for char format, read first into a temporary buffer
		case 1:    { unsigned char* cbuffer = new unsigned char[num_words];
					 n = fread(cbuffer, bytes, num_words, fp);
			
					 // copy to actual buffer and convert to floats
					 for (int i=0; i<n; i++) 
						buffer[i] = float(cbuffer[i]);
			
					 delete [] cbuffer;
					 return n;
					 break;}
			
		// for short format, read first into a temporary buffer
		case 2:		{ short* sbuffer = new short[num_words];
					n = fread(sbuffer, bytes, num_words, fp);
			
					if(isSwapBytes) 
						for (int i=0; i<n; i++)
							// REPLACE THIS WITH A TWO BYTE SWAP
							sbuffer[i] = swapBytes(sbuffer[i]);
			
					// copy to actual buffer and convert to floats
					for (int i=0; i<n; i++) 
						buffer[i] = float(sbuffer[i]);
			
					delete sbuffer;
					return n;
					break;
					}
		// for unsigned short format, read first into a temporary buffer
		case 3:		{ unsigned short* sbuffer = new unsigned short[num_words];
			n = fread(sbuffer, 2, num_words, fp);
			
			if(isSwapBytes) 
				for (int i=0; i<n; i++)
						// REPLACE THIS WITH A TWO BYTE SWAP
					sbuffer[i] = swapBytes(sbuffer[i]);
			
				// copy to actual buffer and convert to floats
			for (int i=0; i<n; i++) 
				buffer[i] = float(sbuffer[i]);
			
			delete sbuffer;
			return n;
			break;
		}
			
		// for float format, read directly into the passed buffer
		case 4:		{ n = fread(buffer, bytes, num_words, fp);
					if(isSwapBytes) 
						for (int i=0; i<n; i++)
							buffer[i] = swapBytes(buffer[i]);
					return n;
					break; }
				
		default: cout<<"Error: Data format not supported"<<endl; return 0;
	}
}
unsigned short ReadFile::swapBytes(unsigned short val){
	
	swapUnion3 su;
	su.x = val;
	unsigned char tmp=su.c[0];
	su.c[0] = su.c[1];
	su.c[1] = tmp;
	return su.x;
}
short ReadFile::swapBytes(short val){
	
	swapUnion2 su;
	su.x = val;
	unsigned char tmp=su.c[0];
	su.c[0] = su.c[1];
	su.c[1] = tmp;
	return su.x;
}
float ReadFile::swapBytes(float val){
	
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