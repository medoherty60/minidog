/*
 * data_generator.cpp
 * This program generates discrete 3D images (volumes) based
 * on implicit surface definitions of common 3D objects.
 */

// Just a comment added on July 26 to test git.

// If compiling for Microsoft Visual C++, turn off the warnings
// about insecure C library functions.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 
#endif

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

#include "ImplicitSurface.h"

static const float PI = M_PI;
static const float TWO_PI = PI*2.0f;
static const float HALF_PI = PI/2.0f;
static const float QUARTER_PI = PI/4.0f;

// If compiling for Windows, enable byte swap when storing results to file.
#ifdef _MSC_VER

const bool need_byte_swap = true;
union swapstruct_short { short x; char c[2]; };
void swapbytes_short(short& x) {
	swapstruct_short ss;
	ss.x = x;
	char tmp = ss.c[0];
	ss.c[0] = ss.c[1];
	ss.c[1] = tmp;
	x = ss.x;
}

#else

const bool need_byte_swap = false;
void swapbytes_short(short& x) { }

#endif

//===============================================

enum shape_t { SPHERE, CUBE, CONE, COMPOSITE };
string shapeName[4] = {"sphere", "cube", "cone", "composite"};

enum field_t { SOLID, DISTANCE };
string fieldName[2] = {"solid", "distance"};

struct vol3D {
	long width, height, slice, datasize;
    float h;
    vol3D(long _width=150, long _height=150, long _slice=150, float _h=1.0) :
    	width(_width), height(_height), slice(_slice), h(_h)
    { datasize = width*height*slice; }
};

//===============================================

ImplicitSurface* makeSphere1(vol3D& v)
{
	float cx = v.width/2.0f, cy = v.height/2.0f, cz = v.slice/2.0f;
	ImplicitSurface* obj = new Sphere(cx,cy,cz, 0,0,0, 50,50,50);
	return obj;
}

ImplicitSurface* makeCube1(vol3D& v)
{
	float cx = v.width/2.0f, cy = v.height/2.0f, cz = v.slice/2.0f;
	ImplicitSurface* obj = new Cube(cx,cy,cz, 0,0,0.3, 50,30,50);
	return obj;
}

ImplicitSurface* makeCone1(vol3D& v)
{
	float cx = v.width/2.0f, cy = v.height/2.0f, cz = v.slice/2.0f;
	ImplicitSurface* obj = new Cone(cx,cy,cz, HALF_PI,0,0, 10,10,50);
	return obj;
}

ImplicitSurface* makeComposite1()
{
	CompoundObject* cobj = new CompoundObject;
	cobj->addObject(new Sphere(50,75,70, 0,0,0, 40,40,40));
	cobj->addObject(new Cube(80,50,70, 0,0,0.4, 30,30,30));
	return cobj;
}

ImplicitSurface* makeComposite2()
{
	CompoundObject* cobj = new CompoundObject;
	cobj->addObject(new Sphere(75,150,75, 0,0,0, 60,30,60));
	cobj->addObject(new Cone(75,75,75, -HALF_PI,0,0, 10,10,50));
	cobj->addObject(new Cone(125,75,75, -3.0*PI/8.0,-HALF_PI,0, 6,6,80));
	return cobj;
}


//===============================================

int main()
{
    vol3D v;
    ImplicitSurface* obj = NULL;
    shape_t shape_type(SPHERE);
    field_t field_type(SOLID);

   	switch (shape_type) {
   		case SPHERE:
   			obj = makeSphere1(v);
   			break;
   		case CUBE:
   			obj = makeCube1(v);
   			break;
   		case CONE:
   			obj = makeCone1(v);
   			break;
   		case COMPOSITE:
   			obj = makeComposite2();
   			break;
   	}

    if (obj == NULL) {
    	cout << "failed to create object - aborting" << endl;
    	exit(1);
    }

	short* data = new short[v.datasize];
	long i = 0;

	for(long z=0; z<v.slice; z+=v.h){
		for(long y=0; y<v.height; y+=v.h){
			for(long x=0; x<v.width; x+=v.h){
				switch (field_type)
				{
					case SOLID:
						data[i] = obj->solidObject(x,y,z);
						break;
					case DISTANCE:
						data[i] = obj->distanceField(x,y,z);
						break;
					default:
						data[i] = 0;
						break;
				}
				if (need_byte_swap)	swapbytes_short(data[i]);
				i++;
			}
		}
	}
	char fname[200];
	sprintf(fname, "%s_%s_volume_%ldx%ldx%ld.raw", shapeName[shape_type].c_str(),
			fieldName[field_type].c_str(), v.width, v.height, v.slice);

	FILE *fp = fopen(fname, "wb");
	fwrite(data, sizeof(short), v.datasize, fp);
	fclose(fp);


	float* fdata = new float[v.datasize];
	for (i=0; i<v.datasize; i++) fdata[i] = float(data[i]);
	char fname2[200];
	sprintf(fname2, "%s_%s_volume_float_%ldx%ldx%ld.raw", shapeName[shape_type].c_str(),
			fieldName[field_type].c_str(), v.width, v.height, v.slice);
	FILE *fp2 = fopen(fname2, "wb");
	fwrite(fdata, sizeof(float), v.datasize, fp2);
	fclose(fp2);

	delete [] data;
	return 0;
}
