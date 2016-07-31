/*
 *  Camera.cpp
 *  Created by Marina Doherty on 7/27/16.
 *  Replaces Rotation.cpp (3/11/11)
 */

#include <iostream>
#include <cmath>
#include <stdio.h>
using namespace std;
#include <GL/glut.h>

#include "Camera.h"
#include "../MyMath.h"

Camera::Camera(int _window) :
		camera_window(_window),
		width(0), height(0), transfac(0.003),
		ttX(0), ttY(0), ttZ(0), winRadius(500) {
	for (short i=0; i<10; i++) saved_camera[i] = NULL;
}

// **** moving and resizing the camera ******

// reshape is called to inform camera that window has been reshaped
void Camera::reshape(int _width, int _height) {
	width = _width; height = _height;
	if (width > height) winRadius = (double)width*.5;
	else winRadius = (double)height*.5;
}

// rotate image at its center, angle degrees around axis <X,Y,Z>
void Camera::doRotationAngleAxis(double angle, double X, double Y, double Z) {
	glutSetWindow(camera_window);
	// Multiply on the right instead of the left
	float m[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glLoadIdentity();

	glTranslatef(transfac*ttX, transfac*ttY, transfac*ttZ);
	glRotated(angle, X, Y, Z);
	glTranslatef(-transfac*ttX, -transfac*ttY, -transfac*ttZ);
	glMultMatrixf(m);
}

// rotate image at its center, in a manner consistent with 2D screen vector <dX,dY>
void Camera::doRotationScreenVector(double dX, double dY) {
	glutSetWindow(camera_window);
    double offX = 0.0; //2.0*(double)dragX/(double)width-1.0;
    double offY = 0.0; //1.0-2.0*(double)dragY/(double)height;
    double offZ = 1.0; //0.25;
    double X, Y, Z, a;
    X = -offZ*dY;
    Y = offZ*dX;
    Z = offX*dY - offY*dX;
    a = 180.*1.5*sqrt(dX*dX + dY*dY)/(M_PI*winRadius);
    doRotationAngleAxis(a, X, Y, Z);
    return;
}

// move the image in the direction indicated by vector <tX,tY,tZ>
void Camera::doTranslation(int tX, int tY, int tZ) {
	// store accumulated translation
	ttX += tX; ttY += tY; ttZ += tZ;
	// do incremental translation
	glutSetWindow(camera_window);
	// multiply on the right instead of the left
	float m[16];
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glLoadIdentity();
	glTranslatef(transfac*tX, transfac*tY, transfac*tZ);
	glMultMatrixf(m);
}


// **** saving and restoring camera states ******

struct SavedCameraState
{
	float mvm[16]; // model-view matrix (extracted from openGL)
	int ttX, ttY, ttZ; // accumulated translation
};

SavedCameraState* Camera::saveCamera()
{
	SavedCameraState* scs = new SavedCameraState;
	scs->ttX = ttX;
	scs->ttY = ttY;
	scs->ttZ = ttZ;
	glutSetWindow(camera_window);
	glGetFloatv(GL_MODELVIEW_MATRIX, scs->mvm);
	return scs;
}

void Camera::restoreCamera(SavedCameraState* scs)
{
	ttX = scs->ttX;
	ttY = scs->ttY;
	ttZ = scs->ttZ;
	glutSetWindow(camera_window);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(scs->mvm);
}

bool Camera::saveState(int i) {
	if ((i<0)||(i>9)) return false;
	if (saved_camera[i] != NULL) delete saved_camera[i];
	saved_camera[i] = saveCamera();
	return true;
}

bool Camera::restoreState(int i) {
	if ((i<0)||(i>9)) return false;
	if (saved_camera[i] == NULL) return false;
	restoreCamera(saved_camera[i]);
	return true;
}

void Camera::writeCameraStatesToFile()
{
	int i, j;
	FILE* fp;
	fp = fopen("SavedCameraState.txt", "w");
	if (fp == NULL) { cerr << "Cannot write saved camera states\n"; return; }
	for (i=0; i<10; i++)
	{
		if (saved_camera[i] == NULL) fprintf(fp, "0\n");
		else
		{
			fprintf(fp, "1 ");
			fprintf(fp, "%d %d %d ", saved_camera[i]->ttX, saved_camera[i]->ttY, saved_camera[i]->ttZ);
			for (j=0; j<16; j++) fprintf(fp, "%f ", saved_camera[i]->mvm[j]);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
}

void Camera::readCameraStatesFromFile()
{
	int i, j, k;
	FILE* fp;
	fp = fopen("SavedCameraState.txt", "r");
	if (fp == NULL) { cerr << "Cannot read saved camera states\n"; return; }
	for (i=0; i<10; i++)
	{
		char line[300];
		fgets(line, 299, fp);
		if (saved_camera[i] != NULL) { delete saved_camera[i]; saved_camera[i] = NULL; }
		if (line[0] != '0')
		{
			saved_camera[i] =  new SavedCameraState;
			j = 0;
			while (line[j] != ' ') j++; while (line[j] == ' ') j++;
			saved_camera[i]->ttX = atoi(&(line[j]));
			while (line[j] != ' ') j++; while (line[j] == ' ') j++;
			saved_camera[i]->ttY = atoi(&(line[j]));
			while (line[j] != ' ') j++; while (line[j] == ' ') j++;
			saved_camera[i]->ttZ = atoi(&(line[j]));
			for (k=0; k<16; k++)
			{
				while (line[j] != ' ') j++; while (line[j] == ' ') j++;
				saved_camera[i]->mvm[k] = atof(&(line[j]));
			}
		}
	}
	fclose(fp);
}

bool Camera::restoreStateAvailable(int i) {
	if ((i<0) || (i>=10)) return false;
	return saved_camera[i] != NULL;
}
