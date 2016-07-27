/*
 *  rotate.cpp
 *  Created by Marina Doherty on 2/19/11.
 *
 */

#define TRACE_BACK
#define PI 3.14159265358979323
#define dfile 0

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <math.h> 
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "Rotation.h"
using namespace std;

float mm[16];
int button, state, mousex, mousey, dragX, dragY, cube = 1;
int width = 1000, height = 750, isSpinning, lmax, do_neighbors = 2,
tX = 0, tY = 0, tZ = 0, ttX = 0, ttY = 0, ttZ = 0, just_up = 0, tW = 0, ttW = 0;
double winRadius = 500, dX, dY;
double tt, transfac = .003, rad = 1.2, sumx = 0., sumy = 0., sumz = 0.;
double xxmin, xxmax, ymin, ymax, zmin, zmax, large = 1000000.;
clock_t ticks1, ticks2, ticks3, ticks4;
int iterations = 0, delay = 4000;
int center_x=0, center_y=0, screen_x=0, screen_y=0;
/*
void renderScence();
void renderScenceow();
void renderScencecpw();
void renderScencetw();


void renderSceneAll(){
	glutSetWindow(controlPanel_window);
	glutPostRedisplay();
	glutSetWindow(text_window);
	glutPostRedisplay();
	glutSetWindow(object_window);
	glutPostRedisplay();
}*/

void setProjection(int w1, int h1){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w1, h1);
//	gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 0.1, 60.0);
	glMatrixMode(GL_MODELVIEW);
}
void reshape(int w, int h) {
	width = w;
	height = h;
	if (width > height)
		winRadius = (double)width*.5;
	else
		winRadius = (double)height*.5;
	
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 0.1, 60.0);
	glTranslatef(0., 0., -4.);
	
	gluPerspective(40., width/height, 0.1, 60.);   //set clipping volume
	glTranslatef(0., 0., -4.);
}

void myRotated(double a, double X, double Y, double Z) {
	
	// Multiply on the right instead of the left
	float m[16], sx, sy, sz;
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glLoadIdentity();
	
	glTranslatef(transfac*ttX, transfac*ttY, transfac*ttZ);
	glRotated(a, X, Y, Z);
	glTranslatef(-transfac*ttX, -transfac*ttY, -transfac*ttZ);
	glMultMatrixf(m);
}

void doRotation(double dX, double dY) {
    double offX = 2.0*(double)dragX/(double)width-1.0;
    double offY = 1.0-2.0*(double)dragY/(double)height;
    double offZ = 0.25;
    double X, Y, Z, s, c, t, d, a;
    X = -offZ*dY;
    Y = offZ*dX;
    Z = offX*dY - offY*dX;
    a = 180.*1.5*sqrt(dX*dX + dY*dY)/(PI*winRadius);
    myRotated(a, X, Y, Z);
    return;
}

void vcross(double v[3], double w[3], double a[3]) {
	a[0] = v[1]*w[2] - v[2]*w[1];
	a[1] = v[2]*w[0] - v[0]*w[2];
	a[2] = v[0]*w[1] - v[1]*w[0];
}

void doTranslation(int tX, int tY, int tZ) {
	
	// Multiply on the right instead of the left
	float m[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glLoadIdentity();
	glTranslatef(transfac*tX, transfac*tY, transfac*tZ);
	glMultMatrixf(m);
}
void mouse(int but, int sta, int x, int y) {
	button = but;
	state = sta;
	if(state == GLUT_DOWN) {
		dragX = x;
		dragY = y;
		dX = 0;
		dY = 0;
	}
	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		/*
		 dX = x - dragX;
		 dY = dragY - y;
		 if(dX!=0||dY!=0)
         isSpinning = 1;
		 else
         isSpinning = 0;
		 */
		ticks1 = clock();
		isSpinning = 0;
		just_up = 1;
	}
}

void passive_motion(int x, int y) {
	double t;
	if(just_up) {
		ticks2 = clock();
		t = (double)(ticks2 - ticks1)/(double)CLOCKS_PER_SEC;
		just_up = 0;
		if (t < .01) {
			dX = .2*(x - dragX);
			dY = .2*(dragY - y);
			isSpinning = 1;
		}
	}
	screen_x = x;
	screen_y = y;
	/*
	center_x = (width-300)/2;
	center_y = (height-50)/2;
	cout<<"width="<<width<<", height="<<height<<endl;
	cout<<"center_x="<<center_x<<"center_y="<<center_y<<endl;
	cout<<"xc="<<x<<", yc="<<y<<endl;
	 */
}

void motion(int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		dX = x - dragX;
		dY = dragY - y;
		glMatrixMode(GL_MODELVIEW);
		doRotation(dX, dY);
		dragX = x;
		dragY = y;
	}
	if (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON) {
		tX = x - dragX;
		tY = dragY - y;
		ttX += tX;
		ttY += tY;
		dragX = x;
		dragY = y;
		doTranslation(tX, tY, 0);
	}
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		tZ = x - dragX;
		ttZ += tZ;
		tW = y - dragY;
		ttW += tW;
		dragX = x;
		dragY = y;
		doTranslation(0, 0, tZ);
	}
	ticks3 = clock();
	tt = (double)(ticks3 - ticks4)/(double)CLOCKS_PER_SEC;
}




