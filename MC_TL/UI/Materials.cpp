/*
 * Materials.cpp
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

#include <cstring>
#include <GL/glut.h>
#include "Materials.h"

static void copyColor(float c1[4], float c2[4]){
	memcpy(c1, c2, 4*sizeof(float));
}

void setMaterial_Cube(){

	GLfloat mat_specular[4],  mat_diffuse[4];
	GLfloat mat_diffuse_bk[4], mat_ambient_bk[4], mat_specular_bk[4];

	GLfloat white[4] = 	{ 1., 1., 1., 1.};
	GLfloat red_dark[4] = { 0.0, 0.0, 0.75, 1.0 };
	GLfloat green_full[4] = { 0, 0., 1, 1.0, };
	GLfloat green_light[4] = { 0, 0.5, 0, 1.0,  };
	GLfloat green_dark[4] = { 0, .75, 0, 1.0, };
	GLfloat gray_dark[4] = {0.75, 0.75, 0.75, 1.0 };

	GLfloat mat_shininess[] = {70.0};

	copyColor(mat_specular, gray_dark);
	copyColor(mat_diffuse, white);
	copyColor(mat_diffuse_bk, green_full);
	copyColor(mat_ambient_bk, green_light);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

	glColorMaterial (GL_FRONT, GL_AMBIENT);
		//glEnable (GL_COLOR_MATERIAL);
}

void setMaterial_using_glColor(){

	GLfloat mat_specular[4],  mat_diffuse[4];
	GLfloat mat_diffuse_bk[4], mat_ambient_bk[4], mat_specular_bk[4];

	GLfloat white[4] = 	{ 1., 1., 1., 1.};
	GLfloat red_dark[4] = { 0.75, 0.75, 0.75, 1.0 };
	GLfloat green_full[4] = { 0, 1., 0, 1.0, };
	GLfloat green_light[4] = { 0, 0.5, 0, 1.0,  };
	GLfloat green_dark[4] = { 0, .75, 0, 1.0, };
	GLfloat gray_dark[4] = {0.75, 0.75, 0.75, 1.0 };

	GLfloat mat_shininess[] = {70.0};

	copyColor(mat_specular, gray_dark);
	copyColor(mat_diffuse, white);
	copyColor(mat_diffuse_bk, green_full);
	copyColor(mat_ambient_bk, green_light);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

	glColorMaterial (GL_FRONT, GL_AMBIENT);
	glEnable (GL_COLOR_MATERIAL);
}

void setMaterial(){

	int SpecularOn = 1;

	GLfloat mat_specular[4], mat_diffuse[4], mat_ambient[4];
	GLfloat mat_shininess[] = {70.0};

	GLfloat green_full[4] = { 0, 1., 0, 1.0, };
	GLfloat green_light[4] = { 0, 0.5, 0, 1.0,  };

	GLfloat black[4] = { 0.,0.,0.,1.0};
	GLfloat gray_dark[4] = { 0.65, 0.65, 0.75, 1.0 };
	GLfloat red_light[4] = { 0.5, 0., 0., 1.};
	GLfloat white[4] = 	{ 1., 1., 1., 1.};

	//Set up Surface Material Color
	copyColor(mat_diffuse, white);
    copyColor(mat_ambient, red_light);

	if(SpecularOn) copyColor(mat_specular, gray_dark);
	else copyColor(mat_specular, black);

	//Back Face Material (default: green)
	GLfloat mat_diffuse_bk[4], mat_ambient_bk[4];

	copyColor(mat_diffuse_bk, green_full);
	copyColor(mat_ambient_bk, green_light);

		//default: glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE )
		// set Ambient and Diffuse the same color using glColor in rendering routine
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

	//glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glEnable (GL_COLOR_MATERIAL);
}

void setMaterialUNUSED(){

	int SpecularOn = 1;

		//Set up Surface Material Color
	GLfloat mat_specular[4];
	GLfloat mat_shininess[] = {70.0};
	GLfloat mat_diffuse[] = {1.,1.,1.,1.};
	GLfloat mat_ambient[] = {0.5,0,0,1.};

	if(SpecularOn){
		mat_specular[0] = 0.75;
		mat_specular[1] = 0.75;
		mat_specular[2] = 0.75;
		mat_specular[3] = 1.0;
	}else {
		mat_specular[0] = 0.0;
		mat_specular[1] = 0.0;
		mat_specular[2] = 0.0;
		mat_specular[3] = 1.0;
	}

		//Back Face Material
	GLfloat mat_diffuse_bk[] = { 0, 1., 0, 1.0 };
	GLfloat mat_ambient_bk[] = { 0, 0.5, 0, 1.0 };

		//default: glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE )
		// set Ambient and Diffuse the same color using glColor in rendering routine
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

		//glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		//glEnable (GL_COLOR_MATERIAL);
}
