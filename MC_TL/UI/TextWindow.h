/*
 * TextWindow.h
 *
 *  Created on: Aug 6, 2016
 *      Author: mdoherty
 */

#ifndef UI_TEXTWINDOW_H_
#define UI_TEXTWINDOW_H_

#include "UICommon.h"
#ifdef USE_GLUI
#include <GL/glui.h>
#endif

class Camera;
class InputProcessor;

class TextWindow {
public:
	TextWindow(int parent_window, int x, int y, int width, int height);
	void display();
	void reshape(int x, int y, int w, int h);
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void passive_motion(int x, int y);
	void keyboard(unsigned char key, int x, int y);

	int getGlutWindow() { return glut_window; }
#ifdef USE_GLUI
	GLUI *getGluiWindow() { return glui_window; }
#endif

	void attachCamera(Camera* _camera) { camera = _camera; }
	void attachInputProcessor(InputProcessor* _input_processor) { input_processor = _input_processor; }

private:
	int glut_window;
#ifdef USE_GLUI
	GLUI *glui_window;
	GLUI_EditText* mouse_x_text;
	GLUI_EditText* mouse_y_text;
#endif

	Camera* camera;
	InputProcessor* input_processor;

	// static callback functions for GLUT
	static TextWindow* callback_rcvr;
	static void mouse_callback(int button, int state, int x, int y)
		{ callback_rcvr->mouse(button,state,x,y); }
	static void motion_callback(int x, int y)
		{ callback_rcvr->motion(x,y); }
	static void passive_motion_callback(int x, int y)
		{ callback_rcvr->passive_motion(x,y); }
	static void keyboard_callback(unsigned char key, int x, int y)
		{ callback_rcvr->keyboard(key,x,y);	}
	static void display_callback()
		{ callback_rcvr->display();	}
};

#endif /* UI_TEXTWINDOW_H_ */
