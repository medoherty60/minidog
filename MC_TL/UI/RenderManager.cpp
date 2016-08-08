/*
 * RendererManager.cpp
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */
#include <GL/glut.h>
#include "RenderManager.h"
#include "ObjectWindow.h"
#include "ControlWindow.h"
#include "TextWindow.h"
#include "Camera.h"
#include "InputProcessor.h"
#include "Renderer.h"

int RenderManager::main_window=0;
ObjectWindow* RenderManager::object_window = NULL;
ControlWindow* RenderManager::control_window = NULL;
TextWindow* RenderManager::text_window = NULL;

// initialize sets everything up so that we're ready to enter the glut loop.
void RenderManager::initialize(int* pargc, char** argv) {

	glutInit(pargc, argv);
#ifdef USE_MULTISAMPLING
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
#endif
	glutInitWindowSize(1000,750);
	glutInitWindowPosition(100, 50);
	main_window = glutCreateWindow("Volume Visualization using - Marina Doherty ( TL:MC July, 2013)");

	//Register callback routines for main window
	//glutDisplayFunc(RenderManager::displayFaces);
#ifdef USE_GLUI
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutIdleFunc(idle);
#else
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
#endif

	// set up the sub-windows
	object_window = new ObjectWindow(main_window, 0, 0, 700, 700);

	Camera* camera = new Camera(object_window->getGlutWindow());
	InputProcessor* input_processor = new InputProcessor(camera);
	Renderer* renderer = new Renderer();

	object_window->attachCamera(camera);
	object_window->attachInputProcessor(input_processor);
	object_window->attachRenderer(renderer);

	control_window = new ControlWindow(main_window, 700, 0, 300, 750);
	control_window->attachCamera(camera);
	control_window->attachInputProcessor(input_processor);
	control_window->attachRenderer(renderer);

	text_window = new TextWindow(main_window, 0, 700, 700, 50);
	text_window->attachCamera(camera);
	text_window->attachInputProcessor(input_processor);
	text_window->attachRenderer(renderer);
}

// idle() is called by GLUT when nothing is happening
void RenderManager::idle(void){
	if (control_window != NULL) {
		glutSetWindow(control_window->getGlutWindow());
		glutPostRedisplay();
	}
	if (text_window != NULL) {
		glutSetWindow(text_window->getGlutWindow());
		glutPostRedisplay();
	}
	if (object_window != NULL) {
		glutSetWindow(object_window->getGlutWindow());
		glutPostRedisplay();
	}
}

// reshape is called by GLUT when window is reshaped
void RenderManager::reshape(int w, int h){

	// determine dimensions of each sub-window, relative to the full window

	// object window
	int ow_x = 0;
	int ow_y = 0;
	int ow_w = 0.7*w;
	int ow_h = 0.93*h;
	// control panel
	int cp_x = ow_x + ow_w;
	int cp_y = ow_y;
	int cp_w = w - ow_w;
	int cp_h = h;
	// text window
	int tw_x = ow_x;
	int tw_y = ow_y + ow_h;
	int tw_w = ow_w;
	int tw_h = h - ow_h;

	// reposition and resize each sub-window.
	if (object_window != NULL) object_window->reshape(ow_x, ow_y, ow_w, ow_h);
	if (control_window != NULL) control_window->reshape(cp_x, cp_y, cp_w, cp_h);
	if (text_window != NULL) text_window->reshape(tw_x, tw_y, tw_w, tw_h);
}
