#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <GL/glut.h>
#include "MarchingCubes.h"
#include "Rotation.h"
#include "LightManager.h"
#include "Configurations.h"
#include "EdgeDetectionOperators.h"
#include "Rotation.h"

#include <iostream>
#include "Point.h"
#include "Face.h"
#include "show_text.h"
using namespace std;
	//#include "gui/sliderWindow.h"
	//#include <QtGui>

extern void init();
extern void idle();
extern void display();
extern void renderSceneow();
extern void renderScenecpw();
extern void renderScenetw();
extern void displayLines();
extern void displayFaces();
extern void renderWireFrame(float borderwidth, float r, float g, float b, float a);

extern int main_window, object_window, controlPanel_window, text_window;
#endif
