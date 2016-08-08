/*
 * ControlWindow.cpp
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

#include <GL/glut.h>
#include "../mc-io/mc1_io.h"
#include "ControlWindow.h"
#include "Camera.h"
#include "InputProcessor.h"
#include "Renderer.h"

ControlWindow* ControlWindow::callback_rcvr = NULL;

Camera* camera = NULL;

ControlWindow::ControlWindow(int parent_window, int x, int y, int width, int height){

	camera = NULL;
	input_processor = NULL;

	if (callback_rcvr == NULL) callback_rcvr = this;

	glut_window = glutCreateSubWindow(parent_window, x, y, width, height);
	glutDisplayFunc(display_callback);
	glEnable(GL_DEPTH_TEST);
#ifdef USE_GLUI
	glui_window = GLUI_Master.create_glui_subwindow(glut_window, GLUI_SUBWINDOW_TOP);
	GLUI_Master.set_glutKeyboardFunc(keyboard_callback);
	GLUI_Master.set_glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);
	glutPassiveMotionFunc(passive_motion_callback);

	GLUI_Panel* panel1 = new GLUI_Panel(glui_window, "", GLUI_PANEL_NONE);

	new GLUI_Checkbox( panel1, "Black background", &ui_common.blackBG );
	new GLUI_Checkbox( panel1, "Show bounding cube", &ui_common.showCube );
	new GLUI_Checkbox( panel1, "Show coordinate axes", &ui_common.showAxes );

	GLUI_Panel* panel2 = new GLUI_Panel(glui_window, "", GLUI_PANEL_NONE);
	new GLUI_Button(panel2, "Load Camera Settings", 1, (GLUI_Update_CB)camera_button_callback);
	for (short i=0; i<10; i++) {
		char s[100];
		sprintf(s, "restore %d",i);
		camera_restore_buttons[i] = new GLUI_Button(panel2, s, 100+i, (GLUI_Update_CB)camera_button_callback);
	}
	new GLUI_Column(panel2);
	new GLUI_Button(panel2, "Save Camera Settings", 2, (GLUI_Update_CB)camera_button_callback);
	for (short i=0; i<10; i++) {
		char s[100];
		sprintf(s, "save %d",i);
		new GLUI_Button(panel2, s, 200+i, (GLUI_Update_CB)camera_button_callback);
	}

	// ----- Marker Control -----
	ui_common.marker_enabled = true;
	marker_incr = 10;

	GLUI_Panel* panel3 = new GLUI_Panel(glui_window, "", GLUI_PANEL_NONE);
	new GLUI_StaticText( panel3, "Marker:" );
	marker_x_text = new GLUI_EditText(panel3, "x", &ui_common.marker_x);
	marker_y_text = new GLUI_EditText(panel3, "y", &ui_common.marker_y);
	marker_z_text = new GLUI_EditText(panel3, "z", &ui_common.marker_z);
	marker_incr_text = new GLUI_EditText(panel3, "incr", &marker_incr);
	new GLUI_Button(panel3, "+incr", 10, (GLUI_Update_CB)marker_button_callback);
	new GLUI_Button(panel3, "-incr", 11, (GLUI_Update_CB)marker_button_callback);
	new GLUI_Column(panel3);
	new GLUI_Button(panel3, "+x", 0, (GLUI_Update_CB)marker_button_callback);
	new GLUI_Button(panel3, "-x", 1, (GLUI_Update_CB)marker_button_callback);
	new GLUI_Button(panel3, "+y", 2, (GLUI_Update_CB)marker_button_callback);
	new GLUI_Button(panel3, "-y", 3, (GLUI_Update_CB)marker_button_callback);
	new GLUI_Button(panel3, "+z", 4, (GLUI_Update_CB)marker_button_callback);
	new GLUI_Button(panel3, "-z", 5, (GLUI_Update_CB)marker_button_callback);

	GLUI_Panel* panel9 = new GLUI_Panel(glui_window, "", GLUI_PANEL_NONE);
	new GLUI_Button(panel9, "quit", 0, (GLUI_Update_CB)exit);
	glClearColor(0.8, 0.8, 0.8, 0.0);
#else
	glutKeyboardFunc(keyboard_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);
	glutPassiveMotionFunc(passive_motion_callback);
	glClearColor(0.9, 0.3, 0.6, 0.0);
#endif

}

void ControlWindow::reshape(int x, int y, int w, int h) {
	glutSetWindow(glut_window);
	renderer->setProjection(x, y, w, h);
}

void ControlWindow::display(){

	glutSetWindow(glut_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef USE_MULTISAMPLING
    glEnable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_MULTISAMPLE_ARB);
#endif

#ifdef USE_GLUI
	for (short i=0; i<10; i++)
		if (camera->restoreStateAvailable(i)) camera_restore_buttons[i]->enable();
		else camera_restore_buttons[i]->disable();
#else
	//render Control Panel
	char s0[1000], s1[1000];

	sprintf(s0,"isovalue=%0.2f", HEADER.isoVal);
	renderer->renderString(-0.5,0.6,s0);

	sprintf(s1,"x,y = %i,%i", ui_common.mouse_x, ui_common.mouse_y);
	renderer->renderString(-0.5,0.5,s1);

    char* s00 = "Keyboard controls (image window)";
    char* s01 = "Sn = save camera state n";
    char* s02 = "Rn = restore camera state n";
    char* s03 = "W = write camera states to file";
    char* s04 = "A = read camera states from file";
    char* s05 = "K and J = move on x-axis";
    char* s06 = "I and O = move on y-axis";
    char* s07 = "N and M = move on z-axis";
    char* s08 = "H and G = rotate horizontal";
    char* s09 = "Y and U = rotate vertical";
    char* s10 = "Q = quit program";
    float x = -0.6, y = -0.4;
    renderer->renderString(x,y-=0.1,s00);
    renderer->renderString(x,y-=0.1,s01);
    renderer->renderString(x,y-=0.1,s02);
    renderer->renderString(x,y-=0.1,s03);
    renderer->renderString(x,y-=0.1,s04);
    renderer->renderString(x,y-=0.1,s05);
    renderer->renderString(x,y-=0.1,s06);
    renderer->renderString(x,y-=0.1,s07);
    renderer->renderString(x,y-=0.1,s08);
    renderer->renderString(x,y-=0.1,s09);
    renderer->renderString(x,y-=0.1,s10);
#endif

	glutSwapBuffers();
	glFlush();
}


void ControlWindow::mouse(int button, int state, int x, int y) {
}

void ControlWindow::motion(int x, int y) {
}

void ControlWindow::passive_motion(int x, int y) {
}

void ControlWindow::keyboard(unsigned char key, int x, int y) {
}

void ControlWindow::camera_button(int id) {
	if (input_processor == NULL) return;
	if (id == 1) {
		input_processor->keyboard('a',0,0);
	}
	else if (id == 2) {
		input_processor->keyboard('w',0,0);
	}
	else if ((id >=100) && (id < 200)) {
		if (input_processor != NULL) {
			input_processor->keyboard('r',0,0);
			input_processor->keyboard(char('0'+(id-100)),0,0);
		}
	}
	else if ((id >=200) && (id < 300)) {
		if (input_processor != NULL) {
			input_processor->keyboard('s',0,0);
			input_processor->keyboard(char('0'+(id-200)),0,0);
		}
	}
}

void ControlWindow::marker_button(int id) {
#ifdef USE_GLUI
	switch (id) {
	case 0:
		ui_common.marker_x += marker_incr;
		marker_x_text->set_int_val(ui_common.marker_x);
		break;
	case 1:
		ui_common.marker_x -= marker_incr;
		marker_x_text->set_int_val(ui_common.marker_x);
		break;
	case 2:
		ui_common.marker_y += marker_incr;
		marker_y_text->set_int_val(ui_common.marker_y);
		break;
	case 3:
		ui_common.marker_y -= marker_incr;
		marker_y_text->set_int_val(ui_common.marker_y);
		break;
	case 4:
		ui_common.marker_z += marker_incr;
		marker_z_text->set_int_val(ui_common.marker_z);
		break;
	case 5:
		ui_common.marker_z -= marker_incr;
		marker_z_text->set_int_val(ui_common.marker_z);
		break;
	case 10:
		marker_incr += 1;
		marker_incr_text->set_int_val(marker_incr);
		break;
	case 11:
		marker_incr -= 1;
		marker_incr_text->set_int_val(marker_incr);
		break;
	}
#endif
}
