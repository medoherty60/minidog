#ifndef ROTATE_H
#define ROTATE_H
/*
 *  rotate.h
 *  Created by Marina Doherty on 3/11/11.
 *
 */
#include <GL/glut.h>
/*
class Rotation{

public: Rotation(){

	mm[16];
	cube = 1;
	width = 1200; height = 900;
	do_neighbors = 2,
	tX = 0; tY = 0; tZ = 0; 
	ttX = 0; ttY = 0; ttZ = 0; 
	just_up = 0; tW = 0; ttW = 0;
	sumx = 0., sumy = 0., sumz = 0.;
	winRadius = 500;
	transfac = .003, rad = 1.2, 
	large = 1000000.; iterations = 0; delay = 4000;
	center_x=0; center_y=0; screen_x=0; screen_y=0;
}
*/
void doRotation(double dX, double dY);
void myRotated(double a, double X, double Y, double Z);
void doTranslation(int tX, int tY, int tZ);
void vcross(double v[3], double w[3], double a[3]);
void mouse(int but, int sta, int x, int y);
void motion(int x, int y);
void passive_motion(int x, int y);
void reshape(int w, int h);
void renderSceneAll();
void setProjection(int w1, int h1);
	
	//int getWidth(){ return width; }
	//int getHeight(){ return height; }
/*
private:
	
float mm[16];
int button, state, mousex, mousey, dragX, dragY, cube;
int width, height, isSpinning, lmax, do_neighbors,
	       tX, tY, tZ, ttX, ttY, ttZ, just_up, tW, ttW;
double winRadius, dX, dY;
double tt, transfac, rad, sumx, sumy, sumz;
double xxmin, xxmax, ymin, ymax, zmin, zmax, large;
clock_t ticks1, ticks2, ticks3, ticks4;
int iterations, delay;
float center_x, center_y, screen_x, screen_y;
 */
	
 extern float mm[16];
 extern int button, state, mousex, mousey, dragX, dragY, cube;
 extern int width, height, isSpinning, lmax, do_neighbors,
tX, tY, tZ, ttX, ttY, ttZ, just_up, tW, ttW;
 extern double winRadius, dX, dY;
 extern double tt, transfac, rad, sumx, sumy, sumz;
 extern double xxmin, xxmax, ymin, ymax, zmin, zmax, large;
 extern clock_t ticks1, ticks2, ticks3, ticks4;
 extern int iterations, delay;
 extern int center_x, center_y, screen_x, screen_y;


#endif


