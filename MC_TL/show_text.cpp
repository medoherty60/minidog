//show_text.cpp
// code adapted from 
// http://www.lighthouse3d.com/opengl/glut/index.php?bmpfont and
// http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho

#include "show_text.h"
void renderBitmapString(float x, float y, float z, void *font, char *string) 
{  
  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void renderString(float x, float y, char* string)
{  
	glPushMatrix();		//save orientation of coordinate systems
	glLoadIdentity();
	renderBitmapString(x ,y, 0, GLUT_BITMAP_HELVETICA_18, string);
	glPopMatrix();		//restore orientation of coordinate systems
}
void renderText(int){
	
	char s[1000];
	glColor3f(0., 0., 0.);
//	sprintf(s,"fps=%0.2f",frame_rate);
//	renderString(0.,-0.55,s);
}
