/*
 * TextWindow.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: mdoherty
 */

#include <GL/glut.h>
#include "TextWindow.h"
#include "../mc-io/mc1_io.h"
#include "Renderer.h"
#include "Camera.h"
#include "InputProcessor.h"

TextWindow* TextWindow::callback_rcvr = NULL;

TextWindow::TextWindow(int parent_window, int x, int y, int width, int height) {
	if (callback_rcvr == NULL) callback_rcvr = this;

	glut_window = glutCreateSubWindow(parent_window, x, y, width, height);

	glEnable(GL_DEPTH_TEST);
#ifdef USE_GLUI
	// Enable this if controls are added to the text window
	glui_window = GLUI_Master.create_glui_subwindow(glut_window, GLUI_SUBWINDOW_TOP);
	glClearColor(0.8, 0.8, 0.8, 0.0);
	GLUI_Master.set_glutKeyboardFunc(keyboard_callback);
	GLUI_Master.set_glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);
	glutPassiveMotionFunc(passive_motion_callback);

	GLUI_EditText* isovalue_text = new GLUI_EditText(glui_window, "isovalue:", &(HEADER.isoVal));
	isovalue_text->disable();
	new GLUI_Column(glui_window);
	mouse_x_text = new GLUI_EditText(glui_window, "mouse x:", &ui_common.mouse_x);
	mouse_y_text = new GLUI_EditText(glui_window, "mouse y:", &ui_common.mouse_y);
	mouse_x_text->disable();
	mouse_y_text->disable();
	new GLUI_Column(glui_window);
	new GLUI_Button(glui_window, "quit", 0, (GLUI_Update_CB)exit);

#else
	glClearColor(0.3, 0.6, 0.1, 0.0);
	glutKeyboardFunc(keyboard_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);
	glutPassiveMotionFunc(passive_motion_callback);
#endif
	glutDisplayFunc(display_callback);
}

void TextWindow::reshape(int x, int y, int w, int h) {
	glutSetWindow(glut_window);
	Renderer::setProjection(x, y, w, h);
}

void TextWindow::display(){
	glutSetWindow(glut_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef USE_MULTISAMPLING
    glEnable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_MULTISAMPLE_ARB);
#endif

#ifdef USE_GLUI
    mouse_x_text->set_int_val(ui_common.mouse_x);
	mouse_y_text->set_int_val(ui_common.mouse_y);
#endif
	glutSwapBuffers();
	glFlush();
}

void TextWindow::mouse(int button, int state, int x, int y) {
}

void TextWindow::motion(int x, int y) {
}

void TextWindow::passive_motion(int x, int y) {
}

void TextWindow::keyboard(unsigned char key, int x, int y) {
}
