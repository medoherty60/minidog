/*
 * InputProcessor.cpp
 *
 *  Created on: Jul 30, 2016
 *      Author: Marina Doherty
 */
#include "UI/InputProcessor.h"

#include <GL/glut.h>
#include "UI/Camera.h"

InputProcessor::InputProcessor(Camera* _camera) :
	camera(_camera),
	mouse_x(0), mouse_y(0), prev_mouse_x(0), prev_mouse_y(0),
	save_requested(false), restore_requested(false) {
	for (short i=0; i<3; i++) button_state[i] = GLUT_UP;
}

// mouse is called when a mouse button is pressed or released
void InputProcessor::mouse(int button, int state, int x, int y) {
	button_state[button] = state;
	if(state == GLUT_DOWN) {
		prev_mouse_x = x;
		prev_mouse_y = y;
	}
	mouse_x = x;
	mouse_y = y;
}

// motion is called when mouse is with a button pressed
void InputProcessor::motion(int x, int y) {
	if (button_state[GLUT_LEFT_BUTTON] == GLUT_DOWN) {
		double dX = x - prev_mouse_x;
		double dY = prev_mouse_y - y;
		prev_mouse_x = x; prev_mouse_y = y;
		if (camera != NULL)	camera->doRotationScreenVector(dX, dY);
	}
	if (button_state[GLUT_MIDDLE_BUTTON] == GLUT_DOWN) {
		int tX = x - prev_mouse_x;
		int tY = prev_mouse_y - y;
		prev_mouse_x = x; prev_mouse_y = y;
		if (camera != NULL)	camera->doTranslation(tX, tY, 0);
	}
	if (button_state[GLUT_RIGHT_BUTTON] == GLUT_DOWN) {
		int tZ = x - prev_mouse_x;
		prev_mouse_x = x; prev_mouse_y = y;
		if (camera != NULL)	camera->doTranslation(0, 0, tZ);
	}
	mouse_x = x; mouse_y = y;
}

// passive_motion is called when mouse is moved with no button pressed
void InputProcessor::passive_motion(int x, int y) {
	mouse_x = x; mouse_y = y;
}

// keyboard is called when an ASCII key is pressed
void InputProcessor::keyboard(unsigned char key, int x, int y)
{
	if ((key=='s')||(key=='S'))
	{
		save_requested = true;
		restore_requested = false;
	}
	else if ((key=='r')||(key=='R'))
	{
		restore_requested = true;
		save_requested = false;
	}
	else if ((key>='0')&&(key<='9'))
	{
		int index = int(key-'0');
		if (save_requested) {
			if (camera != NULL) camera->saveState(index);
		}
		else if (restore_requested)	{
			if (camera != NULL) camera->restoreState(index);
		}
		restore_requested = false;
		save_requested = false;
	}
	else if ((key=='w')||(key=='W'))
	{
		if (camera != NULL) camera->writeCameraStatesToFile();
	}
	else if ((key=='a')||(key=='A'))
	{
		if (camera != NULL) camera->readCameraStatesFromFile();
	}
	else if ((key=='n')||(key=='N'))
	{
		if (camera != NULL) camera->doTranslation(0, 0, 10);
	}
	else if ((key=='m')||(key=='M'))
	{
		if (camera != NULL) camera->doTranslation(0, 0, -10);
	}
	else if ((key=='k')||(key=='K'))
	{
		if (camera != NULL) camera->doTranslation(10, 0, 0);
	}
	else if ((key=='j')||(key=='J'))
	{
		if (camera != NULL) camera->doTranslation(-10, 0, 0);
	}
	else if ((key=='i')||(key=='I'))
	{
		if (camera != NULL) camera->doTranslation(0, 10, 0);
	}
	else if ((key=='o')||(key=='O'))
	{
		if (camera != NULL) camera->doTranslation(0, -10, 0);
	}
	else if ((key=='h')||(key=='H'))
	{
		if (camera != NULL) camera->doRotationScreenVector(10, 0);
	}
	else if ((key=='g')||(key=='G'))
	{
		if (camera != NULL) camera->doRotationScreenVector(-10, 0);
	}
	else if ((key=='y')||(key=='Y'))
	{
		if (camera != NULL) camera->doRotationScreenVector(0, 10);
	}
	else if ((key=='u')||(key=='U'))
	{
		if (camera != NULL) camera->doRotationScreenVector(0, -10);
	}
}





