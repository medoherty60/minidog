/*
 * Renderer.h
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

#ifndef UI_RENDERER_H_
#define UI_RENDERER_H_
#include <vector>
using namespace std;
#include "../Color.h"
#include "../Point.h"
#include "../Vector.h"

class Renderer {
public:
	Renderer();

	void setProjection(int x, int y, int w, int h);

	void drawMarker();
	void displayLines();
	void renderWireFrame(float borderwidth, float r, float g, float b, float a);
	void displayFaces(int animation_face_code);
	void displayNormal(Point p1,Point p2, Point p3,
					   Vector& n1,Vector& n2,Vector& n3,
					   float osx,float osy,float osz, int& mode);

	void drawTriangle(vector<Point>& p, vector<Vector>& n, Color& c);
	void drawLine(Point& p1, Point& p2, Color& c);
	void drawCube(float dx, float dy, float dz, float osx, float osy, float osz);
	void renderBitmapString(float x, float y, float z, void *font, char *string);
	void renderString(float x, float y, char* string);

private:
};




#endif /* UI_RENDERER_H_ */
