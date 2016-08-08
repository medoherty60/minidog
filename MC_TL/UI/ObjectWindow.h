/*
 * ObjectWindow.h
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

#ifndef UI_OBJECTWINDOW_H_
#define UI_OBJECTWINDOW_H_

#include "UICommon.h"
#ifdef USE_GLUI
#include <GL/glui.h>
#endif

class Camera;
class InputProcessor;
class Renderer;

class ObjectWindow {

public:

	ObjectWindow(int parent_window, int x, int y, int width, int height);
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

	void attachCamera(Camera* _camera);
	void attachInputProcessor(InputProcessor* _input_processor) { input_processor = _input_processor; }
	void attachRenderer(Renderer* _renderer) { renderer = _renderer; }
private:

	int glut_window;
#ifdef USE_GLUI
	GLUI *glui_window;
#endif

	Camera* camera;
	InputProcessor* input_processor;
	Renderer* renderer;

	// animation controls
	clock_t last_frame_time;
	float elapsed_frame_time;
	bool first_animation_frame;
	bool enable_animation;
	int animation_face_code;

	void resetAnimation() { animation_face_code = 0; }
	void updateAnimation();

	static ObjectWindow* callback_rcvr;
	static void mouse_callback(int button, int state, int x, int y) {
		if (callback_rcvr != NULL) callback_rcvr->mouse(button,state,x,y);
	}
	static void motion_callback(int x, int y) {
		if (callback_rcvr != NULL) callback_rcvr->motion(x,y);
	}
	static void passive_motion_callback(int x, int y) {
		if (callback_rcvr != NULL) callback_rcvr->passive_motion(x,y);
	}
	static void keyboard_callback(unsigned char key, int x, int y) {
		if (callback_rcvr != NULL) callback_rcvr->keyboard(key,x,y);
	}
	static void display_callback() {
		if (callback_rcvr != NULL) callback_rcvr->display();
	}
};

#endif /* UI_OBJECTWINDOW_H_ */
