/*
 * ObjectWindow.cpp
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

#include <GL/glut.h>
#include <ctime>
#include "ObjectWindow.h"
#include "Materials.h"
#include "Renderer.h"
#include "LightManager.h"
#include "Camera.h"
#include "InputProcessor.h"

ObjectWindow* ObjectWindow::callback_rcvr = NULL;

ObjectWindow::ObjectWindow(int parent_window, int x, int y, int width, int height) {

	camera = NULL;
	input_processor = NULL;

	if (callback_rcvr == NULL) callback_rcvr = this;

	glut_window = glutCreateSubWindow(parent_window, x, y, width, height);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT, GL_FILL);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	if(!ui_common.bothFaces){
		//suppress backfaces
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	light_manager.setup_lightObject();
	light_manager.light0_distance(20.);
	//setMaterial();
	setMaterial_using_glColor();

	// animation control
	last_frame_time = 0;
	elapsed_frame_time = 0.0;
	first_animation_frame = true;
	enable_animation = true;
	animation_face_code = 0;
	frame_count = 0;

	// call backs from GLUT for this window
	glutDisplayFunc(display_callback);
	glutKeyboardFunc(keyboard_callback);
	//glutSpecialFunc(specialKey);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);
	glutPassiveMotionFunc(passive_motion_callback);
}

void ObjectWindow::attachCamera(Camera* _camera) {
	camera = _camera;
	// flip the image upside-down at the start
	camera->doRotationAngleAxis(180.0,1.,0.,0.);
}

void ObjectWindow::reshape(int x, int y, int w, int h) {
	glutSetWindow(glut_window);
	Renderer::setProjection(x, y, w, h);
	// tell the camera about the new dimensions of the object window
	if (camera != NULL) camera->reshape(w, h);
}

void ObjectWindow::display() {

	glutSetWindow(glut_window);
	if (ui_common.blackBG)
		glClearColor (0.0, 0.0, 0.0, 0.0);
	else
		glClearColor (1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();		//save previous matrix - contains settings for perspective projection
	glLoadIdentity();		//reset matrix  // glTranslatef(0., 0., -4.);
	gluPerspective(40., camera->getWindowWidth()/camera->getWindowHeight(), 0.1, 60.);   //set clipping volume
	glTranslatef(0., 0., -3.);

#ifdef USE_MULTISAMPLING
    glEnable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_MULTISAMPLE_ARB);
#endif

    if (ui_common.enable_animation) updateAnimation();
    Renderer::displayFaces(animation_face_code);
	glDisable(GL_LIGHTING);
	//displayNormal();

	if (ui_common.showCube) {
		// draw a unit wireframe cube around the rendered volume
		if (ui_common.blackBG)
			Renderer::renderWireFrame(1.,1.,1.,1.,1.);       //borderwidth,r,g,b,a
		else
			Renderer::renderWireFrame(1.,0.,0.,0.,1.);
	}

	if (ui_common.showAxes) {
		Renderer::displayLines(); // draw the coordinate axes
	}

	if (ui_common.marker_enabled) {
		Renderer::drawMarker();
	}

	// turn on the lights
	glEnable(GL_LIGHTING);
	//renderLightObjects();

	//setOrthogrpahicProjection();
	glutSwapBuffers();
	glFlush();
}

// mouse is called by GLUT when a mouse button is pressed or released
void ObjectWindow::mouse(int button, int state, int x, int y) {
	if (input_processor != NULL) input_processor->mouse(button, state, x, y);
}

// motion is called by GLUT when mouse is moved within the window with a button pressed
void ObjectWindow::motion(int x, int y) {
	if (input_processor != NULL) input_processor->motion(x, y);
}

// passive_motion is called by GLUT when mouse is moved within the window with no button pressed
void ObjectWindow::passive_motion(int x, int y) {
	if (input_processor != NULL) input_processor->passive_motion(x, y);
}

// keyboard is called by GLUT when an ASCII key is pressed
void ObjectWindow::keyboard(unsigned char key, int x, int y) {
	if (input_processor != NULL) input_processor->keyboard(key, x, y);
}


/*
// auto-rotate animation
void ObjectWindow::updateAnimation() {
	if (first_animation_frame) {
		elapsed_frame_time = 0.0f;
		last_frame_time = clock();
		first_animation_frame = false;
	}
	else {
		 clock_t time = clock() ;
		 elapsed_frame_time = (double)(time - last_frame_time)/CLOCKS_PER_SEC;
		 last_frame_time = time;
	}
	float angle = elapsed_frame_time;
	camera->doRotationAngleAxis(angle*2.0,0.0,1.0,0.0);
	camera->doRotationAngleAxis(angle,1.0,0.0,0.0);
}
*/

void ObjectWindow::updateAnimation() {
	if (ui_common.reset_animation) {
		resetAnimation();
		ui_common.reset_animation = false;
	}
	if (first_animation_frame) {
		elapsed_frame_time = 0.0f;
		last_frame_time = clock();
		first_animation_frame = false;
		animation_face_code = 0;
	}
	else {
		 clock_t time = clock() ;
		 elapsed_frame_time = (double)(time - last_frame_time)/CLOCKS_PER_SEC;
		 last_frame_time = time;
		 frame_count++;
		 if (frame_count >= 10) {
			 animation_face_code+=1;
			 frame_count = 0;
		 }
	}
}
