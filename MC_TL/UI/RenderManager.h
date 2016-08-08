/*
 * RendererManager.h
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

#ifndef UI_RENDERMANAGER_H
#define UI_RENDERMANAGER_H

#include "UICommon.h"
class TextWindow;
class ControlWindow;
class ObjectWindow;

class RenderManager {
public:
	// initialize sets everything up so that we're ready to enter the glut loop.
	static void initialize(int* pargc, char** argv);

private:
	static int main_window;
	static ObjectWindow* object_window;
	static ControlWindow* control_window;
	static TextWindow* text_window;

	// idle() is called by GLUT when nothing is happening
	static void idle();
	// reshape() is called by GLUT when window is reshaped
	static void reshape(int w, int h);

	// private constructor to prevent instances of this class.
	RenderManager() { }
};

#endif
