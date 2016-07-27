/*
 *  show_text.h
 *  3d_headVolume_finalProject
 *
 *  Created by Marina Doherty on 3/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include <GL/glut.h>
#include <stdio.h>

void renderBitmapString(float x, float y, float z, void *font, char *string);
void renderString(float x, float y, char* string);
void renderText(GLint);
//void renderAllText(bool animateSlice);
