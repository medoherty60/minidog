#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H
/*
 * InputProcessor.h
 *
 *  Created on: Jul 30, 2016
 *      Author: Marina Doherty
 */
#include <cstdlib>
#include "UICommon.h"
class Camera;

class InputProcessor {
public:
	InputProcessor(Camera* _camera=NULL);

	// mouse is called when a mouse button is pressed or released
	void mouse(int button, int state, int x, int y);
	// motion is called when mouse is moved with a button pressed
	void motion(int x, int y);
	// passive_motion is called when mouse is moved with no button pressed
	void passive_motion(int x, int y);
	// keyboard is called when an ASCII key is pressed
	void keyboard(unsigned char key, int x, int y);

private:
	Camera* camera;

	int button_state[3]; 			// current state (up or down) of each mouse button
	int mouse_x, mouse_y;			// current mouse location (window coordinates)
	int prev_mouse_x, prev_mouse_y; // previous mouse location (window coordinates)

	// saved state for multi-character keyboard commands
	bool save_requested;
	bool restore_requested;

	void shareMouseData() { ui_common.mouse_x = mouse_x; ui_common.mouse_y = mouse_y; }
};

#endif /* INPUTPROCESSOR_H */
