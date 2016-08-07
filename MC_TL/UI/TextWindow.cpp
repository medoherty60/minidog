/*
 * TextWindow.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: mdoherty
 */

#include <GL/glut.h>
#include "TextWindow.h"

//Display func for text Window
void TextWindow::renderSceneTW(){
	glutSetWindow(text_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef USE_MULTISAMPLING
    glEnable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_MULTISAMPLE_ARB);
#endif
	//char s0[1000];
	//sprintf(s0,"Put text here ...");
	//renderString(-0.5,0.0,s0);

	glutSwapBuffers();
	glFlush();
}

void TextWindow::initTW(int parent_window, int x, int y, int width, int height){
	glut_window = glutCreateSubWindow(parent_window, x, y, width, height);

	glEnable(GL_DEPTH_TEST);
#ifdef USE_GLUI
	// Enable this if controls are added to the text window
	//gluiTW = GLUI_Master.create_glui_subwindow(text_window, GLUI_SUBWINDOW_TOP);
	//GLUI_Master.set_glutKeyboardFunc(keyboardTW);
	//GLUI_Master.set_glutMouseFunc(mouseTW);
	//glutMotionFunc(motionTW);
	//glutPassiveMotionFunc(passive_motionTW);
	//new GLUI_Button(gluiTW, "Quit", 0, (GLUI_Update_CB)exit);
	glClearColor(0.8, 0.8, 0.8, 0.0);
#else
	glClearColor(0.3, 0.6, 0.1, 0.0);
#endif
	glutDisplayFunc(renderSceneTW);
	glutKeyboardFunc(keyboardTW);
	glutMouseFunc(mouseTW);
	glutMotionFunc(motionTW);
	glutPassiveMotionFunc(passive_motionTW);
}

void TextWindow::mouseTW(int button, int state, int x, int y) {
}

}void TextWindow::motionTW(int x, int y) {
}

void TextWindow::passive_motionTW(int x, int y) {
}

void TextWindow::keyboardTW(unsigned char key, int x, int y) {
}
