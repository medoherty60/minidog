/*
 *  LightManager.cpp
 *  Created by Marina Doherty on 6/19/12.
 */

#include <GL/glut.h>
#include "LightManager.h"

LightManager light_manager;

void LightManager::setup_lightObject(){

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	
	 //+LIGHT 1
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	//Two-side Lighting is enabled (reverse the normal for back faces)
	lightparam[0] = 0.0;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lightparam);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	/*
	 //+LIGHT 2
	 glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	 glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	 glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);	
	 glEnable(GL_LIGHT1);
	*/
}

