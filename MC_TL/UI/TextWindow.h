/*
 * TextWindow.h
 *
 *  Created on: Aug 6, 2016
 *      Author: mdoherty
 */

#ifndef UI_TEXTWINDOW_H_
#define UI_TEXTWINDOW_H_

#include "UIConfiguration.h"

class TextWindow {
public:
	void initTW(int parent_window, int x, int y, int width, int height);
	void renderSceneTW();
	void mouseTW(int button, int state, int x, int y);
	void motionTW(int x, int y);
	void passive_motionTW(int x, int y);
	void keyboardTW(unsigned char key, int x, int y);

	int getGlutWindow() { return glut_window; }
#ifdef USE_GLUI
	GLUI *getGluiWindow() { return glui_window; }
#endif
private:
	int glut_window;
#ifdef USE_GLUI
	GLUI *glui_window;
#endif
};

#endif /* UI_TEXTWINDOW_H_ */
